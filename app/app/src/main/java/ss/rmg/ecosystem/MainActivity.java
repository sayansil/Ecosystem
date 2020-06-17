package ss.rmg.ecosystem;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;

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

public class MainActivity extends AppCompatActivity {

    List<String> kingdom;
    List<String> plant;
    List<String> animal;

    MaterialBetterSpinner spinnerK;
    MaterialBetterSpinner spinnerS;

    MaterialEditText countText;
    MaterialEditText yearsText;

    String text_kingdom;
    String text_species;

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

        simBtn = findViewById(R.id.simBtn);

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
                text_species = plant.get(position);
            }
        });

        simBtn.setOnClickListener(view -> {
            BaseUtility.vibrate(this);
            String available_url = getString(R.string.url_available);
            StringRequest getRequest = new StringRequest(Request.Method.GET, available_url,
                    response -> {
                        if (response.equalsIgnoreCase("True")) {
                            String query_url = getString(R.string.url_query);
                            query_url += "/" + text_kingdom + "/" + text_species +
                                    "?initial_count=" + countText.getText().toString() +
                                    "&years=" + yearsText.getText().toString() +
                                    "&api-key=" + getString(R.string.api_key);

                            StringRequest newRequest = new StringRequest(Request.Method.GET, query_url,
                                    data -> {
                                        // todo with data string
                                    },
                                    error -> {}
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
    }
}
