package ss.rmg.ecosystem;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.animation.AnimatorSet;
import android.animation.ObjectAnimator;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;

import com.airbnb.lottie.LottieAnimationView;
import com.android.volley.DefaultRetryPolicy;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;

public class SplashActivity extends AppCompatActivity {

    LottieAnimationView animation;
    ConstraintLayout footer;

    final int delay = 3000;

    Handler handler = new Handler();
    Runnable runnable;

    RequestQueue queue;

    private static final String TAG = "SplashActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_splash);

        animation = findViewById(R.id.centreAnimation);
        footer = findViewById(R.id.footer);

        queue = Volley.newRequestQueue(this);
    }

    @Override
    protected void onStart() {
        runnable = () -> {
            if(BaseUtility.isNetworkConnected(getApplicationContext())) {
                fetchDataAndForward();
            } else {
                forward("");
            }
        };
        handler.postDelayed(runnable, delay);

        super.onStart();
    }

    @Override
    protected void onPause() {
        handler.removeCallbacks(runnable);
        super.onPause();
    }

    private void fetchDataAndForward() {
        String url_menu = getString(R.string.url_base) + "/" + getString(R.string.endpoint_menu);
        JsonObjectRequest getRequest = new JsonObjectRequest(Request.Method.GET, url_menu, null,
                response -> forward(response.toString()),
                error -> forward("")
        );
        getRequest.setRetryPolicy(new DefaultRetryPolicy(
                30000, // 30 seconds timeout
                DefaultRetryPolicy.DEFAULT_MAX_RETRIES,
                DefaultRetryPolicy.DEFAULT_BACKOFF_MULT));
        queue.add(getRequest);
    }

    private void forward(String menu) {
        Intent intent = new Intent(SplashActivity.this, MainActivity.class);
        intent.putExtra("menu", menu);
        startActivity(intent);
        finish();
    }
}
