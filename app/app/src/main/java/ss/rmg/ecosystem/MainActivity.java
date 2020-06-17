package ss.rmg.ecosystem;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.inputmethod.EditorInfo;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

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

    MaterialBetterSpinner spinnerK;
    MaterialBetterSpinner spinnerS;

    MaterialEditText countText;
    MaterialEditText yearsText;
    MaterialEditText ipText;

    SwitchView switchLocal;

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

        spinnerK = findViewById(R.id.spinnerKingdom);
        spinnerS = findViewById(R.id.spinnerSpecies);

        countText = findViewById(R.id.textCount);
        yearsText = findViewById(R.id.textYears);
        ipText = findViewById(R.id.textIP);

        switchLocal = findViewById(R.id.switchLocal);

        simBtn = findViewById(R.id.simBtn);

        isLocal = false;

        queue = Volley.newRequestQueue(this);

        kingdom = new ArrayList<>();
        kingdom.add("plant");
        kingdom.add("animal");

        plant = new ArrayList<>();
        animal = new ArrayList<>();

        text_species = "";
        text_kingdom = "";

        Intent intent = getIntent();
        try {
            JSONObject menu;
            JSONArray array;
            menu = new JSONObject(intent.getStringExtra("menu"));

            array = menu.getJSONArray("plant");
            for(int i = 0 ; i < array.length() ; i++) {
                plant.add(array.getString(i));
            }

            array = menu.getJSONArray("animal");
            for(int i = 0 ; i < array.length() ; i++) {
                animal.add(array.getString(i));
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
            String available_url = getBaseUrl() + "/" + getString(R.string.endpoint_available);
            StringRequest getRequest = new StringRequest(Request.Method.GET, available_url,
                    response -> {
                        if (response.equalsIgnoreCase("True")) {
                            String query_url = getBaseUrl() + "/" + getString(R.string.endpoint_query);
                            query_url += "/" + text_kingdom + "/" + text_species +
                                    "?initial_count=" + countText.getText().toString() +
                                    "&years=" + yearsText.getText().toString() +
                                    "&api-key=" + getString(R.string.api_key);

                            JsonObjectRequest newRequest = new JsonObjectRequest(Request.Method.GET, query_url, null,
                                    newResponse -> {

                                        try {
                                            String status = newResponse.getString("status");
                                            if (status.equals("0")) {
                                                String data = newResponse.getString("data");

                                                // todo with data string
                                            } else if (status.equals("1")) {
                                                BaseUtility.show_popup(R.layout.dialog_invalidapi, this);
                                            } else if (status.equals("2")) {
                                                BaseUtility.show_popup(R.layout.dialog_unavailable, this);
                                            }


                                        } catch (JSONException e) {
                                            e.printStackTrace();
                                            BaseUtility.show_popup(R.layout.dialog_unavailable, this);
                                        }
                                    },
                                    error -> {
                                        BaseUtility.show_popup(R.layout.dialog_unavailable, this);
                                    }
                            );
                            queue.add(newRequest);
                        } else {
                            BaseUtility.show_popup(R.layout.dialog_unavailable, this);
                        }
                    },
                    error -> {}
            );
            queue.add(getRequest);
        });

        switchLocal.setOnCheckedChangeListener((switchView, b) -> {
            isLocal = b;
            if (isLocal) {
                ipText.setVisibility(View.VISIBLE);
            } else {
                ipText.setVisibility(View.INVISIBLE);
                refreshSpinners();
            }
        });

        ipText.setOnEditorActionListener((v, actionId, event) -> {
            if (actionId == EditorInfo.IME_ACTION_SEARCH ||
                    actionId == EditorInfo.IME_ACTION_DONE ||
                    event != null &&
                            event.getAction() == KeyEvent.ACTION_DOWN &&
                            event.getKeyCode() == KeyEvent.KEYCODE_ENTER) {
                if (event == null || !event.isShiftPressed()) {
                    refreshSpinners();
                    return true;
                }
            }
            return false;
        });
    }

    private String getBaseUrl() {
        if (isLocal) {
            return ipText.getText().toString();
        } else {
            return getString(R.string.url_base);
        }
    }

    private void refreshSpinners() {
        String url_menu = getString(R.string.url_base) + "/" + getString(R.string.endpoint_menu);
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
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                },
                error -> {}
        );
        queue.add(getRequest);
    }
}
