package senpai.rmg.ecosystem;

import android.content.Context;

import java.io.File;

public class BaseUtils {

    public void setupFilesystem(Context context) {
        File path=new File(context.getFilesDir(),"myfolder");
    }

    public void saveFile(String filepath, String value) {

    }

    public String readFile(String filepath) {
        String value = "";

        return value;
    }
}
