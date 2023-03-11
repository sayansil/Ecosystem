#include <stdio.h>

#include <ecosystem_wrapper.hpp>

int main() {
    create_god(0, "/Users/sayansil/src/canvas/Ecosystem");
    clean_slate();

    set_initial_organisms(0, "deer", 10, 10);
    set_initial_organisms(0, "deer", 20, 100);
    set_initial_organisms(0, "deer", 30, 50);

    create_world();

    happy_new_year();

    struct BufferData fbList = happy_new_year();
    printf("Length: %d\n", (int)fbList.length);

    free_god();
}
