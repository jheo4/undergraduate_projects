package com.example.administrator.bunk;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.webkit.WebView;

public class log extends AppCompatActivity {

    private WebView webView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_log);
        webView = (WebView)findViewById(R.id.webView); /////////////////to show thw webview
        webView.loadUrl("http://192.168.1.21:8080/owhy/mongo_find.jsp");
    }

    public void backbutton3(View v) { // back menu
        finish();
    }
}
