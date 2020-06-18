package ss.rmg.ecosystem;

public class PlotData {
    private double[] X;
    private double[] Y;

    private String title;

    public PlotData() {
        X = new double[0];
        Y = new double[0];
        title = "";
    }

    public PlotData(double[] x, double[] y, String title) {
        X = x;
        Y = y;
        this.title = title;
    }

    public double[] getX() {
        return X;
    }

    public void setX(double[] x) {
        X = x;
    }

    public double[] getY() {
        return Y;
    }

    public void setY(double[] y) {
        Y = y;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

}
