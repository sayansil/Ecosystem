package ss.rmg.ecosystem;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.net.ConnectivityManager;
import android.os.Build;
import android.os.Environment;
import android.os.VibrationEffect;
import android.os.Vibrator;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.inputmethod.InputMethodManager;

import androidx.appcompat.app.AlertDialog;
import androidx.constraintlayout.widget.ConstraintLayout;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Random;

import static android.content.Context.INPUT_METHOD_SERVICE;

public class BaseUtility {

    private static final String TAG = "BaseUtility";

    public static boolean isNetworkConnected(Context context) {
        ConnectivityManager cm = (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        return cm.getActiveNetworkInfo() != null;
    }

    public static void hideKeyboard(Context context, ConstraintLayout layout) {
        InputMethodManager imm = (InputMethodManager)context.getSystemService(INPUT_METHOD_SERVICE);
        imm.hideSoftInputFromWindow(layout.getWindowToken(), 0);
    }

    public static void vibrate(Context context) {
        Vibrator v = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
        v.vibrate(VibrationEffect.createOneShot(50, VibrationEffect.DEFAULT_AMPLITUDE));

    }

    public static void show_popup(int resource, Context context) {
        LayoutInflater li = LayoutInflater.from(context);
        View promptsView = li.inflate(resource, null);
        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(context);
        alertDialogBuilder.setView(promptsView);

        alertDialogBuilder.setCancelable(true);

        AlertDialog alertDialog = alertDialogBuilder.create();

        alertDialog.show();
    }

    public static String safeURL(String raw_url) {
        String url = raw_url;
        if (!raw_url.startsWith("http"))
            url = "http://" + raw_url;

        return url;
    }

    public static int getRandomColor() {
        Random rand = new Random();
        int r = rand.nextInt(255);
        int g = rand.nextInt(255);
        int b = rand.nextInt(255);

        return Color.rgb(r,g,b);
    }

    public static String saveToFile(String data, String directory, String prefix) {
        String file_path = Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + directory;
        File dir = new File(file_path);
        if(!dir.exists())
            dir.mkdirs();

        String timeStamp = new SimpleDateFormat(
                "yyyy-MM-dd HH.mm.ss",
                Locale.getDefault()).format(new Date());

        String complete_filename = prefix + "_" + timeStamp + ".txt";

        File file = new File(dir, complete_filename);

        try {
            FileOutputStream stream = new FileOutputStream(file);
            stream.write(data.getBytes());
            stream.close();
        } catch (Exception e){
            complete_filename = "";
        }

        return complete_filename;
    }

    public static String readFromFile(String directory, String filename) {
        String data;

        String file_path = Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + directory;
        File dir = new File(file_path);
        if(!dir.exists())
            dir.mkdirs();

        File file = new File(dir, filename);

        int length = (int) file.length();
        byte[] bytes = new byte[length];

        try {
            FileInputStream in = new FileInputStream(file);
            in.read(bytes);
            in.close();
            data = new String(bytes);
        } catch (Exception e){
            data = "";
        }

        return data;
    }
}
