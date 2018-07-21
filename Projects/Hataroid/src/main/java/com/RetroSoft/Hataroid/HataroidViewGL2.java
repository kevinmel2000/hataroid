package com.RetroSoft.Hataroid;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.util.Log;
import android.view.Choreographer;
import android.view.MotionEvent;

import com.RetroSoft.Hataroid.Input.Input;
import com.RetroSoft.Hataroid.Input.InputMouse;
import com.RetroSoft.Hataroid.Util.BitFlags;

// A simple GLSurfaceView sub-class that demonstrate how to perform
// OpenGL ES 2.0 rendering into a GL Surface. Note the following important
// details:
// - The class must use a custom context factory to enable 2.0 rendering.
//   See ContextFactory class definition below.
//
// - The class must use a custom EGLConfigChooser to be able to select
//   an EGLConfig that supports 2.0. This is done by providing a config
//   specification to eglChooseConfig() that has the attribute
//   EGL10.ELG_RENDERABLE_TYPE containing the EGL_OPENGL_ES2_BIT flag
//   set. See ConfigChooser class definition below.
//
// - The class must select the surface's format, then choose an EGLConfig
//   that matches it exactly (with regards to red/green/blue/alpha channels
//   bit depths). Failure to do so would result in an EGL_BAD_MATCH error.
//

class HataroidViewGL2 extends GLSurfaceView
{
	private static String TAG = "HataroidViewGL2";
	private static final boolean DEBUG = false;

	public static HataroidViewGL2 instance;
	
	public static final int	MaxSimultaneousTouches = 5;
	public Object		m_inputMuteX;
	public float		m_touchX[] = new float [MaxSimultaneousTouches];
	public float		m_touchY[] = new float [MaxSimultaneousTouches];
	public boolean		m_touched[] = new boolean [MaxSimultaneousTouches];
	public int			m_mouseButtons = 0;
	public float		m_mouseX = 0;
	public float		m_mouseY = 0;
	
	public boolean		m_tryMouse = true;

	public boolean		m_newMouseDisabled = false;

	public boolean		m_pendingNewMouseDisabled = false;
	public boolean		m_newNewMouseDisabledVal = false;

	private Renderer        m_renderer = null;

	private Choreographer.FrameCallback m_vsyncFrameCallback = null;
	private boolean                     m_vsyncEnabled = false;

	public HataroidViewGL2(Context context)
	{
		super(context);
		init(false, 0, 0);
	}

	public HataroidViewGL2(Context context, boolean translucent, int depth, int stencil)
	{
		super(context);
		init(translucent, depth, stencil);
	}

