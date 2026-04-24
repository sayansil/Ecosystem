#define CATCH_CONFIG_MAIN
#include <flatbuffers/idl.h>
#include <fmt/core.h>

#include <catch2/catch.hpp>
#include <database_manager.hpp>
#include <god.hpp>
#include <nlohmann/json.hpp>
#include <setup.hpp>
#include <stat_fetcher.hpp>
#include <unordered_map>

#include <population_generated.h>
#include <world_generated.h>

static void setup_once() {
    setup::setup(helper::get_ecosystem_root());
}

static std::vector<std::unordered_map<std::string, std::string>>
make_organisms(const std::string &kind, const std::string &kingdom,
               const std::string &age, size_t count) {
    std::vector<std::unordered_map<std::string, std::string>> orgs;
    orgs.reserve(count);
    for (size_t i = 0; i < count; i++) {
        orgs.push_back({{"kind", kind}, {"kingdom", kingdom}, {"age", age}});
    }
    return orgs;
}

static size_t total_population(const flatbuffers::DetachedBuffer &buf) {
    auto world = Ecosystem::GetWorld(buf.data());
    size_t total = 0;
    for (auto sp : *world->species()) {
        if (sp->organism()) total += sp->organism()->size();
    }
    return total;
}

static size_t count_species_kind(const flatbuffers::DetachedBuffer &buf,
                                  const std::string &target_kind) {
    auto world = Ecosystem::GetWorld(buf.data());
    for (auto sp : *world->species()) {
        if (sp->kind() && sp->kind()->str() == target_kind) {
            return sp->organism() ? sp->organism()->size() : 0;
        }
    }
    return 0;
}

static bool has_organisms_of_age(const flatbuffers::DetachedBuffer &buf,
                                  uint32_t target_age, uint32_t min_count) {
    auto world = Ecosystem::GetWorld(buf.data());
    uint32_t count = 0;
    for (auto sp : *world->species()) {
        if (!sp->organism()) continue;
        for (auto org : *sp->organism()) {
            if (org->age() == target_age) count++;
        }
    }
    return count >= min_count;
}

static uint32_t max_generation(const flatbuffers::DetachedBuffer &buf) {
    auto world = Ecosystem::GetWorld(buf.data());
    uint32_t max_gen = 0;
    for (auto sp : *world->species()) {
        if (!sp->organism()) continue;
        for (auto org : *sp->organism()) {
            if (org->generation() > max_gen) max_gen = org->generation();
        }
    }
    return max_gen;
}

static bool all_organisms_valid(const flatbuffers::DetachedBuffer &buf) {
    auto world = Ecosystem::GetWorld(buf.data());
    for (auto sp : *world->species()) {
        if (!sp->organism()) continue;
        for (auto org : *sp->organism()) {
            if (!org->kind() || org->kind()->size() == 0) return false;
            if (!org->chromosome() || org->chromosome()->size() == 0) return false;
            if (!org->name() || org->name()->size() == 0) return false;
            if (org->base_height() <= 0 || org->base_weight() <= 0) return false;
            if (org->base_vitality() <= 0) return false;
        }
    }
    return true;
}

// ============================================================
// Setup
// ============================================================

TEST_CASE("Setup for correctness tests", "[correctness_setup]") {
    REQUIRE_NOTHROW(setup_once());
}

// ============================================================
// Population dynamics
// ============================================================

TEST_CASE("Population decreases when deaths exceed births",
          "[correctness][population]") {
    setup_once();

    auto orgs = make_organisms("deer", "0", "1", 500);
    God god;
    god.cleanSlate();
    god.createWorld(orgs);

    size_t initial_pop = total_population(god.buffer);
    REQUIRE(initial_pop == 500);

    god.disable_deaths = false;
    size_t min_pop = initial_pop;
    for (int i = 0; i < 20; i++) {
        god.happy_new_year(false);
        size_t pop = total_population(god.buffer);
        min_pop = std::min(min_pop, pop);
    }

    REQUIRE(min_pop < initial_pop);
}

