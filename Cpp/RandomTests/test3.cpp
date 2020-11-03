#include <StopWatch.hpp>
#include <iostream>

int main()
{
    timer::StopWatch obj;
    obj = timer::StopWatch();
    std::cout << obj.status << '\n';
}
