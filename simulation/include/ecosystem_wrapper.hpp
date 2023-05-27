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

struct BufferData {
  uint8_t *data;
  uint32_t length;
};

EXPORT_C void* session_init();
EXPORT_C const char* get_ecosystem_root(void *session_ptr);
EXPORT_C void create_god(void *session_ptr, uint8_t gods_eye, const char *ecosystem_root);
EXPORT_C void set_initial_organisms(void *session_ptr, uint32_t kingdom, const char *kind,
                                    uint32_t age, uint32_t count);
EXPORT_C void clean_slate(void *session_ptr);
EXPORT_C void create_world(void *session_ptr);
EXPORT_C struct BufferData happy_new_year(void *session_ptr);
EXPORT_C const char *get_world_instance(void *session_ptr);
EXPORT_C const char *get_organism_attribute_list(void *session_ptr);
EXPORT_C void free_god(void *session_ptr);
EXPORT_C void free_session(void *session_ptr);

#endif