TEST_CASE("No deaths flag preserves all organisms",
          "[correctness][population]") {
    setup_once();

    auto orgs = make_organisms("deer", "0", "20", 50);
    God god;
    god.cleanSlate();
    god.createWorld(orgs);

    god.disable_deaths = true;
    for (int i = 0; i < 10; i++) {
        god.happy_new_year(false);
    }

    size_t final_pop = total_population(god.buffer);
    REQUIRE(final_pop >= 50);
}

TEST_CASE("Year counter increments correctly", "[correctness][year]") {
    setup_once();

    auto orgs = make_organisms("deer", "0", "20", 10);
    God god;
    god.cleanSlate();
    god.createWorld(orgs);

    REQUIRE(god.year == 1);

    for (int i = 0; i < 5; i++) {
        god.happy_new_year(false);
    }
    REQUIRE(god.year == 6);
}

TEST_CASE("Organisms age after each year", "[correctness][aging]") {
    setup_once();

    auto orgs = make_organisms("deer", "0", "5", 20);
    God god;
    god.cleanSlate();
    god.createWorld(orgs);

    god.disable_deaths = true;
    god.happy_new_year(false);

    REQUIRE(has_organisms_of_age(god.buffer, 6, 1));
}

// ============================================================
// Multi-species
// ============================================================

TEST_CASE("Multi-species simulation: deer + bamboo",
          "[correctness][multispecies]") {
    setup_once();

    auto deer = make_organisms("deer", "0", "20", 100);
    auto bamboo = make_organisms("bamboo", "1", "20", 20);
    deer.insert(deer.end(), bamboo.begin(), bamboo.end());

    God god;
    god.cleanSlate();
    god.createWorld(deer);

    REQUIRE(total_population(god.buffer) == 120);
    REQUIRE(count_species_kind(god.buffer, "deer") == 100);
    REQUIRE(count_species_kind(god.buffer, "bamboo") == 20);

    god.disable_deaths = true;
    for (int i = 0; i < 5; i++) {
        god.happy_new_year(false);
    }

    REQUIRE(count_species_kind(god.buffer, "deer") >= 100);
    REQUIRE(count_species_kind(god.buffer, "bamboo") >= 20);
}

TEST_CASE("Asexual reproduction: grass", "[correctness][asexual]") {
    setup_once();

    auto grass = make_organisms("grass", "1", "5", 50);
    God god;
    god.cleanSlate();
    god.createWorld(grass);

    REQUIRE(count_species_kind(god.buffer, "grass") == 50);

    god.disable_deaths = true;
    for (int i = 0; i < 10; i++) {
        god.happy_new_year(false);
    }

    size_t grass_pop = count_species_kind(god.buffer, "grass");
    REQUIRE(grass_pop > 50);
}

TEST_CASE("Plant species: bamboo longevity", "[correctness][plants]") {
    setup_once();

    auto bamboo = make_organisms("bamboo", "1", "20", 30);
    God god;
    god.cleanSlate();
    god.createWorld(bamboo);

    god.disable_deaths = true;
    for (int i = 0; i < 5; i++) {
        god.happy_new_year(false);
    }

    size_t bamboo_pop = count_species_kind(god.buffer, "bamboo");
    REQUIRE(bamboo_pop >= 30);
}

// ============================================================
// Organism correctness
// ============================================================

TEST_CASE("All created organisms have valid required fields",
          "[correctness][organism]") {
    setup_once();

    auto orgs = make_organisms("deer", "0", "20", 100);
    God god;
    god.cleanSlate();
    god.createWorld(orgs);

    REQUIRE(all_organisms_valid(god.buffer));
}

