package ss.rmg.ecosystem;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.airbnb.lottie.LottieAnimationView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class ReportActivity extends AppCompatActivity {

    private static final String TAG = "ReportActivity";

    ArrayList<PlotData> mortality_graphs;
    ArrayList<PlotData> demographic_graphs;
    ArrayList<PlotData> copulation_graphs;
    ArrayList<PlotData> dependency_graphs;
    ArrayList<PlotData> average_graphs;
    ArrayList<PlotData> theoretical_graphs;

    ListView mortality_listview;
    ListView demographic_listview;
    ListView copulation_listview;
    ListView dependency_listview;
    ListView average_listview;
    ListView theoretical_listview;

    TextView mortality_title;
    TextView demographic_title;
    TextView copulation_title;
    TextView dependency_title;
    TextView average_title;
    TextView theoretical_title;

    LottieAnimationView emptyAnim;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_report);

        mortality_listview = findViewById(R.id.listViewMO);
        demographic_listview = findViewById(R.id.listViewDM);
        copulation_listview = findViewById(R.id.listViewCP);
        dependency_listview = findViewById(R.id.listViewDP);
        average_listview = findViewById(R.id.listViewAV);
        theoretical_listview = findViewById(R.id.listViewTH);

        mortality_title = findViewById(R.id.headerMO);
        demographic_title = findViewById(R.id.headerDM);
        copulation_title = findViewById(R.id.headerCP);
        dependency_title = findViewById(R.id.headerDP);
        average_title = findViewById(R.id.headerAV);
        theoretical_title = findViewById(R.id.headerTH);

        emptyAnim = findViewById(R.id.emptyAnim);
        emptyAnim.setVisibility(View.GONE);

        Intent intent = getIntent();

        String data_file = intent.getStringExtra("data_file");
        String kingdom = intent.getStringExtra("kingdom");
        String schema = intent.getStringExtra("schema");
        String data = BaseUtility.readFromFile(
                getString(R.string.directory_parent),
                data_file);

        mortality_graphs = new ArrayList<>();
        demographic_graphs = new ArrayList<>();
        copulation_graphs = new ArrayList<>();
        dependency_graphs = new ArrayList<>();
        average_graphs = new ArrayList<>();
        theoretical_graphs = new ArrayList<>();

        generatePlotData(data, kingdom, schema);

        populatePlotList(mortality_graphs, mortality_listview, mortality_title);
        populatePlotList(demographic_graphs, demographic_listview, demographic_title);
        populatePlotList(copulation_graphs, copulation_listview, copulation_title);
        populatePlotList(dependency_graphs, dependency_listview, dependency_title);
        populatePlotList(average_graphs, average_listview, average_title);
        populatePlotList(theoretical_graphs, theoretical_listview, theoretical_title);

        if (mortality_graphs.size() == 0 &&
                demographic_graphs.size() == 0 &&
                copulation_graphs.size() == 0 &&
                dependency_graphs.size() == 0 &&
                average_graphs.size() == 0 &&
                theoretical_graphs.size() == 0) {
            emptyAnim.setVisibility(View.VISIBLE);
            emptyAnim.playAnimation();
        }
    }

    private void generatePlotData(String data, String kingdom, String schema) {
        mortality_graphs.clear();
        demographic_graphs.clear();
        copulation_graphs.clear();
        dependency_graphs.clear();
        average_graphs.clear();
        theoretical_graphs.clear();

        List<String> plant_schema = new ArrayList<>();
        List<String> animal_schema = new ArrayList<>();

        try {
            JSONObject jsonSchema = new JSONObject(schema);
            JSONArray tmp;
            tmp = jsonSchema.getJSONArray("plant");
            for (int i=0; i<tmp.length(); i++) {
                plant_schema.add(tmp.getString(i));
            }
            tmp = jsonSchema.getJSONArray("animal");
            for (int i=0; i<tmp.length(); i++) {
                animal_schema.add(tmp.getString(i));
            }
        } catch (JSONException e) {
            e.printStackTrace();
            return;
        }

        List<List<String>> raw_table = new ArrayList<>();
        List<List<Double>> table = new ArrayList<>();
        for (String row : data.split("\\s*\\n\\s*")) {
            raw_table.add(Arrays.asList(row.split("\\s*,\\s*")));
        }

        double value;
        for (List<String> raw_row : raw_table) {
            List<Double> row = new ArrayList<>();
            for (String raw_value : raw_row) {
                try {
                    if (raw_value.length() <= 20) {
                        value = Double.parseDouble(raw_value);
                    } else {
                        value = Double.parseDouble(raw_value.substring(0, 20));
                    }
                } catch (NumberFormatException e) {
                    value = 0;
                }
                row.add(value);
            }
            table.add(row);
        }

        String title = "";
        List<Double> X = getRowOfValue(table, plant_schema, "YEAR");

        List<List<Double>> Ys;
        List<String> legend;

        if (kingdom.equalsIgnoreCase("plant")) {

            // MORTALITY GRAPHS

            Ys = new ArrayList<>();legend = new ArrayList<>();

            Ys.add(getRowOfValue(table, plant_schema, "AGE_DTH"));
            legend.add("");
            title = "Age affecting Death";
            mortality_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();

            Ys.add(getRowOfValue(table, plant_schema, "FIT_DTH"));
            legend.add("");
            title = "Fitness affecting Death";
            mortality_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "AFR_DTH"));
            legend.add("");
            title = "Age vs. Fitness affecting Death";
            mortality_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "MX_AGE"));
            legend.add("");
            title = "Maximum age";
            mortality_graphs.add(new PlotData(X, Ys, title, legend));

            // DEMOGRAPHIC GRAPHS

            Ys = new ArrayList<>();legend = new ArrayList<>();
            Ys.add(getRowOfValue(table, plant_schema, "POP"));
            legend.add("");
            title = "Population";
            demographic_graphs.add(new PlotData(X, Ys, title, legend));

            // COPULATION GRAPHS

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "M_POP"));
            legend.add("");
            title = "Matable Population";
            copulation_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "M_AGE_START"));
            legend.add("Age");
            title = "Mating Start";
            copulation_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "M_AGE_END"));
            legend.add("Age");
            title = "Mating End";
            copulation_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "MT_PROB"));
            legend.add("");
            title = "Mutation Probability";
            copulation_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "C_PROB"));
            legend.add("");
            title = "Conceiving Probability";
            copulation_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "OF_FACTOR"));
            legend.add("");
            title = "Multiple offspring factor";
            copulation_graphs.add(new PlotData(X, Ys, title, legend));

            // DEPENDENCY GRAPHS

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "HT_VT"));
            legend.add("Height");
            
            Ys.add(getRowOfValue(table, plant_schema, "WT_VT"));
            legend.add("Weight");
            title = "Factors affecting Vitality";
            dependency_graphs.add(new PlotData(X, Ys, title, legend));

            //AVERAGE GRAPHS

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "AVG_GEN"));
            legend.add("");
            title = "Generation";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "AVG_AGE"));
            legend.add("");
            title = "Age";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "AVG_HT"));
            legend.add("");
            title = "Height";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "AVG_WT"));
            legend.add("");
            title = "Weight";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "AVG_SFIT"));
            legend.add("");
            title = "Static Fitness";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "AVG_IMM"));
            legend.add("");
            title = "Immunity";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "AVG_DTHF"));
            legend.add("");
            title = "Death Factor";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "AVGMA_VT"));
            legend.add("");
            title = "Maximum Vitality";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            // THEORETICAL GRAPHS

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "TMB_HT"));
            legend.add("Base height");
            
            Ys.add(getRowOfValue(table, plant_schema, "TM_HT"));
            legend.add("Net height");
            title = "Maximum height";
            theoretical_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "TMB_WT"));
            legend.add("Base weight");
            
            Ys.add(getRowOfValue(table, plant_schema, "TM_WT"));
            legend.add("Net weight");
            title = "Maximum weight";
            theoretical_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "TMB_VT"));
            legend.add("");
            title = "Maximum base vitality";
            theoretical_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "TMM_VT"));
            legend.add("");
            title = "Maximum vitality multiplier";
            theoretical_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, plant_schema, "TMM_HT"));
            legend.add("Height multiplier");
            
            Ys.add(getRowOfValue(table, plant_schema, "TMM_WT"));
            legend.add("Weight multiplier");
            title = "Maximum physical multipliers";
            theoretical_graphs.add(new PlotData(X, Ys, title, legend));

        } else if (kingdom.equalsIgnoreCase("animal")) {

            // MORTALITY GRAPHS

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "AGE_DTH"));
            legend.add("");
            title = "Age affecting Death";
            mortality_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "FIT_DTH"));
            legend.add("");
            title = "Fitness affecting Death";
            mortality_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "AFR_DTH"));
            legend.add("");
            title = "Age vs. Fitness affecting Death";
            mortality_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "MX_AGE"));
            legend.add("");
            title = "Maximum age";
            mortality_graphs.add(new PlotData(X, Ys, title, legend));

            // DEMOGRAPHIC GRAPHS

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "MALE"));
            legend.add("Male");
            
            Ys.add(getRowOfValue(table, animal_schema, "FEMALE"));
            legend.add("Female");
            title = "Population";
            demographic_graphs.add(new PlotData(X, Ys, title, legend));

            // COPULATION GRAPHS

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "M_MALE"));
            legend.add("Male");
            
            Ys.add(getRowOfValue(table, animal_schema, "M_FEMALE"));
            legend.add("Female");
            title = "Matable Population";
            copulation_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "M_AGE_START"));
            legend.add("Age");
            title = "Mating Start";
            copulation_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "M_AGE_END"));
            legend.add("Age");
            title = "Mating End";
            copulation_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "MT_PROB"));
            legend.add("");
            title = "Mutation Probability";
            copulation_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "C_PROB"));
            legend.add("");
            title = "Conceiving Probability";
            copulation_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "OF_FACTOR"));
            legend.add("");
            title = "Multiple offspring factor";
            copulation_graphs.add(new PlotData(X, Ys, title, legend));

            // DEPENDENCY GRAPHS

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "HT_ST"));
            legend.add("Height");
            
            Ys.add(getRowOfValue(table, animal_schema, "WT_ST"));
            legend.add("Weight");
            title = "Factors effecting Stamina";
            dependency_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "HT_VT"));
            legend.add("Height");
            
            Ys.add(getRowOfValue(table, animal_schema, "WT_VT"));
            legend.add("Weight");
            title = "Factors effecting Vitality";
            dependency_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "HT_SP"));
            legend.add("Height");
            
            Ys.add(getRowOfValue(table, animal_schema, "WT_SP"));
            legend.add("Weight");
            
            Ys.add(getRowOfValue(table, animal_schema, "ST_SP"));
            legend.add("Stamina");
            
            Ys.add(getRowOfValue(table, animal_schema, "VT_SP"));
            legend.add("Vitality");
            title = "Factors effecting Speed";
            dependency_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "VT_AP"));
            legend.add("Vitality");
            
            Ys.add(getRowOfValue(table, animal_schema, "ST_AP"));
            legend.add("Stamina");
            title = "Factors effecting Appetite";
            dependency_graphs.add(new PlotData(X, Ys, title, legend));

            //AVERAGE GRAPHS

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "AVG_GEN"));
            legend.add("");
            title = "Generation";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "AVG_AGE"));
            legend.add("");
            title = "Age";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "AVG_HT"));
            legend.add("");
            title = "Height";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "AVG_WT"));
            legend.add("");
            title = "Weight";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "AVG_SFIT"));
            legend.add("");
            title = "Static Fitness";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "AVG_IMM"));
            legend.add("");
            title = "Immunity";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "AVG_DTHF"));
            legend.add("");
            title = "Death Factor";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "AVGMA_SP"));
            legend.add("");
            title = "Maximum speed";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "AVGMA_AP"));
            legend.add("");
            title = "Maximum appetite";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "AVGMA_ST"));
            legend.add("");
            title = "Maximum stamina";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "AVGMA_VT"));
            legend.add("");
            title = "Maximum vitality";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "AVG_VIS"));
            legend.add("");
            title = "Maximum Vision radius";
            average_graphs.add(new PlotData(X, Ys, title, legend));

            // THEORETICAL GRAPHS

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "TMB_HT"));
            legend.add("Base height");
            
            Ys.add(getRowOfValue(table, animal_schema, "TM_HT"));
            legend.add("Net height");
            title = "Maximum height";
            theoretical_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "TMB_WT"));
            legend.add("Base weight");
            
            Ys.add(getRowOfValue(table, animal_schema, "TM_WT"));
            legend.add("Net weight");
            title = "Maximum weight";
            theoretical_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "TMB_SP"));
            legend.add("Base speed");
            
            Ys.add(getRowOfValue(table, animal_schema, "TM_SP"));
            legend.add("Net speed");
            title = "Maximum speed";
            theoretical_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "TMB_AP"));
            legend.add("");
            title = "Maximum base appetite";
            theoretical_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "TMB_ST"));
            legend.add("");
            title = "Maximum base stamina";
            theoretical_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "TMB_VT"));
            legend.add("");
            title = "Maximum base vitality";
            theoretical_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "TMM_ST"));
            legend.add("Stamina multiplier");
            
            Ys.add(getRowOfValue(table, animal_schema, "TMM_SP"));
            legend.add("Speed multiplier");
            
            Ys.add(getRowOfValue(table, animal_schema, "TMM_VT"));
            legend.add("Vitality multiplier");
            title = "Maximum attribute multipliers";
            theoretical_graphs.add(new PlotData(X, Ys, title, legend));

            Ys = new ArrayList<>();legend = new ArrayList<>();
            
            Ys.add(getRowOfValue(table, animal_schema, "TMM_HT"));
            legend.add("Height multiplier");
            
            Ys.add(getRowOfValue(table, animal_schema, "TMM_WT"));
            legend.add("Weight multiplier");
            title = "Maximum physical multipliers";
            theoretical_graphs.add(new PlotData(X, Ys, title, legend));
        }
    }

    private void populatePlotList(ArrayList<PlotData> graphs, ListView listView, TextView header) {
        if(graphs.size() > 0) {
            header.setVisibility(View.VISIBLE);
            listView.setVisibility(View.VISIBLE);

            PlotAdapter mAdapter = new PlotAdapter(this, graphs);
            listView.setAdapter(mAdapter);
            ListUtils.setDynamicHeight(listView);
        } else {
            header.setVisibility(View.GONE);
            listView.setVisibility(View.GONE);
        }
    }

    public static class ListUtils {
        public static void setDynamicHeight(ListView mListView) {
            ListAdapter mListAdapter = mListView.getAdapter();
            if (mListAdapter == null) {
                return;
            }
            int height = 0;
            int desiredWidth = View.MeasureSpec.makeMeasureSpec(mListView.getWidth(), View.MeasureSpec.UNSPECIFIED);
            for (int i = 0; i < mListAdapter.getCount(); i++) {
                View listItem = mListAdapter.getView(i, null, mListView);
                listItem.measure(desiredWidth, View.MeasureSpec.UNSPECIFIED);
                height += listItem.getMeasuredHeight();
            }
            ViewGroup.LayoutParams params = mListView.getLayoutParams();
            params.height = height + (mListView.getDividerHeight() * (mListAdapter.getCount() - 1));
            mListView.setLayoutParams(params);
            mListView.requestLayout();
        }
    }

    private List<Double> getRowOfValue(List<List<Double>> table, List<String> schema, String rowName) {
        List<Double> result = new ArrayList<>();

        int index = schema.indexOf(rowName);
        for (int i = 0; i < table.size(); i++) {
            try {
                result.add(table.get(i).get(index));
            } catch (Exception e) {
                e.printStackTrace();
                result.add(0.0);
            }
        }

        return result;
    }
}