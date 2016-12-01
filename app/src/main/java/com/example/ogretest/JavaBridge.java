package com.example.ogretest;

import android.content.res.AssetManager;
import android.view.Surface;

public class JavaBridge {

    public static native void InitNative(AssetManager assetMgr);

    public static native void OnSurfaceCreated(Surface surface);

    public static native void OnTouchEvent(int source, int action, float x, float y, float rawX, float rawY);
}
