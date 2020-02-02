#include <arrayfire.h>
#include <string>
#include <vector>
#include <utility>

int main()
{
    std::pair<std::string, std::string> titles("x", "x^2");
    int n = 10;
    af::array X = af::seq(1, n);
    af::array Y(n, 2);
    Y(af::span, 0) = X.col(0) * X.col(0);
    Y(af::span, 1) = X.col(0) * X.col(0) * X.col(0);
    af::Window win;
    int count = 0;
    while(true)
    {
        win.setAxesTitles(titles.first.c_str(), titles.second.c_str());
        win.plot(X, Y.col((++count) % 2));
        win.show();
    }
}
