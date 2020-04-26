package com.bn.bullet;

import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;

public class JNIPort 
{
    static 
    {
        System.loadLibrary("BNJNI");
    }
	  
    public static native void step();
    public static native void onSurfaceChanged(int width,int height);
    public static native void onSurfaceCreated(GLSurfaceView gsv);
    public static native void setCamera(float dx,float dy);
    public static native void nativeSetAssetManager(AssetManager am);
}
