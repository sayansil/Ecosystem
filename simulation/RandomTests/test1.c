#include <stdio.h>

#include <ecosystem_wrapper.hpp>

int main() {
    create_god(0, "/Users/sayansil/src/canvas/Ecosystem");

    set_initial_organisms(0, "deer", 10, 10);
    set_initial_organisms(0, "deer", 20, 100);
    set_initial_organisms(0, "deer", 30, 50);

    create_world();
    run_simulation(100);
}
