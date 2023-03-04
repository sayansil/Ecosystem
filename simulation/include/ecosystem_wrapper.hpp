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

EXPORT_C void create_god(uint8_t gods_eye, const char *ecosystem_root);
EXPORT_C void set_initial_organisms(uint32_t kingdom, const char *kind,
                                    uint32_t age, uint32_t count);
EXPORT_C void clean_slate();
EXPORT_C void create_world();
EXPORT_C void run_simulation(uint32_t years);

#endif
