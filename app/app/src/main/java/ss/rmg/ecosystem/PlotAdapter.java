package ss.rmg.ecosystem;

import android.content.Context;
import android.graphics.Color;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.core.content.ContextCompat;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.GridLabelRenderer;
import com.jjoe64.graphview.LegendRenderer;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

import java.util.ArrayList;
import java.util.List;

public class PlotAdapter extends ArrayAdapter<PlotData> {
    private static final String TAG = "PlotAdapter";

    private Context mContext;
    private List<PlotData> itemList;

    public PlotAdapter(@NonNull Context context, ArrayList<PlotData> list) {
        super(context, 0, list);
        this.mContext = context;
        this.itemList = list;
    }


    @NonNull
    @Override
    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        View listItem = convertView;
        if (listItem == null) {
            listItem = LayoutInflater.from(mContext)
                    .inflate(R.layout.item_plot, parent, false);
        }

        PlotData currentItem = itemList.get(position);

        TextView name = listItem.findViewById(R.id.plotTitle);
        name.setText(currentItem.getTitle());

        GraphView plot = listItem.findViewById(R.id.plot);

        List<Double> X = currentItem.getX();
        List<List<Double>> Ys = currentItem.getY();
        List<String> legends = currentItem.getLegend();

        double minX, maxX, minY, maxY;

        minX = X.get(0);
        maxX = X.get(X.size() - 1);

        minY = Ys.get(0).get(0);
        maxY = Ys.get(0).get(0);

        List<DataPoint> tmp = new ArrayList<>();
        for(int i = 0; i < Ys.size(); i++) {
            tmp.clear();
            for(int j = 0; j < Ys.get(i).size(); j++) {
                if (minY > Ys.get(i).get(j)) {
                    minY = Ys.get(i).get(j);
                }
                if (maxY < Ys.get(i).get(j)) {
                    maxY = Ys.get(i).get(j);
                }
                tmp.add(new DataPoint(X.get(j), Ys.get(i).get(j)));
            }

            LineGraphSeries<DataPoint> series = new LineGraphSeries<>(tmp.toArray(new DataPoint[0]));
            series.setTitle(legends.get(i));
            series.setColor(BaseUtility.getRandomColor());

            plot.addSeries(series);
        }

        double t, diffY = maxY - minY;

        if (diffY == 0 && minY == 0) {
            minY  = -1;
            maxY  = 1;
        } else if (diffY == 0) {
            t = minY;
            minY = t - (t * 0.1);
            maxY = t + (t * 0.1);
        } else {
            minY = minY - (diffY * 0.1);
            maxY = maxY + (diffY * 0.1);
        }

        plot.getViewport().setXAxisBoundsManual(true);
        plot.getViewport().setMinX(minX);
        plot.getViewport().setMaxX(maxX);
        plot.getViewport().setYAxisBoundsManual(true);
        plot.getViewport().setMinY(minY);
        plot.getViewport().setMaxY(maxY);

        plot.getGridLabelRenderer().setHorizontalAxisTitle("Time (years)");


        if (legends.size() > 1) {
            plot.getLegendRenderer().setVisible(true);
            plot.getLegendRenderer().setBackgroundColor(ContextCompat.getColor(mContext, android.R.color.transparent));
            plot.getLegendRenderer().setAlign(LegendRenderer.LegendAlign.TOP);
        }

        return listItem;
    }
}
