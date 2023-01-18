#include <ecosystem_wrapper.hpp>
#include <stdio.h>

int main() {
    create_god(0);
    set_initial_parms(500, 10);
    create_world();
    run_simulation();
}
