package com.example.ogretest;

import android.content.res.AssetManager;
import android.view.Surface;

public class JavaBridge {

    public static native void InitNative(AssetManager assetMgr);

    public static native void OnSurfaceCreated(Surface surface);
}
