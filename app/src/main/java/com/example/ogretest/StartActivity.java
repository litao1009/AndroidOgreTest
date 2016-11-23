package com.example.ogretest;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;


public class StartActivity extends AppCompatActivity{

    private Button BtnStartOgre_;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.start_activity_layout);

        BtnStartOgre_ = (Button)findViewById(R.id.btn_start_ogre);
        BtnStartOgre_.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent();
                intent.setClass(StartActivity.this, OgreActivity.class);
                startActivity(intent);
            }
        });
    }
}
