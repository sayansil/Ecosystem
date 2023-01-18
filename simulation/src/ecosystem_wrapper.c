#include <ecosystem_wrapper_impl.hpp>

void create_god(uint8_t gods_eye) {
    create_god_impl(gods_eye);
}

void set_initial_parms(uint32_t count, uint32_t years) {
    set_initial_parms_impl(count, years);
}

void create_world() {
    create_world_impl();
}

void run_simulation() {
    run_simulation_impl();
}

void free_god() {
    free_god_impl();
}
