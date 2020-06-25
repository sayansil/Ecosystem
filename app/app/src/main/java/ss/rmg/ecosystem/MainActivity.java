package ss.rmg.ecosystem;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.inputmethod.EditorInfo;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.DefaultRetryPolicy;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.rengwuxian.materialedittext.MaterialEditText;
import com.weiwangcn.betterspinner.library.material.MaterialBetterSpinner;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;

import vn.luongvo.widget.iosswitchview.SwitchView;

public class MainActivity extends AppCompatActivity {

    List<String> kingdom;
    List<String> plant;
    List<String> animal;

    String API_KEY;
    String default_localIP;

    ImageButton apiBtn;

    MaterialBetterSpinner spinnerK;
    MaterialBetterSpinner spinnerS;

    MaterialEditText countText;
    MaterialEditText yearsText;
    MaterialEditText ipText;

    SwitchView switchLocal;

    ProgressBar progressBar;

    String text_kingdom;
    String text_species;
    boolean isLocal;

    Button simBtn;

    RequestQueue queue;

    private static final String TAG = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_main);

        progressBar = findViewById(R.id.progressBar);
        progressBar.setVisibility(View.GONE);

        spinnerK = findViewById(R.id.spinnerKingdom);
        spinnerS = findViewById(R.id.spinnerSpecies);

        countText = findViewById(R.id.textCount);
        yearsText = findViewById(R.id.textYears);
        ipText = findViewById(R.id.textIP);

        apiBtn = findViewById(R.id.changeAPIbtn);

        switchLocal = findViewById(R.id.switchLocal);

        simBtn = findViewById(R.id.simBtn);
        submitEnable(true);

        isLocal = false;

        queue = Volley.newRequestQueue(this);

        kingdom = new ArrayList<>();
        kingdom.add("plant");
        kingdom.add("animal");

        plant = new ArrayList<>();
        animal = new ArrayList<>();

        text_species = "";
        text_kingdom = "";

        SharedPreferences settings = PreferenceManager.getDefaultSharedPreferences(this);
        API_KEY = settings.getString("api-key", getString(R.string.api_key));
        default_localIP = settings.getString("def-ip", "");
        ipText.setText(default_localIP);

        apiBtn.setOnClickListener(view -> {
            BaseUtility.vibrate(this);
            LayoutInflater li = LayoutInflater.from(this);
            View promptsView = li.inflate(R.layout.dialog_updateapi, null);
            AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(this);
            alertDialogBuilder.setView(promptsView);

            final MaterialEditText newAPItxt = promptsView.findViewById(R.id.newApi);

            alertDialogBuilder
                    .setCancelable(true)
                    .setPositiveButton("OK",
                            (DialogInterface dialog, int d_id) -> {
                                API_KEY = newAPItxt.getText().toString().trim();
                                SharedPreferences.Editor editor = settings.edit();
                                editor.putString("api-key", newAPItxt.getText().toString().trim());
                                editor.apply();
                            })
                    .setNegativeButton("Cancel",
                            (DialogInterface dialog,int d_id) -> {
                                dialog.cancel();
                            });
            AlertDialog alertDialog = alertDialogBuilder.create();
            alertDialog.show();
        });

        Intent intent = getIntent();
        try {
            String tmp = intent.getStringExtra("menu");

            if (!tmp.isEmpty()) {
                JSONObject menu;
                JSONArray array;
                menu = new JSONObject(tmp);

                array = menu.getJSONArray("plant");
                for(int i = 0 ; i < array.length() ; i++) {
                    plant.add(array.getString(i));
                }

                array = menu.getJSONArray("animal");
                for(int i = 0 ; i < array.length() ; i++) {
                    animal.add(array.getString(i));
                }
            } else {
                isLocal = true;
                switchLocal.setChecked(true);
                ipText.setVisibility(View.VISIBLE);
                BaseUtility.show_popup(R.layout.dialog_nonet, this);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }

        ArrayAdapter<String> adapter = new ArrayAdapter<>(this,
                android.R.layout.simple_dropdown_item_1line, kingdom);
        spinnerK.setAdapter(adapter);

        List<String> tmp = new ArrayList<>();
        adapter = new ArrayAdapter<>(this,
                android.R.layout.simple_dropdown_item_1line, tmp);
        spinnerS.setAdapter(adapter);

        spinnerK.setOnItemClickListener((parent, view, position, id) -> {
            text_kingdom = kingdom.get(position);
            if (text_kingdom.equalsIgnoreCase("plant")) {
                ArrayAdapter<String> newAdpater = new ArrayAdapter<>(this,
                        android.R.layout.simple_dropdown_item_1line, plant);
                spinnerS.setAdapter(newAdpater);
            } else if (text_kingdom.equalsIgnoreCase("animal")) {
                ArrayAdapter<String> newAdpater = new ArrayAdapter<>(this,
                        android.R.layout.simple_dropdown_item_1line, animal);
                spinnerS.setAdapter(newAdpater);
            }
        });

        spinnerS.setOnItemClickListener((parent, view, position, id) -> {
            if (text_kingdom.equalsIgnoreCase("plant")) {
                text_species = plant.get(position);
            } else if (text_kingdom.equalsIgnoreCase("animal")) {
                text_species = animal.get(position);
            }
        });

        simBtn.setOnClickListener(view -> {
            BaseUtility.vibrate(this);
            progressBar.setVisibility(View.VISIBLE);
            submitEnable(false);
            requestStoragePermissions();

            String available_url = getBaseUrl() + "/" + getString(R.string.endpoint_schema);
            JsonObjectRequest getRequest = new JsonObjectRequest(Request.Method.GET, available_url, null,
                    response -> {
                        String schema = response.toString();
                        if (!schema.isEmpty()) {
                            String query_url = getBaseUrl() + "/" + getString(R.string.endpoint_query);
                            query_url += "?initial_count=" + countText.getText().toString() +
                                    "&years=" + yearsText.getText().toString() +
                                    "&kingdom=" + text_kingdom +
                                    "&species=" + text_species +
                                    "&api-key=" + API_KEY;

                            JsonObjectRequest newRequest = new JsonObjectRequest(Request.Method.GET, query_url, null,
                                    newResponse -> {
                                        progressBar.setVisibility(View.GONE);
                                        submitEnable(true);
                                        try {
                                            String status = newResponse.getString("status");
                                            switch (status) {
                                                case "0":
                                                    String data = newResponse.getString("data");

                                                    Intent newIntent = new Intent(MainActivity.this, ReportActivity.class);
                                                    String data_file = BaseUtility.saveToFile(data,
                                                            getString(R.string.directory_parent),
                                                            getString(R.string.file_fetched_data));
                                                    newIntent.putExtra("data_file", data_file);
                                                    newIntent.putExtra("kingdom", text_kingdom);
                                                    newIntent.putExtra("schema", schema);
                                                    startActivity(newIntent);
                                                    break;
                                                case "1":
                                                    BaseUtility.show_popup(R.layout.dialog_invalidapi, this);
                                                    break;
                                                case "2":
                                                    BaseUtility.show_popup(R.layout.dialog_unavailable, this);
                                                    break;
                                            }


                                        } catch (JSONException e) {
                                            e.printStackTrace();
                                            BaseUtility.show_popup(R.layout.dialog_unavailable, this);
                                        }
                                    },
                                    error -> {
                                        progressBar.setVisibility(View.GONE);
                                        submitEnable(true);
                                        error.printStackTrace();
                                        BaseUtility.show_popup(R.layout.dialog_unavailable, this);
                                    }
                            );
                            newRequest.setRetryPolicy(new DefaultRetryPolicy(
                                    900000, // 15 minutes timeout
                                    DefaultRetryPolicy.DEFAULT_MAX_RETRIES,
                                    DefaultRetryPolicy.DEFAULT_BACKOFF_MULT));
                            queue.add(newRequest);
                        } else {
                            progressBar.setVisibility(View.GONE);
                            submitEnable(true);
                            BaseUtility.show_popup(R.layout.dialog_unavailable, this);
                        }
                    },
                    error -> {
                        progressBar.setVisibility(View.GONE);
                        submitEnable(true);
                        error.printStackTrace();
                        BaseUtility.show_popup(R.layout.dialog_unavailable, this);
                    }
            );
            queue.add(getRequest);
        });

        switchLocal.setOnCheckedChangeListener((switchView, b) -> {
            isLocal = b;
            if (isLocal) {
                ipText.setVisibility(View.VISIBLE);
                if(!ipText.getText().toString().isEmpty())
                    refreshSpinners();
            } else {
                ipText.setVisibility(View.INVISIBLE);
                refreshSpinners();
            }
        });

        ipText.setOnEditorActionListener((v, actionId, event) -> {
            if (actionId == EditorInfo.IME_ACTION_SEARCH ||
                    actionId == EditorInfo.IME_ACTION_DONE ||
                    actionId == EditorInfo.IME_ACTION_NEXT ||
                    event != null &&
                            event.getAction() == KeyEvent.ACTION_DOWN &&
                            event.getKeyCode() == KeyEvent.KEYCODE_ENTER) {
                if (event == null || !event.isShiftPressed()) {
                    SharedPreferences.Editor editor = settings.edit();
                    editor.putString("def-ip", ipText.getText().toString().trim());
                    editor.apply();
                    refreshSpinners();
                    return true;
                }
            }
            return false;
        });

        requestStoragePermissions();
    }

    private void requestStoragePermissions() {
        if(checkSelfPermission(android.Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED &&
                checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this,
                    new String[]{android.Manifest.permission.WRITE_EXTERNAL_STORAGE, android.Manifest.permission.READ_EXTERNAL_STORAGE}, 1);
        } else if(checkSelfPermission(android.Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this,
                    new String[]{android.Manifest.permission.WRITE_EXTERNAL_STORAGE}, 1);
        } else if(checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, 1);
        }
    }

    private String getBaseUrl() {
        if (isLocal) {
            return BaseUtility.safeURL(ipText.getText().toString().trim() + ":" + getString(R.string.port_default));
        } else {
            return getString(R.string.url_base);
        }
    }

    private void refreshSpinners() {
        String url_menu = getBaseUrl() + "/" + getString(R.string.endpoint_menu);
        Log.e(TAG, url_menu);
        JsonObjectRequest getRequest = new JsonObjectRequest(Request.Method.GET, url_menu, null,
                response -> {
                    try {
                        plant.clear();
                        animal.clear();

                        JSONArray array;
                        array = response.getJSONArray("plant");
                        for(int i = 0 ; i < array.length() ; i++) {
                            plant.add(array.getString(i));
                        }

                        array = response.getJSONArray("animal");
                        for(int i = 0 ; i < array.length() ; i++) {
                            animal.add(array.getString(i));
                        }
                        Toast.makeText(getApplicationContext(), "Fetched species list", Toast.LENGTH_SHORT).show();
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                },
                error -> {error.printStackTrace();}
        );
        queue.add(getRequest);
    }

    private void submitEnable(boolean value) {
        simBtn.setEnabled(value);
        if(value)
            simBtn.setBackground(getDrawable(R.drawable.bg_round_gradient_1));
        else
            simBtn.setBackground(getDrawable(R.drawable.bg_round_gradient_2));
    }
}
