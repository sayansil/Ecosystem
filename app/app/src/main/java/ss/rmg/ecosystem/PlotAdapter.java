package ss.rmg.ecosystem;

import android.content.Context;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

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

        // todo create plot with X[] and Y[]

        return listItem;
    }
}
