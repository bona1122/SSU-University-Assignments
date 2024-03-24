package com.example.projectshemin2;

import android.content.ComponentName;
import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.example.projectshemin2.R;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.firestore.CollectionReference;
import com.google.firebase.firestore.FirebaseFirestore;
import com.google.firebase.firestore.QueryDocumentSnapshot;
import com.google.firebase.firestore.QuerySnapshot;

import org.w3c.dom.Text;

import java.util.ArrayList;
import java.util.List;

import javax.xml.datatype.Duration;

public class MainActivity extends AppCompatActivity {

    private List<String> list; //데이터를 넣을 리스트변수
    TextView select_location;
    Result_data result_data = new Result_data();
    //Location_data location_data = null;
    //AutoCompleteTextView autoCompleteTextView = (AutoCompleteTextView) findViewById(R.id.autoCompleteTextView);

    //추가시도
    AutoCompleteTextView autoCompleteTextView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //final AutoCompleteTextView autoCompleteTextView = (AutoCompleteTextView) findViewById(R.id.autoCompleteTextView);
        autoCompleteTextView = (AutoCompleteTextView)findViewById(R.id.autoCompleteTextView);
        Intent intent = getIntent();

        //객체 인텐트 시도
        final Location_data location_data = (Location_data) intent.getSerializableExtra("location");

        //Result_data result_data = new Result_data();
        //String gu = (String)location_data.getGu();
        //result_data.setGu(gu);
        //result_data.setDong(location_data.getDong());
        //result_data.setGarbage(autoCompleteTextView.getText());

        //list = new ArrayList<String>(); //리스트를 생성
        final FirebaseFirestore db = FirebaseFirestore.getInstance();


        final ArrayAdapter adapter = new ArrayAdapter(this, android.R.layout.simple_list_item_1);
        autoCompleteTextView.setAdapter(adapter);

        db.collection("garbage").get().addOnCompleteListener(new OnCompleteListener<QuerySnapshot>() {
            CollectionReference garbage = db.collection("garbage");

            @Override
            public void onComplete(@NonNull Task<QuerySnapshot> task) {
                if (task.isSuccessful()) {
                    //array_gu = new ArrayList<String>();
                    for (QueryDocumentSnapshot document : task.getResult()) {
                        adapter.add(document.getId());
                    }
                }
            }
        });

        select_location = findViewById(R.id.select_location);
        //객체 인텐트
        if (location_data == null) {
            return;
        }
        select_location.setText(location_data.getGu() + " " + location_data.getDong());
        result_data.setGu(location_data.getGu());
        result_data.setDong(location_data.getDong());

        autoCompleteTextView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                if(location_data != null){
                    result_data.setGarbage(((TextView)view).getText().toString());
                }
            }
        });
    }
    public void resultButton(View view) {
        if(result_data.getDong() == null){
            Toast.makeText(this,"위치를 설정해주세요." ,Toast.LENGTH_LONG).show();
        }
        else if(result_data.getGarbage() ==null){
            Toast.makeText(this,"버릴 것을 설정해주세요." ,Toast.LENGTH_LONG).show();
        }
        else{
            Intent intent = new Intent();

            ComponentName componentName = new ComponentName(
                    "com.example.projectshemin2",
                    "com.example.projectshemin2.Result"
            );
            intent.setComponent(componentName);
            intent.putExtra("Result_Data",result_data);
            //result_data.setGarbage(autoCompleteTextView.getText().getString());

            startActivity(intent);
        }
        /*Intent intent = new Intent();

        ComponentName componentName = new ComponentName(
                "com.example.projectshemin2",
                "com.example.projectshemin2.Result"
        );
        intent.setComponent(componentName);
        intent.putExtra("Result_Data",result_data);
        //result_data.setGarbage(autoCompleteTextView.getText().getString());

        startActivity(intent);*/
    }

    public void onClickLocationButton(View view) {
        Intent intent = new Intent();

        ComponentName componentName = new ComponentName(
                "com.example.projectshemin2",
                "com.example.projectshemin2.Location"
        );
        intent.setComponent(componentName);
        startActivity(intent);
    }
}