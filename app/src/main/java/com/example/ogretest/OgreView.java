package com.example.ogretest;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class OgreView extends SurfaceView implements SurfaceHolder.Callback {

    @Override
    protected void onDraw(Canvas canvas){
        canvas.drawColor(Color.GRAY);
    }

    public OgreView(Context context, AttributeSet attrs) {
        super(context, attrs);
        getHolder().addCallback(this);
        //getHolder().setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    }

    public OgreView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        getHolder().addCallback(this);
        //getHolder().setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    }

    @Override
    public boolean onTouchEvent(MotionEvent evt){
        return super.onTouchEvent(evt);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder){

        Canvas canvas = holder.lockCanvas();
        onDraw(canvas);
        holder.unlockCanvasAndPost(canvas);
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height){
        int i = 0;
    }

    @Override
    public  void surfaceDestroyed(SurfaceHolder holder){

    }
}