TEST_CASE("Children have higher generation than parents",
          "[correctness][generation]") {
    setup_once();

    auto orgs = make_organisms("deer", "0", "20", 200);
    God god;
    god.cleanSlate();
    god.createWorld(orgs);

    REQUIRE(max_generation(god.buffer) == 0);

    god.disable_deaths = true;
    for (int i = 0; i < 20; i++) {
        god.happy_new_year(false);
    }

    uint32_t gen = max_generation(god.buffer);
    REQUIRE(gen > 0);
}

TEST_CASE("Deer population grows with no deaths",
          "[correctness][growth]") {
    setup_once();

    auto orgs = make_organisms("deer", "0", "20", 100);
    God god;
    god.cleanSlate();
    god.createWorld(orgs);

    god.disable_deaths = true;
    for (int i = 0; i < 5; i++) {
        god.happy_new_year(false);
    }

    size_t final_pop = total_population(god.buffer);
    REQUIRE(final_pop > 100);
}

// ============================================================
// Stat fetcher
// ============================================================

TEST_CASE("Stat fetcher produces valid avg_world",
          "[correctness][stat_fetcher]") {
    setup_once();

    auto orgs = make_organisms("deer", "0", "20", 50);
    God god;
    god.cleanSlate();
    god.createWorld(orgs);

    god.avg_buffer = stat_fetcher::create_avg_world(god.buffer);
    REQUIRE(god.avg_buffer.size() > 0);

    auto avg_world = Ecosystem::GetWorld(god.avg_buffer.data());
    REQUIRE(avg_world != nullptr);
    REQUIRE(avg_world->species() != nullptr);
    REQUIRE(avg_world->species()->size() > 0);
}

TEST_CASE("Stat fetcher produces valid population_stats",
          "[correctness][stat_fetcher]") {
    setup_once();

    auto orgs = make_organisms("deer", "0", "20", 50);
    God god;
    god.cleanSlate();
    god.createWorld(orgs);

    god.population_stats =
        stat_fetcher::get_population_stats(god.buffer);
    REQUIRE(god.population_stats.size() > 0);

    auto wp = Ecosystem::GetWorldPopulation(god.population_stats.data());
    REQUIRE(wp != nullptr);
    REQUIRE(wp->species_population() != nullptr);
    REQUIRE(wp->species_population()->size() > 0);

    auto sp = wp->species_population()->Get(0);
    REQUIRE(sp->kind() != nullptr);
    REQUIRE(sp->kind()->str() == "deer");

    uint32_t total_matable = sp->matable_population()->male_population() +
                              sp->matable_population()->female_population();
    uint32_t total_non_matable =
        sp->non_matable_population()->male_population() +
        sp->non_matable_population()->female_population();
    REQUIRE(total_matable + total_non_matable == 50);
}

TEST_CASE("get_generation_count returns valid data",
          "[correctness][stat_fetcher]") {
    setup_once();

    auto orgs = make_organisms("deer", "0", "20", 50);
    God god;
    god.cleanSlate();
    god.createWorld(orgs);

    god.disable_deaths = true;
    for (int i = 0; i < 5; i++) {
        god.happy_new_year(false);
    }

    auto gen_counts = stat_fetcher::get_generation_count(god.buffer);
    REQUIRE(gen_counts.size() > 0);
    REQUIRE(gen_counts.count(0) > 0);

    uint32_t total = 0;
    for (auto &[gen, count] : gen_counts) {
        total += count;
    }
    REQUIRE(total == total_population(god.buffer));
}

TEST_CASE("get_age_count matches organism ages",
          "[correctness][stat_fetcher]") {
    setup_once();

    auto orgs = make_organisms("deer", "0", "10", 30);
    God god;
    god.cleanSlate();
    god.createWorld(orgs);

    god.disable_deaths = true;
    god.happy_new_year(false);

    auto age_counts = stat_fetcher::get_age_count(god.buffer);
    REQUIRE(age_counts.size() > 0);

    uint32_t total = 0;
    for (auto &[age, count] : age_counts) {
        total += count;
    }
    REQUIRE(total == total_population(god.buffer));
}

