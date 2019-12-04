#include <arrayfire.h>

int main()
{
    const int n = 10;
    af::array X(n);
    af::array Y(n);
    for(int i = 0; i < n; i++)
    {
        X(i) = i + 1;
        Y(i) = (i + 1) * (i + 1);
    }
    af::Window win;
    win.setColorMap(AF_COLORMAP_BLUE);
    while (true)
    {
        win.plot(X, Y);
        win.show();
    }
}
