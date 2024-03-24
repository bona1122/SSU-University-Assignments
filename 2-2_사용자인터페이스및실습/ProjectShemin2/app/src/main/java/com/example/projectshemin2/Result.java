package com.example.projectshemin2;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.ComponentName;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.firestore.CollectionReference;
import com.google.firebase.firestore.DocumentReference;
import com.google.firebase.firestore.DocumentSnapshot;
import com.google.firebase.firestore.FirebaseFirestore;
import com.google.firebase.firestore.QueryDocumentSnapshot;
import com.google.firebase.firestore.QuerySnapshot;

import org.w3c.dom.Text;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;

public class Result extends AppCompatActivity {

    TextView dongText;
    TextView dateTextView;
    TextView timeTextView;
    TextView garbageNameTextView;
    TextView garageCategoryTextView;
    ImageView resultImage;
    TextView resultTextView;
    Button moreButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_result);

        dongText = (TextView) findViewById(R.id.dong);
        dateTextView = (TextView) findViewById(R.id.day);
        timeTextView = (TextView) findViewById(R.id.time);
        garbageNameTextView = (TextView)findViewById(R.id.garbageName);
        garageCategoryTextView = (TextView)findViewById(R.id.garbageCategory);
        resultImage = (ImageView)findViewById(R.id.resultImage);
        resultTextView = (TextView)findViewById(R.id.result);
        moreButton = (Button)findViewById(R.id.moreButton);

        final String week = String.valueOf(doDayOfWeek());
        //resultTextView.setText(week);

        Intent intent = getIntent();
        //intent.getSerializableExtra("Result_Data");

        final Result_data result_data = (Result_data)intent.getSerializableExtra("Result_Data");
        if(result_data==null){
            return;
        }
        dongText.setText(result_data.getDong());
        garbageNameTextView.setText(" - ".concat(result_data.getGarbage()));

        final String guText;
        guText=result_data.getGu();

        final String dongText2;
        dongText2=result_data.getDong();

        String trash;
        trash = result_data.getGarbage();
        //garbageAllTextView.setText(result_data.getGarbage());

        final FirebaseFirestore db = FirebaseFirestore.getInstance();

        DocumentReference docRef = db.collection("gu").document(guText).collection("dong").document(dongText2);
        docRef.get().addOnCompleteListener(new OnCompleteListener<DocumentSnapshot>() {
            @Override
            public void onComplete(@NonNull Task<DocumentSnapshot> task) {
                final String dateText;
                final String timeText;
                if (task.isSuccessful()) {
                    DocumentSnapshot document = task.getResult();
                    if (document.exists()) {
                        dateText = (String) document.getString("date");
                        dateTextView.setText(dateText);

                        timeText  = (String) document.getString("time");
                        timeTextView.setText(timeText);

                        String[] weekArray = dateText.split(", ");
                        boolean isPossible = false;
                        for(int i=0; i<weekArray.length; i++) {
                            if(weekArray[i].equals(week)) {
                                isPossible = true; break;
                            }
                        }
                        if(isPossible) {
                            String time1 = timeText.substring(0,2);
                            int time1_1 = Integer.parseInt(time1);
                            String time2 = timeText.substring(6,8);
                            int time2_1 = Integer.parseInt(time2);

                            long now = System.currentTimeMillis();
                            Date mDate = new Date(now);

                            SimpleDateFormat simpleDate = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
                            String getTime = simpleDate.format(mDate);

                            String currentTime = getTime.substring(11,13);
                            int currentTime_1 = Integer.parseInt(currentTime);

                            if(currentTime_1>=time1_1 && currentTime_1<time2_1) {
                                resultTextView.setText("지금 배출 가능!");
                            }
                            else {
                                resultTextView.setText("지금 배출 불가능!");
                            }
                        }
                        else {
                            resultTextView.setText("지금 배출 불가능!");
                        }
                    }
                }
            }
        });

        DocumentReference docRef2 = db.collection("garbage").document(trash);
        docRef2.get().addOnCompleteListener(new OnCompleteListener<DocumentSnapshot>() {
            @Override
            public void onComplete(@NonNull Task<DocumentSnapshot> task) {
                final String categoryText;
                if (task.isSuccessful()) {
                    DocumentSnapshot document = task.getResult();
                    if (document.exists()) {
                        categoryText = (String) document.getString("category");
                        garageCategoryTextView.setText(categoryText);

                        moreButton.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                goToMoreInfo(categoryText,result_data);
                            };
                        });

                        if(categoryText.equals("종이류")){
                            resultImage.setImageResource(R.drawable.paper);
                        }
                        else if(categoryText.equals("비닐류")){
                            resultImage.setImageResource(R.drawable.binil);
                        }
                        else if(categoryText.equals("고철")){
                            resultImage.setImageResource(R.drawable.gochul);
                        }
                        else if(categoryText.equals("종이팩")){
                            resultImage.setImageResource(R.drawable.paperpack);
                        }
                        else if(categoryText.equals("유리병류")){
                            resultImage.setImageResource(R.drawable.bottle);
                        }
                        else if(categoryText.equals("스티로폼")){
                            resultImage.setImageResource(R.drawable.stiropom);
                        }
                        else if(categoryText.equals("플라스틱 용기류")){
                            resultImage.setImageResource(R.drawable.pet);
                        }
                        else{   //금속캔
                            resultImage.setImageResource(R.drawable.can);
                        }
                    }
                }
            }
        });
    }
    private String doDayOfWeek() {
        Calendar cal = Calendar.getInstance();
        String strWeek = null;

        int nWeek = cal.get(Calendar.DAY_OF_WEEK);
        if (nWeek == 1) {
            strWeek = "일";
        } else if (nWeek == 2) {
            strWeek = "월";
        } else if (nWeek == 3) {
            strWeek = "화";
        } else if (nWeek == 4) {
            strWeek = "수";
        } else if (nWeek == 5) {
            strWeek = "목";
        } else if (nWeek == 6) {
            strWeek = "금";
        } else if (nWeek == 7) {
            strWeek = "토";
        }
        return strWeek;
    }

    private void goToMoreInfo(String categoryText,Result_data result_data){
            Intent intent = new Intent();
            String text = new String();

            if(categoryText.equals("종이류")){
                text = "paper";
            }
            else if(categoryText.equals("비닐류")){
                text = "vinyl";
            }
            else if(categoryText.equals("고철")){
                text = "ironcan";
            }
            else if(categoryText.equals("종이팩")){
                text = "paperpack";
            }
            else if(categoryText.equals("유리병류")){
                text = "glass";
            }
            else if(categoryText.equals("스티로폼")){
                text = "sti";
            }
            else if(categoryText.equals("플라스틱 용기류")){
                text="plastic";
            }
            else if(categoryText.equals("페트류")){
                text = "pet";
            }
            else{   //금속캔
                text = "ironcan";
            }

            ComponentName componentName = new ComponentName(
                    "com.example.projectshemin2",
                    "com.example.projectshemin2.Y_Moreinfor_"+ text
            );
            intent.setComponent(componentName);
            intent.putExtra("Result_Data",result_data);

            startActivity(intent);
    }
}
