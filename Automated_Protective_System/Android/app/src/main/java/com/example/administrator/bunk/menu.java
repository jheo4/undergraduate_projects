package com.example.administrator.bunk;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

public class menu extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu);
    }
    public void onoff(View v){  ////////to go to onoff menu
        Intent MenuToOnoff = new Intent(getApplicationContext(), onoff.class);
        startActivity(MenuToOnoff);
    }

    public void log(View v){  ///////////// to go to log menu
        Intent MenuToLog = new Intent(getApplicationContext(), log.class);
        startActivity(MenuToLog);
    }

    public void backbutton1(View v){  ////////////////back button
       Intent MenuToIntro = new Intent(getApplicationContext(), MainActivity.class);
        startActivity(MenuToIntro);
       // finish();
    }
}
