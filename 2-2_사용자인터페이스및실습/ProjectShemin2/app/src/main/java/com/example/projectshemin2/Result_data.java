package com.example.projectshemin2;

import java.io.Serializable;

public class Result_data implements Serializable {
    private String gu = null;
    private String dong = null;
    private String garbage = null;

    public String getGu(){
        return gu;
    }
    public String getDong() {
        return dong;
    }
    public String getGarbage() {
        return garbage;
    }
    public void setGu(String guData){
        gu = guData;
    }
    public void setDong(String dongData){
        dong = dongData;
    }
    public void setGarbage(String garbage) {
        this.garbage = garbage;
    }
}