// ============================================================
// Database persistence
// ============================================================

TEST_CASE("DB row count matches years with persistence",
          "[correctness][database]") {
    setup_once();

    const size_t sim_years = 5;
    auto orgs = make_organisms("deer", "0", "20", 30);

    {
        God god(helper::get_ecosystem_root(), true);
        god.cleanSlate();
        god.createWorld(orgs);
        for (size_t i = 0; i < sim_years; i++) {
            god.happy_new_year(false);
        }
    }

    DatabaseManager db(
        std::filesystem::path(helper::get_ecosystem_root()) / "data" /
        "ecosystem_master.db");
    auto rows = db.read_all_rows();
    REQUIRE(rows.size() == sim_years);
}

// ============================================================
// Edge cases
// ============================================================

TEST_CASE("Single organism simulation", "[correctness][edge]") {
    setup_once();

    auto orgs = make_organisms("deer", "0", "20", 1);
    God god;
    god.cleanSlate();
    god.createWorld(orgs);

    REQUIRE(total_population(god.buffer) == 1);

    god.disable_deaths = true;
    god.happy_new_year(false);

    REQUIRE(total_population(god.buffer) >= 1);
}

TEST_CASE("Organisms at max age still simulate", "[correctness][edge]") {
    setup_once();

    auto orgs = make_organisms("deer", "0", "50", 20);
    God god;
    god.cleanSlate();
    god.createWorld(orgs);

    REQUIRE_NOTHROW(god.happy_new_year(false));
}

TEST_CASE("Multiple createWorld calls reset state",
          "[correctness][edge]") {
    setup_once();

    auto orgs1 = make_organisms("deer", "0", "20", 50);
    auto orgs2 = make_organisms("deer", "0", "20", 30);

    God god;
    god.cleanSlate();
    god.createWorld(orgs1);
    REQUIRE(total_population(god.buffer) == 50);

    god.cleanSlate();
    god.createWorld(orgs2);
    REQUIRE(total_population(god.buffer) == 30);
}

TEST_CASE("Three species simultaneously", "[correctness][edge]") {
    setup_once();

    auto deer = make_organisms("deer", "0", "20", 30);
    auto grass = make_organisms("grass", "1", "5", 40);
    auto bamboo = make_organisms("bamboo", "1", "20", 30);

    std::vector<std::unordered_map<std::string, std::string>> all_orgs;
    all_orgs.insert(all_orgs.end(), deer.begin(), deer.end());
    all_orgs.insert(all_orgs.end(), grass.begin(), grass.end());
    all_orgs.insert(all_orgs.end(), bamboo.begin(), bamboo.end());

    God god;
    god.cleanSlate();
    god.createWorld(all_orgs);

    REQUIRE(total_population(god.buffer) == 100);
    REQUIRE(count_species_kind(god.buffer, "deer") == 30);
    REQUIRE(count_species_kind(god.buffer, "grass") == 40);
    REQUIRE(count_species_kind(god.buffer, "bamboo") == 30);

    god.disable_deaths = true;
    for (int i = 0; i < 5; i++) {
        god.happy_new_year(false);
    }

    REQUIRE(count_species_kind(god.buffer, "deer") >= 30);
    REQUIRE(count_species_kind(god.buffer, "grass") >= 40);
    REQUIRE(count_species_kind(god.buffer, "bamboo") >= 30);
}

TEST_CASE("Long simulation stability (50 years)", "[correctness][stability]") {
    setup_once();

    auto orgs = make_organisms("deer", "0", "20", 200);
    God god;
    god.cleanSlate();
    god.createWorld(orgs);

    REQUIRE_NOTHROW([&]() {
        for (int i = 0; i < 50; i++) {
            god.happy_new_year(false);
        }
    }());

    REQUIRE(god.year == 51);
    REQUIRE(total_population(god.buffer) > 0);
}
