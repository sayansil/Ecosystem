#include <ecosystem_wrapper.hpp>
#include <filesystem>
#include <god.hpp>
#include <memory>
#include <setup.hpp>
#include <vector>

enum GodState { unborn, born, dead };

std::vector<std::unordered_map<std::string, std::string>> organisms;
God *god;
GodState godState = GodState::unborn;

void create_god(uint8_t gods_eye, const char *ecosystem_root) {
  if (godState == GodState::unborn || godState == GodState::dead) {
    setup::setup(ecosystem_root);
    god = new God(ecosystem_root, gods_eye);
    godState = GodState::born;
  }
}

void set_initial_organisms(uint32_t kingdom, const char *kind, uint32_t age,
                           uint32_t count) {
  if (godState == GodState::born) {
    organisms.reserve(organisms.capacity() + count);

    for (size_t i = 0; i < count; i++) {
      organisms.push_back({{"kind", std::string(kind)},
                           {"kingdom", std::to_string(kingdom)},
                           {"age", std::to_string(age)}});
    }
  }
}

void clean_slate() {
    if (godState == GodState::born) {
        god->cleanSlate();
    }
}

void create_world() {
    if (godState == GodState::born) {
        god->createWorld(organisms);
    }
}

struct BufferData happy_new_year() {
    if (godState == GodState::born) {
        god->happy_new_year(true);

        BufferData obj;
        obj.data = god->buffer.data();
        obj.length = god->buffer.size();

        return obj;
    }

    return BufferData();
}

void free_god() {
    if (godState == GodState::born) {
        organisms.clear();
        delete god;
        godState = GodState::dead;
    }
}
