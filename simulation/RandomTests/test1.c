#include <stdio.h>

#include <ecosystem_wrapper.hpp>

int main() {
    create_god(0, "/Users/sayansil/src/canvas/Ecosystem");

    set_initial_organisms(0, "deer", 10, 10);
    set_initial_organisms(0, "deer", 20, 100);
    set_initial_organisms(0, "deer", 30, 50);

    // run_simulation(100);

    clean_slate();
    create_world();

    happy_new_year();

    struct BufferData fbList = happy_new_year();
    printf("Length: %d\n", (int)fbList.length);

    free_god();
}
