package ss.rmg.ecosystem;

import java.util.ArrayList;
import java.util.List;

public class PlotData {
    private List<Double> X;
    private List<Double> Y;

    private String title;

    public PlotData() {
        X = new ArrayList<>();
        Y = new ArrayList<>();
        this.title = "";
    }

    public PlotData(List<Double> x, List<Double> y, String title) {
        X = x;
        Y = y;
        this.title = title;
    }

    public List<Double> getX() {
        return X;
    }

    public void setX(List<Double> x) {
        X = x;
    }

    public List<Double> getY() {
        return Y;
    }

    public void setY(List<Double> y) {
        Y = y;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

}
