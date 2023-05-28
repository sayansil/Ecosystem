#include <stdio.h>

#include <ecosystem_wrapper.hpp>

int main() {
    void *session = session_init();
    create_god(session, 1, get_ecosystem_root(session));
    clean_slate(session);

    set_initial_organisms(session, 0, "deer", 10, 10);
    set_initial_organisms(session, 0, "deer", 20, 100);
    set_initial_organisms(session, 0, "deer", 30, 50);
    set_initial_organisms(session, 1, "bamboo", 30, 50);

    create_world(session);

    for (size_t i = 0; i < 5; i++) {
        happy_new_year(session);
        add_current_world_record(session);
    }

    struct FloatData fData = get_plot_attribute(session, "deer", "population");
    for (size_t i = 0; i < fData.length; i++) {
        printf("%f\n", fData.data[i]);
    }

    fData = get_plot_attribute(session, "bamboo", "population");
    for (size_t i = 0; i < fData.length; i++) {
        printf("%f\n", fData.data[i]);
    }

    printf("%s\n", get_plot_attributes(session));

    free_god(session);
    free_session(session);
}
