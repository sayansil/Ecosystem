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
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;

public class SplashActivity extends AppCompatActivity {

    LottieAnimationView animation;
    ConstraintLayout footer;
    TextView errorText;

    final int startDelay = 3500;
    final int delay = 2000;

    Handler handler = new Handler();
    Runnable runnable;

    boolean flag;

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
        errorText = findViewById(R.id.errorText);

        flag = false;

        queue = Volley.newRequestQueue(this);
    }

    public void animateIn(final View v) {
        if(v.getVisibility()!=View.VISIBLE) {
            ObjectAnimator fadeIn = ObjectAnimator.ofFloat(v, "alpha", 0f, 1f);
            fadeIn.setDuration(startDelay);

            final AnimatorSet mAnimationSet = new AnimatorSet();

            mAnimationSet.play(fadeIn);
            mAnimationSet.addListener(new AnimatorListenerAdapter() {
                @Override
                public void onAnimationEnd(Animator animation) {
                    super.onAnimationEnd(animation);
                    v.setVisibility(View.VISIBLE);
                }
            });
            mAnimationSet.start();
        }
    }

    @Override
    protected void onStart() {
        handler.postDelayed( runnable = () -> {
            if(BaseUtility.isNetworkConnected(getApplicationContext())) {
                if(!flag) {
                    fetchDataAndForward();
                }
            } else {
                errorText.setText(getString(R.string.error_network));
                animateIn(errorText);
            }
            handler.postDelayed(runnable, delay);
        }, startDelay);

        super.onStart();
    }

    @Override
    protected void onPause() {
        handler.removeCallbacks(runnable);
        super.onPause();
    }

    private void fetchDataAndForward() {
        flag = true;
        String url_menu = getString(R.string.url_base) + "/" + getString(R.string.endpoint_menu);
        JsonObjectRequest getRequest = new JsonObjectRequest(Request.Method.GET, url_menu, null,
                response -> {
                    Intent intent = new Intent(SplashActivity.this, MainActivity.class);
                    intent.putExtra("menu", response.toString());
                    startActivity(intent);
                    finish();
                },
                error -> {
                    errorText.setText(getString(R.string.error_server));
                    animateIn(errorText);
                    flag = false;
                }
        );
        queue.add(getRequest);
    }
}
