package com.example.ogretest;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;


public class StartActivity extends AppCompatActivity{

    static {
        System.loadLibrary("NativeMain");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.start_activity_layout);

        RenderEngine.InitNative(getAssets());
    }
}
