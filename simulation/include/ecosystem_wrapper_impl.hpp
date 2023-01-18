#ifndef ECOSYSTEM_WRAPPER_IMPL_HPP
#define ECOSYSTEM_WRAPPER_IMPL_HPP

#include <stdint.h>

#ifdef __cplusplus
  #define EXPORT_C extern "C"
#else
  #define EXPORT_C
#endif

//============ C++ Only Header =================//
#ifdef __cplusplus


#endif //-- End of __cplusplus definition //

EXPORT_C void create_god_impl(uint8_t gods_eye);
EXPORT_C void set_initial_parms_impl(uint32_t count, uint32_t years);
EXPORT_C void create_world_impl();
EXPORT_C void run_simulation_impl();
EXPORT_C void free_god_impl();

#endif
