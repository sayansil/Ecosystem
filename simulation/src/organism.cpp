#include <helper.hpp>
#include <organism.hpp>

static void increment_age_animal(Ecosystem::Organism *organism);
static void increment_age_plant(Ecosystem::Organism *organism);
static void evaluate_static_fitness_animal(Ecosystem::Organism *organism);
static void evaluate_static_fitness_plant(Ecosystem::Organism *organism);
static bool is_normal_child_animal(const Ecosystem::Organism *organism);
static bool is_normal_child_plant(const Ecosystem::Organism *organism);

namespace organism_opts {
float get_fitness(const Ecosystem::Organism *organism) {
    return organism->static_fitness() * organism->dynamic_fitness();
}

float generate_death_factor(const Ecosystem::Organism *organism) {
    const double tmp = std::exp((std::log(1.0f / 199) / organism->max_age()) *
                                organism->age());
    const double age_death_factor = static_cast<float>((1 - tmp) / (1 + tmp));
    const double fitness_death_factor = 1.0f / get_fitness(organism);

    return static_cast<float>(helper::weighted_average(
        {age_death_factor, fitness_death_factor},
        {organism->age_fitness_on_death_ratio(), 1.0}));
}

bool is_normal_child(const Ecosystem::Organism *organism) {
    if (organism->kingdom() == Ecosystem::KingdomE::Animal) {
        return is_normal_child_animal(organism);
    } else if (organism->kingdom() == Ecosystem::KingdomE::Plant) {
        return is_normal_child_plant(organism);
    } else {
        throw std::runtime_error("Unknown kingdom");
    }

    return false;
}

void increment_age(Ecosystem::Organism *organism) {
    if (organism->kingdom() == Ecosystem::KingdomE::Animal) {
        increment_age_animal(organism);
    } else if (organism->kingdom() == Ecosystem::KingdomE::Plant) {
        increment_age_plant(organism);
    } else {
        throw std::runtime_error("Unknown kingdom");
    }
}

void evaluate_static_fitness(Ecosystem::Organism *organism) {
    if (organism->kingdom() == Ecosystem::KingdomE::Animal) {
        evaluate_static_fitness_animal(organism);
    } else if (organism->kingdom() == Ecosystem::KingdomE::Plant) {
        evaluate_static_fitness_plant(organism);
    } else {
        throw std::runtime_error("Unknown kingdom");
    }
}
};  // namespace organism_opts

static bool is_normal_child_animal(const Ecosystem::Organism *organism) {
    return helper::is_nonzero_nonnegative_nonnan(organism->height()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->weight()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->immunity()) &&
           helper::is_nonzero_nonnegative_nonnan(
               organism->max_appetite_at_age()) &&
           helper::is_nonzero_nonnegative_nonnan(
               organism->max_speed_at_age()) &&
           helper::is_nonzero_nonnegative_nonnan(
               organism->max_stamina_at_age()) &&
           helper::is_nonzero_nonnegative_nonnan(
               organism->max_vitality_at_age()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->base_appetite()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->base_height()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->base_speed()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->base_stamina()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->base_vitality()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->base_weight()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->max_height()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->max_weight()) &&
           helper::is_nonzero_nonnegative_nonnan(
               organism->height_multiplier()) &&
           helper::is_nonzero_nonnegative_nonnan(
               organism->speed_multiplier()) &&
           helper::is_nonzero_nonnegative_nonnan(
               organism->stamina_multiplier()) &&
           helper::is_nonzero_nonnegative_nonnan(
               organism->vitality_multiplier()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->weight_multiplier());
}

static bool is_normal_child_plant(const Ecosystem::Organism *organism) {
    return helper::is_nonzero_nonnegative_nonnan(organism->height()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->weight()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->immunity()) &&
           helper::is_nonzero_nonnegative_nonnan(
               organism->max_vitality_at_age()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->base_height()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->base_vitality()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->base_weight()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->max_height()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->max_weight()) &&
           helper::is_nonzero_nonnegative_nonnan(
               organism->height_multiplier()) &&
           helper::is_nonzero_nonnegative_nonnan(
               organism->vitality_multiplier()) &&
           helper::is_nonzero_nonnegative_nonnan(organism->weight_multiplier());
}

static void evaluate_static_fitness_animal(Ecosystem::Organism *organism) {
    organism->mutate_static_fitness(
        organism->immunity() *
        (0.01f * organism->max_vitality_at_age() +
         0.01f * organism->max_stamina_at_age() +
         organism->max_speed_at_age()) /
        (3 * static_cast<float>(pow(organism->age(), 1.0f / 32))));
}

static void evaluate_static_fitness_plant(Ecosystem::Organism *organism) {
    organism->mutate_static_fitness(
        organism->immunity() * (0.03f * organism->max_vitality_at_age()) /
        (3 * static_cast<float>(pow(organism->age(), 1.0f / 32))));
}

