package com.example.projectshemin2;

import java.io.Serializable;

public class Location_data implements Serializable {
    private String gu = null;
    private String dong = null;

    public String getGu(){
        return gu;
    }
    public String getDong() {
        return dong;
    }
    public void setGu(String guData){
        gu = guData;
    }
    public void setDong(String dongData){
        dong = dongData;
    }
}
