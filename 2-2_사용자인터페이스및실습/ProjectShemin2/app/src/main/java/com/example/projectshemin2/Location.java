package com.example.projectshemin2;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.ComponentName;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.firestore.CollectionReference;
import com.google.firebase.firestore.FirebaseFirestore;
import com.google.firebase.firestore.QueryDocumentSnapshot;
import com.google.firebase.firestore.QuerySnapshot;

import java.util.ArrayList;

public class Location extends AppCompatActivity {

    private ArrayList<String> array_gu;
    private ListView mListView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_location);

        final FirebaseFirestore db = FirebaseFirestore.getInstance();

        final ArrayAdapter adapter = new ArrayAdapter(this, android.R.layout.simple_list_item_1);

        mListView = (ListView) findViewById(R.id.list_gu);
        mListView.setAdapter(adapter);

        db.collection("gu").get().addOnCompleteListener(new OnCompleteListener<QuerySnapshot>() {
            CollectionReference gu = db.collection("gu");
            @Override
            public void onComplete(@NonNull Task<QuerySnapshot> task) {
                if(task.isSuccessful()) {
                    //array_gu = new ArrayList<String>();
                    for(QueryDocumentSnapshot document : task.getResult()) {
                        adapter.add(document.getId());
                    }
                }
            }
        });

        mListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            String strText;
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                strText = (String)parent.getItemAtPosition(position);

                Intent intent = new Intent();

                ComponentName componentName = new ComponentName(
                        "com.example.projectshemin2",
                        "com.example.projectshemin2.Location_dong"
                );

                intent.setComponent(componentName);

                intent.putExtra("select_gu", strText);

                startActivity(intent);
            }
        });
    }

}//https://velog.io/@dooreplay/firebase-sub-collection