	public boolean onTouchEvent(final MotionEvent event)
	{
		synchronized (m_inputMuteX)
		{
			int action = event.getActionMasked();

			//Log.i(TAG, "action: " + action);
			switch (action)
			{
				case MotionEvent.ACTION_DOWN:
				case MotionEvent.ACTION_POINTER_DOWN:
				{
					int pointerIndex = event.getActionIndex();
					int pointerId = event.getPointerId(pointerIndex);
					//Log.i(TAG, "pointerIndex: " + pointerIndex + ", pointerID: " + pointerId + ", pos: " + event.getX(pointerIndex) + ", " + event.getY(pointerIndex));

					boolean isMouse = false;
					if (m_tryMouse && !m_newMouseDisabled)
					{
						try
						{
							if (event.getToolType(pointerIndex) == MotionEvent.TOOL_TYPE_MOUSE)
							{
								m_mouseButtons = event.getButtonState();
								m_mouseX = event.getX(pointerIndex);
								m_mouseY = event.getY(pointerIndex);
								//Log.i(TAG, "btn: " + m_mouseButtons);
								isMouse = true;
							}
						}
						catch (Error e)
						{
							m_tryMouse = false;
						}
						catch (Exception e)
						{
							m_tryMouse = false;
						}
					}

					if (!isMouse && pointerId < MaxSimultaneousTouches)
					{
						m_touchX[pointerId] = event.getX(pointerIndex);
						m_touchY[pointerId] = event.getY(pointerIndex);
						m_touched[pointerId] = true;
					}
					break;
				}
				case MotionEvent.ACTION_UP:
				case MotionEvent.ACTION_POINTER_UP:
				case MotionEvent.ACTION_CANCEL:
				{
					int pointerIndex = event.getActionIndex();
					int pointerId = event.getPointerId(pointerIndex);

					boolean isMouse = false;
					if (m_tryMouse && !m_newMouseDisabled)
					{
						try
						{
							if (event.getToolType(pointerIndex) == MotionEvent.TOOL_TYPE_MOUSE)
							{
								m_mouseButtons = event.getButtonState();
								m_mouseX = event.getX(pointerIndex);
								m_mouseY = event.getY(pointerIndex);
								//Log.i(TAG, "btn: " + m_mouseButtons);
								isMouse = true;
							}
						}
						catch (Error e)
						{
							m_tryMouse = false;
						}
						catch (Exception e)
						{
							m_tryMouse = false;
						}
					}

					if (!isMouse && pointerId < MaxSimultaneousTouches)
					{
						m_touchX[pointerId] = event.getX(pointerIndex);
						m_touchY[pointerId] = event.getY(pointerIndex);
						m_touched[pointerId] = false;
					}
					break;
				}
				case MotionEvent.ACTION_MOVE:
				{
					int pointerCount = event.getPointerCount();
					for (int pointerIndex = 0; pointerIndex < pointerCount; ++pointerIndex)
					{
						int pointerId = event.getPointerId(pointerIndex);
						if (pointerId < MaxSimultaneousTouches)
						{
							boolean isMouse = false;
							if (m_tryMouse && !m_newMouseDisabled) // TODO: move out of for loop
							{
								try
								{
									if (event.getToolType(pointerIndex) == MotionEvent.TOOL_TYPE_MOUSE)
									{
										m_mouseButtons = event.getButtonState();
										m_mouseX = event.getX(pointerIndex);
										m_mouseY = event.getY(pointerIndex);
										//Log.i(TAG, "btn: " + m_mouseButtons + " pos: " + m_mouseX + ", " + m_mouseY);
										isMouse = true;
									}
								}
								catch (Error e)
								{
									m_tryMouse = false;
								}
								catch (Exception e)
								{
									m_tryMouse = false;
								}
							}

							if (!isMouse && pointerId < MaxSimultaneousTouches)
							{
								m_touchX[pointerId] = event.getX(pointerIndex);
								m_touchY[pointerId] = event.getY(pointerIndex);
							}
						}
					}
				}
			}
		}

//		try {
//			Thread.sleep(8);
//		} catch (Exception e) {
//		}

		return true;
	}	

	public void enableNewMouse(boolean enableNewMouse)
	{
		try
		{
			synchronized (m_inputMuteX)
			{
				if (m_tryMouse)
				{
					if (m_mouseButtons == 0)
					{
						m_newMouseDisabled = !enableNewMouse;
					}
					else
					{
						// wait till all mouse buttons are off before disabling
						m_pendingNewMouseDisabled = true;
						m_newNewMouseDisabledVal = !enableNewMouse;
					}
				}
			}
		}
		catch (Error e)
		{
		}
		catch (Exception e)
		{
		}
	}
	
	public void checkNewMouseDisabling()
	{
		synchronized (m_inputMuteX)
		{
			if (m_pendingNewMouseDisabled && m_mouseButtons == 0)
			{
				m_pendingNewMouseDisabled = false;
				m_newMouseDisabled = m_newNewMouseDisabledVal;
			}
		}
	}

	private void init(boolean translucent, int depth, int stencil)
	{
		Log.i("hataroid", "translucent: " + translucent + ", depth: " + depth + ", stencil: " + stencil);

		instance = this;

		m_inputMuteX = new Object();

		m_tryMouse = (android.os.Build.VERSION.SDK_INT >= 12);

		// By default, GLSurfaceView() creates a RGB_565 opaque surface.
		// If we want a translucent one, we should change the surface's
		// format here, using PixelFormat.TRANSLUCENT for GL Surfaces
		// is interpreted as any 32-bit surface with alpha by SurfaceFlinger.
		if (translucent)
		{
			this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
		}

		// Setup the context factory for 2.0 rendering.
		// See ContextFactory class definition below
		setEGLContextFactory(new ContextFactory());

		// We need to choose an EGLConfig that matches the format of
		// our surface exactly. This is going to be done in our
		// custom config chooser. See ConfigChooser class definition
		// below.
		setEGLConfigChooser(translucent ?
				new ConfigChooser(8, 8, 8, 8, depth, stencil) :
				new ConfigChooser(5, 6, 5, 0, depth, stencil) );

		// Set the renderer responsible for frame rendering
		m_renderer = new Renderer();
		setRenderer(m_renderer);

/*
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN)
		{
			m_vsyncFrameCallback = new Choreographer.FrameCallback() {
				@Override public void doFrame(long frameTimeNanos) {

//					long dT = frameTimeNanos - prevTime;
//					prevTime = frameTimeNanos;
//
//					Log.i("hataroid", "frame time: " + (dT));

					HataroidViewGL2.instance.requestRender();
					if (m_vsyncEnabled) {
						Choreographer.getInstance().postFrameCallback(this);
					}
				}
			};
		}
*/
	}

//	long prevTime = 0;

