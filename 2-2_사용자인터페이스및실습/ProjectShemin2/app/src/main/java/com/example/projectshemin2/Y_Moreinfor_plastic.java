package com.example.projectshemin2;

import androidx.appcompat.app.AppCompatActivity;

import android.content.ComponentName;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class Y_Moreinfor_plastic extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_y__moreinfor_plastic);
        Button startbutton = (Button)findViewById(R.id.startButton);

        startbutton.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View view) {
                goToStart();
            }
        }) ;
    }

    private void goToStart(){
        Intent intent = new Intent();
        ComponentName componentName = new ComponentName(
                "com.example.projectshemin2",
                "com.example.projectshemin2.MainActivity"
        );
        intent.setComponent(componentName);

        startActivity(intent);
    }
}