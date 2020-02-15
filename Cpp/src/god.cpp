#include <god.hpp>

God::God()
{
    db = DatabaseManager();
}

God::~God()
{
    std::cout << "There is no GOD\n";
}

void God::produceAnimal(const std::string& kind, const std::string& chromosome, const chromosome_map_type& chromosome_structure, const unsigned int& generation, const std::string& name, const unsigned int& X, const unsigned int& Y)
{
    Animal current_animal(kind, chromosome, chromosome_structure, generation, name, X, Y);
    animals[name] = current_animal;

    std::vector<std::vector<stat_type>> tmp;
    tmp.emplace_back(std::vector<stat_type>{
            stat_type(current_animal.name),
            stat_type(current_animal.kind),
            stat_type(current_animal.chromosome),
            stat_type(current_animal.generation),
            stat_type(current_animal.immunity),
            stat_type(current_animal.gender),
            stat_type(current_animal.age),
            stat_type(current_animal.height),
            stat_type(current_animal.weight),
            stat_type(current_animal.fitness)
    });
    db.insertRows(tmp);
}

void God::spawnAnimal(const std::string &kind, const std::string &chromosome, const chromosome_map_type &chromosome_structure, const unsigned int &generation, const std::string &name)
{
    Animal current_animal(kind, chromosome, chromosome_structure, generation, name);
    animals[name] = current_animal;

    std::vector<std::vector<stat_type>> tmp;
    tmp.emplace_back(std::vector<stat_type>{
            stat_type(current_animal.name),
            stat_type(current_animal.kind),
            stat_type(current_animal.chromosome),
            stat_type(current_animal.generation),
            stat_type(current_animal.immunity),
            stat_type(current_animal.gender),
            stat_type(current_animal.age),
            stat_type(current_animal.height),
            stat_type(current_animal.weight),
            stat_type(current_animal.fitness)
    });
    db.insertRows(tmp);
}

void God::killAnimal(const std::string &name)
{
    animals.erase(name);
}

void God::mate(const std::string &name1, const std::string &name2)
{
    const auto& parent1 = animals[name1];
    const auto& parent2 = animals[name2];
    const auto& child_chromosome = helper::get_random_mixture(parent1.chromosome, parent2.chromosome);
    produceAnimal(parent1.kind,
                  child_chromosome,
                  parent1.chromosome_structure,
                  std::max(parent1.generation, parent2.generation) + 1,
                  "",
                  (parent1.X + parent2.X) / 2,
                  (parent1.Y + parent2.Y) / 2);
}
