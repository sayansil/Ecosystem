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

import java.util.ArrayList;

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
        String data = intent.getStringExtra("data");
        mortality_graphs = new ArrayList<>();
        demographic_graphs = new ArrayList<>();
        copulation_graphs = new ArrayList<>();
        dependency_graphs = new ArrayList<>();
        average_graphs = new ArrayList<>();
        theoretical_graphs = new ArrayList<>();

        generatePlots(data);

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

    private void generatePlots(String data) {
        // todo
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
}