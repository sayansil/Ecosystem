package ss.rmg.ecosystem;

import java.util.ArrayList;
import java.util.List;

public class PlotData {
    private List<List<Double>> X;
    private List<List<Double>> Y;

    private String title;
    private List<String> legend;

    public PlotData() {
        X = new ArrayList<>();
        Y = new ArrayList<>();
        this.title = "";
    }

    public PlotData(List<List<Double>> x, List<List<Double>> y, String title, List<String> legend) {
        X = x;
        Y = y;
        this.title = title;
        this.legend = legend;
    }

    public List<List<Double>> getX() {
        return X;
    }

    public void setX(List<List<Double>> x) {
        X = x;
    }

    public List<List<Double>> getY() {
        return Y;
    }

    public void setY(List<List<Double>> y) {
        Y = y;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

}
