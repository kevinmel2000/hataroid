/*
  Hatari - audio.c

  This file is distributed under the GNU General Public License, version 2
  or at your option any later version. Read the file gpl.txt for details.

  This file contains the routines which pass the audio data to the SDL library.
*/
const char Audio_fileid[] = "Hatari audio.c : " __DATE__ " " __TIME__;

#include <SDL.h>
#include <JNI.h>

#include "main.h"
#include "audio.h"
#include "configuration.h"
#include "log.h"
#include "sound.h"
#include "dmaSnd.h"
#include "falcon/crossbar.h"

#include "screen.h"
#include "video.h"	/* FIXME: video.h is dependent on HBL_PALETTE_LINES from screen.h */

int nAudioFrequency = 44100;			/* Sound playback frequency */
bool bSoundWorking = false;			/* Is sound OK */
static volatile bool bPlayingBuffer = false;	/* Is playing buffer? */
int SoundBufferSize = 1024 / 4;			/* Size of sound buffer (in samples) */
int CompleteSndBufIdx;				/* Replay-index into MixBuffer */
int SdlAudioBufferSize = 0;			/* in ms (0 = use default) */
int pulse_swallowing_count = 0;			/* Sound disciplined emulation rate controlled by  */
						/*  window comparator and pulse swallowing counter */

//extern Sint64 Time_GetTicks(void);

extern int g_vsync;
extern float g_scrRefreshRate;

//extern volatile int g_emuReady;
extern volatile int _doubleBusError;
extern volatile int _runTillQuit;

/*-----------------------------------------------------------------------*/
/**
 * SDL audio callback function - copy emulation sound to audio system.
 */

static int Audio_CallBack(void *userdata, Uint8 *stream, int len)
{
	//JNIEnv* env = (JNIEnv*)userdata;

	// check playback rate
	if (g_vsync && g_scrRefreshRate > 0)
	{
		float playBackRate = g_scrRefreshRate / ((float)nScreenRefreshRate);
		SDL_PlaybackRateAudio(playBackRate);
	}

	Sint16 *pBuffer;
	int i, window, nSamplesPerFrame;

	pBuffer = (Sint16 *)stream;
	len = len >> 2;  // Use length in samples (16 bit stereo), not in bytes

	/* Adjust emulation rate within +/- 0.58% (10 cents) occasionally,
	 * to synchronize sound. Note that an octave (frequency doubling)
	 * has 12 semitones (12th root of two for a semitone), and that
	 * one semitone has 100 cents (1200th root of two for one cent).
	 * Ten cents are desired, thus, the 120th root of two minus one is
	 * multiplied by 1,000,000 to convert to microseconds, and divided
	 * by nScreenRefreshRate=60 to get a 96 microseconds swallow size.
	 * (2^(10cents/(12semitones*100cents)) - 1) * 10^6 / nScreenRefreshRate
	 * See: main.c - Main_WaitOnVbl()
	 */

	pulse_swallowing_count = 0;	/* 0 = Unaltered emulation rate */

	if (ConfigureParams.Sound.bEnableSoundSync)
	{
		/* Sound synchronized emulation */
		nSamplesPerFrame = nAudioFrequency/nScreenRefreshRate;
		window = (nSamplesPerFrame > SoundBufferSize) ? nSamplesPerFrame : SoundBufferSize;

		int min = window + (window >> 1); //1.5x
		int max = (window << 1) + (window >> 2); // 2.25x

		/* Window Comparator for SoundBufferSize */
		if (nGeneratedSamples < min)
		{
		/* Increase emulation rate to maintain sound synchronization */
			pulse_swallowing_count = -5793 / nScreenRefreshRate;
			//Debug_Printf("not enough audio, speeding emu up: gen: %d, thresh: %d - %d", nGeneratedSamples, min, max);
		}
		else
		if (nGeneratedSamples > max)
		{
		/* Decrease emulation rate to maintain sound synchronization */
			pulse_swallowing_count = 5793 / nScreenRefreshRate;
			//Debug_Printf("too much audio, slowing emu down: gen: %d, thresh: %d - %d", nGeneratedSamples, min, max);
		}

		/* Otherwise emulation rate is unaltered. */
	}

	//Debug_Printf("req audio len: %d, generated: %d, freeBufs: %d", len, nGeneratedSamples, _sdlAudio_dbgFreeBufs);

	if (nGeneratedSamples < len || _runTillQuit != 0 || _doubleBusError)// && g_emuReady != 0)
	{
		if (g_vsync)
		{
		//	return 0;
		}

		memset(stream, 0, len<<2);
		return (len << 2);
	}

	//Debug_Printf("sending audio: %d samples, free: %d, gen: %d", len, maxFreeBuf, nGeneratedSamples);

	if (ConfigureParams.Hataroid.downmixStereo)
	{
		for (i = 0; i < len; i++)
        {
            int idx = (CompleteSndBufIdx + i) % MIXBUFFER_SIZE;
            int val = (MixBuffer[idx][0] + MixBuffer[idx][1]) >> 1;
            *pBuffer++ = val;
            *pBuffer++ = val;
        }
	}
	else
	{
		for (i = 0; i < len; i++)
		{
			int idx = (CompleteSndBufIdx + i) % MIXBUFFER_SIZE;
			*pBuffer++ = MixBuffer[idx][0];
			*pBuffer++ = MixBuffer[idx][1];
		}
	}

	CompleteSndBufIdx += len;
	nGeneratedSamples -= len;

	CompleteSndBufIdx = CompleteSndBufIdx % MIXBUFFER_SIZE;

	return (len << 2);
}

/*-----------------------------------------------------------------------*/
/**
 * Initialize the audio subsystem. Return true if all OK.
 * We use direct access to the sound buffer, set to a unsigned 8-bit mono stream.
 */
