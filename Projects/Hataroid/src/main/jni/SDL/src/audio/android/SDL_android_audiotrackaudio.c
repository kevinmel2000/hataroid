/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org

    This file written by Ryan C. Gordon (icculus@icculus.org)
*/
#include "SDL_config.h"

#include "SDL_rwops.h"
#include "SDL_timer.h"
#include "SDL_audio.h"
#include "../SDL_audiomem.h"
#include "../SDL_audio_c.h"
#include "../SDL_audiodev_c.h"
#include "SDL_android_audiotrackaudio.h"

/* The tag name used by Android AudioTrack audio */
#define ANDROIDAUDIOTRACK_DRIVER_NAME         "android-audiotrack"

static SDL_AudioDevice* ANDROIDAUDIOTRACK_CreateDevice(int devindex);
static int              ANDROIDAUDIOTRACK_Available(void);

AudioBootStrap ANDROIDAUDIOTRACK_bootstrap = {
	ANDROIDAUDIOTRACK_DRIVER_NAME, "SDL android audiotrack audio driver",
	ANDROIDAUDIOTRACK_Available, ANDROIDAUDIOTRACK_CreateDevice
};

/* Audio driver functions */
static int      ANDROIDAUDIOTRACK_OpenAudio(_THIS, SDL_AudioSpec *spec);
static void     ANDROIDAUDIOTRACK_WaitAudio(_THIS);
static void     ANDROIDAUDIOTRACK_PlayAudio(_THIS);
static Uint8*   ANDROIDAUDIOTRACK_GetAudioBuf(_THIS);
static void     ANDROIDAUDIOTRACK_CloseAudio(_THIS);

static void     ANDROIDAUDIOTRACK_ThreadInit(_THIS);
static void     ANDROIDAUDIOTRACK_ThreadDeinit(_THIS);

static int      initAndroidAudio(_THIS, SDL_AudioSpec *spec, Uint16 format, int freq, int channels, int bufSizeBytes);
static void     deinitAndroidAudio(_THIS);

extern JavaVM*          g_jvm;
extern struct JNIAudio  g_jniAudioInterface;
extern volatile int     g_validMainActivity;

//extern volatile int     g_emuReady;
extern int              kMaxAudioMuteFrames;
extern volatile int     g_audioMute;
extern volatile int     g_audioMuteFrames;

volatile int            _validLenBytes = 0;
volatile int            _deviceMinBufSize = 0;

/* Audio driver bootstrap functions */
static int ANDROIDAUDIOTRACK_Available(void)
{
	const char *envr = SDL_getenv("SDL_AUDIODRIVER");
	if (envr && (SDL_strcmp(envr, ANDROIDAUDIOTRACK_DRIVER_NAME) == 0)) {
		return(1);
	}
	return(0);
}

static void ANDROIDAUDIOTRACK_DeleteDevice(SDL_AudioDevice *device)
{
	SDL_free(device->hidden);
	SDL_free(device);
}

static SDL_AudioDevice *ANDROIDAUDIOTRACK_CreateDevice(int devindex)
{
	SDL_AudioDevice *this;

	/* Initialize all variables that we clean on shutdown */
	this = (SDL_AudioDevice *)SDL_malloc(sizeof(SDL_AudioDevice));
	if ( this ) {
		SDL_memset(this, 0, (sizeof *this));
		this->hidden = (struct SDL_PrivateAudioData *)SDL_malloc((sizeof *this->hidden));
	}
	if ( (this == NULL) || (this->hidden == NULL) ) {
		SDL_OutOfMemory();
		if ( this ) {
			SDL_free(this);
		}
		return(0);
	}
	SDL_memset(this->hidden, 0, (sizeof *this->hidden));

	/* Set the function pointers */
	this->OpenAudio = ANDROIDAUDIOTRACK_OpenAudio;
	this->WaitAudio = ANDROIDAUDIOTRACK_WaitAudio;
	this->PlayAudio = ANDROIDAUDIOTRACK_PlayAudio;
	this->GetAudioBuf = ANDROIDAUDIOTRACK_GetAudioBuf;
	this->CloseAudio = ANDROIDAUDIOTRACK_CloseAudio;

	this->ThreadInit = ANDROIDAUDIOTRACK_ThreadInit;
	this->ThreadDeinit = ANDROIDAUDIOTRACK_ThreadDeinit;

	this->free = ANDROIDAUDIOTRACK_DeleteDevice;

	return this;
}

/* This function waits until it is possible to write a full sound buffer */
static void ANDROIDAUDIOTRACK_WaitAudio(_THIS)
{
#if 0
	/* Don't block on first calls to simulate initial fragment filling. */
	if (this->hidden->initial_calls)
	{
		//this->hidden->initial_calls--;
	}
	else
#endif
	 if (g_validMainActivity == 0)
	{
		SDL_Delay(3);//this->hidden->write_delay);
	}
	else
	{
		SDL_Delay(2);//1);//this->hidden->write_delay);
	}
}

static void ANDROIDAUDIOTRACK_PlayAudio(_THIS)
{
	struct SDL_PrivateAudioData *hidden = this->hidden;

	int validShorts = _validLenBytes >> 1;

	if (hidden->numjShorts == 0 || this->paused || validShorts <= 0)// || _doubleBusError)// || g_emuReady == 0)
	{
		return;
	}

#if 0
	if (this->hidden->initial_calls)
	{
		this->hidden->initial_calls--;
	}
#endif

	JNIEnv *j_env = hidden->playThreadjenv; //g_jniAudioInterface.android_env;
	if (j_env)
	{
		validShorts = validShorts > hidden->numjShorts ? hidden->numjShorts : validShorts;

		if (g_validMainActivity)
		{
			{
				(*j_env)->SetShortArrayRegion(j_env, hidden->playThreadjavaSendBuf, 0, validShorts, (int16_t*)this->hidden->mixbuf);
				(*j_env)->CallVoidMethod(j_env, g_jniAudioInterface.android_mainActivity, g_jniAudioInterface.sendAudio, this->hidden->playThreadjavaSendBuf, validShorts, 0);
			}
		}
	}

	_validLenBytes = 0;
}

