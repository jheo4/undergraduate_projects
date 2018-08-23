package com.example.administrator.bunk;

import android.content.Context;
import android.net.Uri;
import android.os.Handler;
import android.os.Vibrator;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.common.api.GoogleApiClient;

import java.io.ObjectOutputStream;
import java.net.Socket;



public class onoff extends AppCompatActivity {
    Handler handler = new Handler();
    TextView textview;
    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    private GoogleApiClient client;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_onoff);
        textview = (TextView) findViewById(R.id.textView);

    }

    public void OpenClose(View v) {
        Toast.makeText(this, "button", Toast.LENGTH_SHORT).show();
        ConnectThreadOn thread = new ConnectThreadOn();
        thread.start();
        Vibrator vib = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
        vib.vibrate(10000);
    }

    public void backbutton2(View v) {

        finish();
    }


    class ConnectThreadOn extends Thread {

        public void run() {
            String host = "192.168.1.17"; // ip address
            int port = 10000;  ///port num
            try {
                Socket socket = new Socket(host, port);
                println("Connected : " + host + "," + port);
                int output = 111;
                ObjectOutputStream outputStream = new ObjectOutputStream(socket.getOutputStream());
                outputStream.writeInt(output);
                  outputStream.flush();
                outputStream.close();
                socket.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        private void println(final String data) {
            handler.post(new Runnable() {
                @Override
                public void run() {
                    textview.append(data + "\n");
                }
            });
        }
    }



}
