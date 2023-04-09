#include <stdio.h>

#include <ecosystem_wrapper.hpp>

int main() {
    void *session = session_init();
    create_god(session, 0, get_ecosystem_root(session));
    clean_slate(session);

    set_initial_organisms(session, 0, "deer", 10, 10);
    set_initial_organisms(session, 0, "deer", 20, 100);
    set_initial_organisms(session, 0, "deer", 30, 50);

    create_world(session);

    happy_new_year(session);

    struct BufferData fbList = happy_new_year(session);
    printf("Length: %d\n", (int)fbList.length);
    printf("JSON: %s\n", get_world_instance(session));

    free_god(session);
    session_free(session);
}