static Uint8 *ANDROIDAUDIOTRACK_GetAudioBuf(_THIS)
{
	return(this->hidden->mixbuf);
}

static void ANDROIDAUDIOTRACK_CloseAudio(_THIS)
{
	if ( this->hidden->mixbuf != NULL ) {
		SDL_FreeAudioMem(this->hidden->mixbuf);
		this->hidden->mixbuf = NULL;
	}

	deinitAndroidAudio(this);
}

static int ANDROIDAUDIOTRACK_OpenAudio(_THIS, SDL_AudioSpec *spec)
{
	if (initAndroidAudio(this, spec, spec->format, spec->freq, spec->channels, spec->size) < 0) {
		return -1;
	}

	float bytes_per_sec = 0.0f;

	/* Allocate mixing buffer */
	this->hidden->mixlen = spec->size;
	this->hidden->mixbuf = (Uint8 *) SDL_AllocAudioMem(this->hidden->mixlen);
	if ( this->hidden->mixbuf == NULL ) {
		return(-1);
	}
	SDL_memset(this->hidden->mixbuf, spec->silence, spec->size);

	bytes_per_sec = (float) (((spec->format & 0xFF) / 8) * spec->channels * spec->freq);

	/*
	 * We try to make this request more audio at the correct rate for
	 *  a given audio spec, so timing stays fairly faithful.
	 * Also, we have it not block at all for the first two calls, so
	 *  it seems like we're filling two audio fragments right out of the
	 *  gate, like other SDL drivers tend to do.
	 */
	//this->hidden->initial_calls = 2;
	this->hidden->write_delay = (Uint32) ((((float) spec->size) / bytes_per_sec) * 1000.0f);

	/* We're ready to rock and roll. :-) */
	return(0);
}

static void ANDROIDAUDIOTRACK_ThreadInit(_THIS)
{
	Debug_Printf("SDL_Audio Native->Java Audio Thread Init");

	JNIEnv *env = NULL;

	int status = (*g_jvm)->GetEnv(g_jvm, (void**)&env, JNI_VERSION_1_6);
	int attached = 0;

	if(status < 0)
	{
		status = (*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL);
		attached = 1;
	}

	this->hidden->playThreadEnvAttached = attached;
	this->hidden->playThreadjenv = env;

	jshortArray sendBuf = (*env)->NewShortArray(env, this->hidden->numjShorts);
	this->hidden->playThreadjavaSendBuf = (*env)->NewGlobalRef(env, sendBuf);
	(*env)->DeleteLocalRef(env, sendBuf);

	this->spec.userdata = env;
}

static void ANDROIDAUDIOTRACK_ThreadDeinit(_THIS)
{
	Debug_Printf("SDL_Audio Native Audio Thread DeInit");

	JNIEnv *j_env = this->hidden->playThreadjenv;
	(*j_env)->DeleteGlobalRef(j_env, this->hidden->playThreadjavaSendBuf);
	this->hidden->playThreadjavaSendBuf = 0;

	if (this->hidden->playThreadEnvAttached)
	{
		(*g_jvm)->DetachCurrentThread(g_jvm);
	}
	this->hidden->playThreadEnvAttached = 0;
	this->hidden->playThreadjenv = 0;
}

static int initAndroidAudio(_THIS, SDL_AudioSpec *spec, Uint16 format, int freq, int channels, int bufSizeBytes)
{
	JNIEnv *j_env = g_jniAudioInterface.android_env;
	if (j_env)
	{
		int bits = 16;
		int bytes = 2;

		if (format != AUDIO_S16SYS) { // only signed 16 supported atm
			return -1;
		}

		Debug_Printf("SDL_Audio Native->Java AudioInit_callback");

		jint minBufSize = (*j_env)->CallIntMethod(j_env,
				g_jniAudioInterface.android_mainActivity, g_jniAudioInterface.getMinBufSize,
				freq, bits, channels);

		_deviceMinBufSize = minBufSize;

		Debug_Printf("SDL_Audio Requested BufSize: %d, minSize: %d", bufSizeBytes, minBufSize);

		int deviceBufSize = bufSizeBytes;
		if (deviceBufSize < minBufSize)
		{
			Debug_Printf("Device MinBufSize: %d", minBufSize);
			deviceBufSize = minBufSize;

			spec->samples = deviceBufSize / (channels * bytes);
			spec->size = deviceBufSize;
		}

		Debug_Printf("SDL_Audio Native->Java AudioInit_callback");

		(*j_env)->CallVoidMethod(j_env,
				g_jniAudioInterface.android_mainActivity, g_jniAudioInterface.initAudio,
				freq, bits, channels,
				deviceBufSize);

		this->hidden->numjShorts = deviceBufSize/2;

		return 0;
	}

	return -1;
}

static void deinitAndroidAudio(_THIS)
{
	JNIEnv *j_env = g_jniAudioInterface.android_env;
	if (j_env)
	{
		Debug_Printf("SDL_Audio Native->Java AudioDeinit_callback");

		(*j_env)->CallVoidMethod(j_env,
				g_jniAudioInterface.android_mainActivity, g_jniAudioInterface.deinitAudio);
	}
}
