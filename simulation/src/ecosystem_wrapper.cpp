#include <flatbuffers/idl.h>
#include <flatbuffers/minireflect.h>
#include <fmt/core.h>
#include <population_generated.h>

#include <ecosystem_wrapper.hpp>
#include <filesystem>
#include <god.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <setup.hpp>
#include <stat_fetcher.hpp>
#include <vector>

enum class GodState { unborn, born, dead };

struct SessionHandler {
    SessionHandler() = default;
    std::unique_ptr<God> god;
    std::vector<std::unordered_map<std::string, std::string>> organisms;
    GodState godState = GodState::unborn;
    std::string stringData;
};

void *session_init() {
    SessionHandler *session = new SessionHandler();
    return session;
}

const char *get_ecosystem_root(void *session_ptr) {
    if (session_ptr != nullptr) {
        SessionHandler *session =
            reinterpret_cast<SessionHandler *>(session_ptr);
        session->stringData = helper::get_ecosystem_root();
        return session->stringData.c_str();
    }
    return nullptr;
}

void create_god(void *session_ptr, uint8_t gods_eye,
                const char *ecosystem_root) {
    if (session_ptr != nullptr) {
        SessionHandler *session =
            reinterpret_cast<SessionHandler *>(session_ptr);
        if (session->godState == GodState::unborn ||
            session->godState == GodState::dead) {
            setup::setup(ecosystem_root);
            session->god = std::make_unique<God>(ecosystem_root, gods_eye);
            session->godState = GodState::born;
        }
    }
}

void set_initial_organisms(void *session_ptr, uint32_t kingdom,
                           const char *kind, uint32_t age, uint32_t count) {
    if (session_ptr != nullptr) {
        SessionHandler *session =
            reinterpret_cast<SessionHandler *>(session_ptr);
        if (session->godState == GodState::born) {
            session->organisms.reserve(session->organisms.capacity() + count);

            for (size_t i = 0; i < count; i++) {
                session->organisms.push_back(
                    {{"kind", std::string(kind)},
                     {"kingdom", std::to_string(kingdom)},
                     {"age", std::to_string(age)}});
            }
        }
    }
}

void clean_slate(void *session_ptr) {
    if (session_ptr != nullptr) {
        SessionHandler *session =
            reinterpret_cast<SessionHandler *>(session_ptr);
        if (session->godState == GodState::born) {
            session->god->cleanSlate();
        }
    }
}

void create_world(void *session_ptr) {
    if (session_ptr != nullptr) {
        SessionHandler *session =
            reinterpret_cast<SessionHandler *>(session_ptr);
        if (session->godState == GodState::born) {
            session->god->createWorld(session->organisms);
        }
    }
}

struct BufferData happy_new_year(void *session_ptr) {
    if (session_ptr != nullptr) {
        SessionHandler *session =
            reinterpret_cast<SessionHandler *>(session_ptr);
        if (session->godState == GodState::born) {
            session->god->happy_new_year(true);

            BufferData obj;
            obj.data = session->god->buffer.data();
            obj.length = session->god->buffer.size();

            return obj;
        }
    }

    return BufferData();
}

const char *get_world_instance(void *session_ptr) {
    if (session_ptr != nullptr) {
        SessionHandler *session =
            reinterpret_cast<SessionHandler *>(session_ptr);
        nlohmann::json jsonObject;
        {
            FBuffer avg_world_buffer =
                stat_fetcher::create_avg_world(session->god->buffer);
            flatbuffers::ToStringVisitor visitor("", true, "", false);
            flatbuffers::IterateFlatBuffer(
                avg_world_buffer.data(), Ecosystem::WorldTypeTable(), &visitor);
            jsonObject["avg_world"] = nlohmann::json::parse(visitor.s);
        }
        {
            FBuffer population_buffer =
                stat_fetcher::get_population_stats(session->god->buffer);
            flatbuffers::ToStringVisitor visitor("", true, "", false);
            flatbuffers::IterateFlatBuffer(
                population_buffer.data(), Ecosystem::WorldPopulationTypeTable(),
                &visitor);
            jsonObject["world_population"] = nlohmann::json::parse(visitor.s);
        }
        session->stringData = jsonObject.dump();
        return session->stringData.c_str();
    }
    return nullptr;
}

const char *get_plot_attributes(void *session_ptr) {
    if (session_ptr != nullptr) {
        SessionHandler *session =
            reinterpret_cast<SessionHandler *>(session_ptr);
        session->stringData = "";
        const flatbuffers::TypeTable *type_table =
            Ecosystem::OrganismTypeTable();

        /* Extract float organism attributes */

        for (size_t i = 0; i < type_table->num_elems; i++) {
            if (type_table->type_codes[i].base_type == flatbuffers::ET_FLOAT) {
                session->stringData += type_table->names[i];
                session->stringData += ",";
            }
        }

        /* Extract species population attributes */

        type_table = Ecosystem::SpeciesPopulationTypeTable();
        for (size_t i = 0; i < type_table->num_elems; i++) {
            session->stringData += type_table->names[i];
            session->stringData += ",";
        }

        /* Extract raw population attributes */

        type_table = Ecosystem::RawPopulationTypeTable();
        for (size_t i = 0; i < type_table->num_elems; i++) {
            session->stringData += type_table->names[i];
            session->stringData += ",";
        }
        session->stringData.erase(session->stringData.length() - 1);
        return session->stringData.c_str();
    }
    return "";
}

void free_god(void *session_ptr) {
    if (session_ptr != nullptr) {
        SessionHandler *session =
            reinterpret_cast<SessionHandler *>(session_ptr);
        if (session->godState == GodState::born) {
            session->organisms.clear();
            session->god.reset();
            session->godState = GodState::dead;
        }
    }
}

void free_session(void *session_ptr) {
    SessionHandler *session = reinterpret_cast<SessionHandler *>(session_ptr);
    delete session;
}