static void increment_age_animal(Ecosystem::Organism *organism) {
    organism->mutate_age(organism->age() + 1);

    // change height and weight independently

    organism->mutate_height(std::min(
        std::max(
            0.5f * (1 + organism->height_multiplier()) *
                    static_cast<float>(log(organism->age() + 1)) -
                static_cast<float>(pow(
                    static_cast<float>(organism->age()) / organism->max_age(),
                    2)) +
                organism->base_height(),
            organism->base_height()),
        organism->max_height()));
    organism->mutate_weight(std::min(
        std::max(organism->weight_multiplier() * organism->max_age() *
                         static_cast<float>(log(organism->age() + 1)) -
                     (0.5f / organism->max_age()) *
                         static_cast<float>(
                             pow(organism->age(),
                                 2 * (organism->weight_multiplier() + 0.75f))) +
                     organism->base_weight(),
                 organism->base_weight()),
        organism->max_weight()));

    // change stats independently

    organism->mutate_max_vitality_at_age(
        organism->base_vitality() *
        (organism->vitality_multiplier() * 0.5f *
             static_cast<float>(pow(organism->max_age(), 0.5f)) *
             static_cast<float>(
                 exp(-pow(organism->age() - organism->max_age() * 0.5f, 2) /
                     (organism->max_age() * organism->base_vitality()))) +
         1));
    organism->mutate_max_stamina_at_age(
        organism->base_stamina() *
        (organism->stamina_multiplier() * 0.5f *
             pow(organism->max_age(), 0.5f) *
             static_cast<float>(
                 exp(-pow(organism->age() - organism->max_age() * 0.5f, 2) /
                     (organism->max_age() * organism->base_stamina()))) +
         1));
    organism->mutate_max_speed_at_age(
        organism->speed_multiplier() * 100 *
            static_cast<float>(
                exp((-1 / (organism->speed_multiplier() *
                           pow(organism->max_age(), 1.5f))) *
                    pow(organism->age() - organism->max_age() / 2.5f, 2))) +
        organism->base_speed());
    organism->mutate_max_appetite_at_age(
        organism->base_appetite() +
        organism->base_appetite() *
            static_cast<float>(
                exp((-0.5f / pow(organism->max_age(), 1.25f)) *
                    pow(organism->age() - organism->max_age() / 3.0f, 2))));

    // change stats dependently

    organism->mutate_max_vitality_at_age(
        organism->max_vitality_at_age() *
        (1 +
         organism->height_on_vitality() * organism->height() /
             organism->max_height() +
         organism->weight_on_vitality() * organism->weight() /
             organism->max_weight()));
    organism->mutate_max_stamina_at_age(
        organism->max_stamina_at_age() *
        (1 +
         organism->height_on_stamina() * organism->height() /
             organism->max_height() +
         organism->weight_on_stamina() * organism->weight() /
             organism->max_weight()));
    organism->mutate_max_speed_at_age(
        organism->max_speed_at_age() *
        (1 +
         organism->height_on_speed() * organism->height() /
             organism->max_height() +
         organism->weight_on_speed() * organism->weight() /
             organism->max_weight()));
    evaluate_static_fitness_animal(organism);
}

static void increment_age_plant(Ecosystem::Organism *organism) {
    organism->mutate_age(organism->age() + 1);

    // change height and weight independently

    organism->mutate_height(std::min(
        std::max(
            0.5f * (1 + organism->height_multiplier()) *
                    static_cast<float>(log(organism->age() + 1)) -
                static_cast<float>(pow(
                    static_cast<double>(organism->age()) / organism->max_age(),
                    2)) +
                organism->base_height(),
            organism->base_height()),
        organism->max_height()));
    organism->mutate_weight(std::min(
        std::max(organism->weight_multiplier() * organism->max_age() *
                         static_cast<float>(log(organism->age() + 1)) -
                     (0.5f / organism->max_age()) *
                         static_cast<float>(
                             pow(organism->age(),
                                 2 * (organism->weight_multiplier() + 0.75f))) +
                     organism->base_weight(),
                 organism->base_weight()),
        organism->max_weight()));

    // change stats independently

    organism->mutate_max_vitality_at_age(
        organism->base_vitality() *
        (organism->vitality_multiplier() * 0.5f *
             static_cast<float>(pow(organism->max_age(), 0.5f)) *
             static_cast<float>(
                 exp(-pow(organism->age() - organism->max_age() * 0.5f, 2) /
                     (organism->max_age() * organism->base_vitality()))) +
         1));

    // change stats dependently

    organism->mutate_max_vitality_at_age(
        organism->max_vitality_at_age() *
        (1 +
         organism->height_on_vitality() * organism->height() /
             organism->max_height() +
         organism->weight_on_vitality() * organism->weight() /
             organism->max_weight()));

    evaluate_static_fitness_plant(organism);
}