	public void enableVSync(boolean enable)
	{
/*
		if (enable == m_vsyncEnabled) {
			return;
		}

		if (m_vsyncFrameCallback != null) {

			m_vsyncEnabled = enable;
			try {
				if (enable) {
					Choreographer.getInstance().postFrameCallback(m_vsyncFrameCallback);
					//HataroidViewGL2.instance.setRenderMode(RENDERMODE_WHEN_DIRTY);
				} else {
					Choreographer.getInstance().removeFrameCallback(m_vsyncFrameCallback);
					//HataroidViewGL2.instance.setRenderMode( RENDERMODE_CONTINUOUSLY);
				}
			} catch (Exception e) {
				e.printStackTrace();
			} catch (Error e) {
				e.printStackTrace();
			}
		} else {
			Log.i("hataroid", "Choreographer unavailable");
		}
*/	}

	private static class ContextFactory implements GLSurfaceView.EGLContextFactory
	{
		private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
		public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig)
		{
			Log.w(TAG, "creating OpenGL ES 2.0 context");
			checkEglError("Before eglCreateContext", egl);
			int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
			EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
			checkEglError("After eglCreateContext", egl);
			return context;
		}

		public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context)
		{
			egl.eglDestroyContext(display, context);
		}
	}

	private static void checkEglError(String prompt, EGL10 egl)
	{
		int error;
		while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS)
		{
			Log.e(TAG, String.format("%s: EGL error: 0x%x", prompt, error));
		}
	}

	private static class ConfigChooser implements GLSurfaceView.EGLConfigChooser
	{
		public ConfigChooser(int r, int g, int b, int a, int depth, int stencil)
		{
			mRedSize = r;
			mGreenSize = g;
			mBlueSize = b;
			mAlphaSize = a;
			mDepthSize = depth;
			mStencilSize = stencil;
		}

		// This EGL config specification is used to specify 2.0 rendering.
		// We use a minimum size of 4 bits for red/green/blue, but will
		// perform actual matching in chooseConfig() below.
		private static int EGL_OPENGL_ES2_BIT = 4;
		private static int[] s_configAttribs2 =
		{
			EGL10.EGL_RED_SIZE, 4,
			EGL10.EGL_GREEN_SIZE, 4,
			EGL10.EGL_BLUE_SIZE, 4,
			EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL10.EGL_NONE
		};

		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display)
		{
			// Get the number of minimally matching EGL configurations
			int[] num_config = new int[1];
			egl.eglChooseConfig(display, s_configAttribs2, null, 0, num_config);
			
			int numConfigs = num_config[0];
			
			if (numConfigs <= 0)
			{
				throw new IllegalArgumentException("No configs match configSpec");
			}

            // Allocate then read the array of minimally matching EGL configs
			EGLConfig[] configs = new EGLConfig[numConfigs];
			egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs, num_config);

			if (DEBUG)
			{
				printConfigs(egl, display, configs);
			}

			//Now return the "best" one
			return chooseConfig(egl, display, configs);
		}

		public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs)
		{
			for(EGLConfig config : configs)
			{
				int d = findConfigAttrib(egl, display, config, EGL10.EGL_DEPTH_SIZE, 0);
				int s = findConfigAttrib(egl, display, config, EGL10.EGL_STENCIL_SIZE, 0);

				// We need at least mDepthSize and mStencilSize bits
				if (d < mDepthSize || s < mStencilSize)
				{
					continue;
				}

				// We want an *exact* match for red/green/blue/alpha
				int r = findConfigAttrib(egl, display, config, EGL10.EGL_RED_SIZE, 0);
				int g = findConfigAttrib(egl, display, config, EGL10.EGL_GREEN_SIZE, 0);
				int b = findConfigAttrib(egl, display, config, EGL10.EGL_BLUE_SIZE, 0);
				int a = findConfigAttrib(egl, display, config, EGL10.EGL_ALPHA_SIZE, 0);

				if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize)
				{
					return config;
				}
			}
			return null;
		}

		private int findConfigAttrib(EGL10 egl, EGLDisplay display, EGLConfig config, int attribute, int defaultValue)
		{
			if (egl.eglGetConfigAttrib(display, config, attribute, mValue))
			{
				return mValue[0];
			}
			return defaultValue;
		}

		private void printConfigs(EGL10 egl, EGLDisplay display, EGLConfig[] configs)
		{
			int numConfigs = configs.length;
			Log.w(TAG, String.format("%d configurations", numConfigs));
			for (int i = 0; i < numConfigs; i++)
			{
				Log.w(TAG, String.format("Configuration %d:\n", i));
				printConfig(egl, display, configs[i]);
			}
		}

		private void printConfig(EGL10 egl, EGLDisplay display, EGLConfig config)
		{
			int[] attributes =
			{
				EGL10.EGL_BUFFER_SIZE,
				EGL10.EGL_ALPHA_SIZE,
				EGL10.EGL_BLUE_SIZE,
				EGL10.EGL_GREEN_SIZE,
				EGL10.EGL_RED_SIZE,
				EGL10.EGL_DEPTH_SIZE,
				EGL10.EGL_STENCIL_SIZE,
				EGL10.EGL_CONFIG_CAVEAT,
				EGL10.EGL_CONFIG_ID,
				EGL10.EGL_LEVEL,
				EGL10.EGL_MAX_PBUFFER_HEIGHT,
				EGL10.EGL_MAX_PBUFFER_PIXELS,
				EGL10.EGL_MAX_PBUFFER_WIDTH,
				EGL10.EGL_NATIVE_RENDERABLE,
				EGL10.EGL_NATIVE_VISUAL_ID,
				EGL10.EGL_NATIVE_VISUAL_TYPE,
				0x3030, // EGL10.EGL_PRESERVED_RESOURCES,
				EGL10.EGL_SAMPLES,
				EGL10.EGL_SAMPLE_BUFFERS,
				EGL10.EGL_SURFACE_TYPE,
				EGL10.EGL_TRANSPARENT_TYPE,
				EGL10.EGL_TRANSPARENT_RED_VALUE,
				EGL10.EGL_TRANSPARENT_GREEN_VALUE,
				EGL10.EGL_TRANSPARENT_BLUE_VALUE,
				0x3039, // EGL10.EGL_BIND_TO_TEXTURE_RGB,
				0x303A, // EGL10.EGL_BIND_TO_TEXTURE_RGBA,
				0x303B, // EGL10.EGL_MIN_SWAP_INTERVAL,
				0x303C, // EGL10.EGL_MAX_SWAP_INTERVAL,
				EGL10.EGL_LUMINANCE_SIZE,
				EGL10.EGL_ALPHA_MASK_SIZE,
				EGL10.EGL_COLOR_BUFFER_TYPE,
				EGL10.EGL_RENDERABLE_TYPE,
				0x3042 // EGL10.EGL_CONFORMANT
			};
			String[] names =
			{
				"EGL_BUFFER_SIZE",
				"EGL_ALPHA_SIZE",
				"EGL_BLUE_SIZE",
				"EGL_GREEN_SIZE",
				"EGL_RED_SIZE",
				"EGL_DEPTH_SIZE",
				"EGL_STENCIL_SIZE",
				"EGL_CONFIG_CAVEAT",
				"EGL_CONFIG_ID",
				"EGL_LEVEL",
				"EGL_MAX_PBUFFER_HEIGHT",
				"EGL_MAX_PBUFFER_PIXELS",
				"EGL_MAX_PBUFFER_WIDTH",
				"EGL_NATIVE_RENDERABLE",
				"EGL_NATIVE_VISUAL_ID",
				"EGL_NATIVE_VISUAL_TYPE",
				"EGL_PRESERVED_RESOURCES",
				"EGL_SAMPLES",
				"EGL_SAMPLE_BUFFERS",
				"EGL_SURFACE_TYPE",
				"EGL_TRANSPARENT_TYPE",
				"EGL_TRANSPARENT_RED_VALUE",
				"EGL_TRANSPARENT_GREEN_VALUE",
				"EGL_TRANSPARENT_BLUE_VALUE",
				"EGL_BIND_TO_TEXTURE_RGB",
				"EGL_BIND_TO_TEXTURE_RGBA",
				"EGL_MIN_SWAP_INTERVAL",
				"EGL_MAX_SWAP_INTERVAL",
				"EGL_LUMINANCE_SIZE",
				"EGL_ALPHA_MASK_SIZE",
				"EGL_COLOR_BUFFER_TYPE",
				"EGL_RENDERABLE_TYPE",
				"EGL_CONFORMANT"
			};
			int[] value = new int[1];
			for (int i = 0; i < attributes.length; i++)
			{
				int attribute = attributes[i];
				String name = names[i];
				if ( egl.eglGetConfigAttrib(display, config, attribute, value))
				{
					Log.w(TAG, String.format("  %s: %d\n", name, value[0]));
				}
				else
				{
					// Log.w(TAG, String.format("  %s: failed\n", name));
					while (egl.eglGetError() != EGL10.EGL_SUCCESS);
				}
			}
		}

		// Subclasses can adjust these values:
		protected int mRedSize;
		protected int mGreenSize;
		protected int mBlueSize;
		protected int mAlphaSize;
		protected int mDepthSize;
		protected int mStencilSize;
		private int[] mValue = new int[1];
	}

	private static class Renderer implements GLSurfaceView.Renderer
	{
		public void onDrawFrame(GL10 gl)
		{
			HataroidViewGL2 sview = HataroidViewGL2.instance;

			HataroidActivity ha = HataroidActivity.instance;
			Input input = ha.getInput();
			
			float mouseX = 0, mouseY = 0;
			int mouseBtns = sview.m_mouseButtons;
			if (sview.m_tryMouse && !sview.m_newMouseDisabled)
			{
				try
				{
					InputMouse inputMouse = input.getInputMouse();
					if (inputMouse != null)
					{
						if (mouseBtns == 0)
						{
							mouseX = inputMouse.getMouseX();
							mouseY = inputMouse.getMouseY();
						}
						else
						{
							mouseX = sview.m_mouseX;
							mouseY = sview.m_mouseY;
						}
					}
				}
				catch (Error e)
				{
					sview.m_tryMouse = false;
				}
				catch (Exception e)
				{
					sview.m_tryMouse = false;
				}
			}
			
			sview.checkNewMouseDisabling();

			int [] keyPresses = null;
			BitFlags keyPressFlags = input.getKeyPresses();
			boolean hasDirectPresses = input.hasDirectPresses();
			if (hasDirectPresses)
			{
				BitFlags directPressFlags = input.getDirectPresses();
				directPressFlags.orAll(keyPressFlags); // Note: modifies direct press flags so I don't have to alloc new one

				keyPresses = directPressFlags._flags;

				//String s = "direct keys:";
				//for (int i = 0; i < directPressFlags._flags.length; ++i)
				//{
				//	s += " " + String.valueOf(directPressFlags._flags[i]);
				//}
				//Log.i("hataroid", s);
			}
			else
			{
				keyPresses = keyPressFlags._flags;
			}

			float[] curAxis = input.getCurAxis();

			HataroidNativeLib.updateInput(
					sview.m_touched[0], sview.m_touchX[0], sview.m_touchY[0],
					sview.m_touched[1], sview.m_touchX[1], sview.m_touchY[1],
					sview.m_touched[2], sview.m_touchX[2], sview.m_touchY[2],
					mouseX, mouseY, mouseBtns,
					keyPresses, curAxis);

			if (hasDirectPresses)
			{
				input.clearDirectPresses();
			}

			boolean forceQuit = HataroidNativeLib.onDrawFrame();
			if (forceQuit)
			{
				ha.quitHataroid();
			}
		}

		public void onSurfaceChanged(GL10 gl, int width, int height)
		{
			HataroidNativeLib.onSurfaceChanged(width, height);
			
			HataroidActivity.instance.startEmulationThread();
		}

		public void onSurfaceCreated(GL10 gl, EGLConfig config)
		{
			HataroidNativeLib.onSurfaceCreated();
		}
	}
}
