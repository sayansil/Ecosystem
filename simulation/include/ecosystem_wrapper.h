#ifndef ECOSYSTEM_WRAPPER_H
#define ECOSYSTEM_WRAPPER_H

/*
   This is a purely C header file used for the sole purpose of declaring the functions defined in
   ecosystem_wrapper.c. Do not include any C++ specific stuff as this header file along with the ecosystem_wrapper
   shared library would be used for FFI calls across languages.
*/

#include <stdint.h>

void create_god(uint8_t gods_eye);

void set_initial_parms(uint32_t count, uint32_t years);

void create_world();

void run_simulation();

void free_god();

#endif
