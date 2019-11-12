#include <arrayfire.h>
#include <iostream>

int main()
{
    af::array a = af::randu(2, 2);
    af_print(a);
}