void Audio_Init(void)
{
	SDL_AudioSpec desiredAudioSpec;    /* We fill in the desired SDL audio options here */

	/* Is enabled? */
	if (!ConfigureParams.Sound.bEnableSound)
	{
		/* Stop any sound access */
		Log_Printf(LOG_DEBUG, "Sound: Disabled\n");
		bSoundWorking = false;
		return;
	}

	/* Init the SDL's audio subsystem: */
	if (SDL_WasInit(SDL_INIT_AUDIO) == 0)
	{
		if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
		{
			fprintf(stderr, "Could not init audio: %s\n", SDL_GetError() );
			bSoundWorking = false;
			return;
		}
	}

	int nSamplesPerFrame = nAudioFrequency/nScreenRefreshRate; // hack data passing
//	if (g_vsync) {
//		nSamplesPerFrame = nAudioFrequency / 50; // max number of samples per frame in all refreshrates
//	}

	/* Set up SDL audio: */
	desiredAudioSpec.freq = nAudioFrequency;
	desiredAudioSpec.format = AUDIO_S16SYS;		/* 16-Bit signed */
	desiredAudioSpec.channels = 2;			/* stereo */
	desiredAudioSpec.callback = Audio_CallBack;
	desiredAudioSpec.userdata = nSamplesPerFrame;

	/* In most case, setting samples to 1024 will give an equivalent */
	/* sdl sound buffer of ~20-30 ms (depending on freq). */
	/* But setting samples to 1024 for all the freq can cause some faulty */
	/* OS sound drivers to add an important delay when playing sound at lower freq. */
	/* In that case we use SdlAudioBufferSize (in ms) to compute a value */
	/* of samples that matches the corresponding freq and buffer size. */
	if ( SdlAudioBufferSize == 0 )			/* don't compute "samples", use default value */
		desiredAudioSpec.samples = 1024;	/* buffer size in samples */
	else
	{
		int samples = (desiredAudioSpec.freq / 1000) * SdlAudioBufferSize;

		// my audio driver will take care of this as required
//		int power2 = 1;
//		while ( power2 < samples )		    /* compute the power of 2 just above samples */
//            power2 *= 2;

		//fprintf ( stderr , "samples %d power %d\n" , samples , power2 );
		//desiredAudioSpec.samples = power2;	/* number of samples corresponding to the requested SdlAudioBufferSize */
		desiredAudioSpec.samples = samples;
	}

	if (SDL_OpenAudio(&desiredAudioSpec, NULL))	/* Open audio device */
	{
		fprintf(stderr, "Can't use audio: %s\n", SDL_GetError());
		bSoundWorking = false;
		ConfigureParams.Sound.bEnableSound = false;
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
		return;
	}

	SoundBufferSize = desiredAudioSpec.size;    /* May be different than the requested one! */
	SoundBufferSize /= 4;				        /* bytes -> samples (16 bit signed stereo -> 4 bytes per sample) */
	if (SoundBufferSize > MIXBUFFER_SIZE/2)
	{
		fprintf(stderr, "Warning: Soundbuffer size is too big!\n");
	}

	/* All OK */
	bSoundWorking = true;
	/* And begin */
	Audio_EnableAudio(true);
}


/*-----------------------------------------------------------------------*/
/**
 * Free audio subsystem
 */
void Audio_UnInit(void)
{
	if (bSoundWorking)
	{
		/* Stop */
		Audio_EnableAudio(false);

		SDL_CloseAudio();

		bSoundWorking = false;
	}
}


/*-----------------------------------------------------------------------*/
/**
 * Lock the audio sub system so that the callback function will not be called.
 */
void Audio_Lock(void)
{
	SDL_LockAudio();
}


/*-----------------------------------------------------------------------*/
/**
 * Unlock the audio sub system so that the callback function will be called again.
 */
void Audio_Unlock(void)
{
	SDL_UnlockAudio();
}


/*-----------------------------------------------------------------------*/
/**
 * Set audio playback frequency variable, pass as PLAYBACK_xxxx
 */
void Audio_SetOutputAudioFreq(int nNewFrequency)
{
	/* Do not reset sound system if nothing has changed! */
	if (nNewFrequency != nAudioFrequency)
	{
		/* Set new frequency */
		nAudioFrequency = nNewFrequency;

		if (ConfigureParams.System.nMachineType == MACHINE_FALCON)
		{
			/* Compute Ratio between host computer sound frequency and Hatari's sound frequency. */
			Crossbar_Compute_Ratio();
		}
		else if (ConfigureParams.System.nMachineType != MACHINE_ST)
		{
			/* Adapt LMC filters to this new frequency */			
			DmaSnd_Init_Bass_and_Treble_Tables();
		}

		/* Re-open SDL audio interface if necessary: */
		if (bSoundWorking)
		{
			Audio_UnInit();
			Audio_Init();
		}
	}

	if ((ConfigureParams.System.nMachineType == MACHINE_ST) &&
		(nAudioFrequency >= 40000))
	{
		/* Apply YM2149 C10 filter. */
		UseLowPassFilter = true;
	}
	else
	{
		UseLowPassFilter = false;
	}
}


/*-----------------------------------------------------------------------*/
/**
 * Start/Stop sound buffer
 */
void Audio_EnableAudio(bool bEnable)
{
	if (bEnable && !bPlayingBuffer)
	{
		/* Start playing */
		SDL_PauseAudio(false);
		bPlayingBuffer = true;
	}
	else if (!bEnable && bPlayingBuffer)
	{
		/* Stop from playing */
		SDL_PauseAudio(true);
		bPlayingBuffer = false;
	}
}
