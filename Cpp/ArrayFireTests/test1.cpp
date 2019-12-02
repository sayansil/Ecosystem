#include <arrayfire.h>
#include <iostream>
#include <vector>

int main()
{
    const int& n = 10;
    std::vector<double> X(n), Y(n);
    for(int i = 0; i < n; i++)
    {
        X[i] = i;
        Y[i] = X[i] * X[i];
    }
    af::array x(X.size(), X.data());
    af::array y(Y.size(), Y.data());
    af_print(x);
    af_print(y);
    af::Window win("Window");
    while(true)
    {
        win.plot(x, y);
        win.show();
    }

//    const static int width = 512, height = 512;
//    af::Window window(width, height, "2D plot example title");
//    af::array t = af::seq(0, 100, 0.01);
//    af::array X = af::sin(t) * (af::exp(af::cos(t)) - 2 * af::cos(4 * t) - af::pow(af::sin(t / 12), 5));
//    af::array Y = af::cos(t) * (af::exp(af::cos(t)) - 2 * af::cos(4 * t) - af::pow(af::sin(t / 12), 5));
//    while(true)
//    {
//        window.plot(X, Y);
//        window.show();
//    }
}

//#include <arrayfire.h>
//#include <math.h>
//#include <cstdio>

//using namespace af;

//static const int ITERATIONS  = 50;
//static const float PRECISION = 1.0f / ITERATIONS;

//int main(int, char**) {
//    try {
//        // Initialize the kernel array just once
//        af::info();
//        af::Window myWindow(800, 800, "2D Plot example: ArrayFire");

//        array Y;
//        int sign    = 1;
//        array X     = seq(-af::Pi, af::Pi, PRECISION);
//        array noise = randn(X.dims(0)) / 5.f;

//        myWindow.grid(2, 1);

//        for (double val = 0; !myWindow.close();) {
//            Y = sin(X);

//            myWindow(0, 0).plot(X, Y);
//            myWindow(1, 0).scatter(X, Y + noise, AF_MARKER_POINT);

//            myWindow.show();

//            X = X + PRECISION * float(sign);
//            val += PRECISION * float(sign);

//            if (val > af::Pi) {
//                sign = -1;
//            } else if (val < -af::Pi) {
//                sign = 1;
//            }
//        }

//    } catch (af::exception& e) {
//        fprintf(stderr, "%s\n", e.what());
//        throw;
//    }
//    return 0;
//}
