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

void God::mate(const std::string &name1, const std::string &name2, const double& mutationChance)
{
    const auto& parent1 = animals[name1];
    const auto& parent2 = animals[name2];
    auto child_chromosome = helper::get_random_mixture(parent1.chromosome, parent2.chromosome);
    for(auto& bit : child_chromosome)
        if(helper::weighted_prob(mutationChance))
            bit = (bit == '1')?'0':'1';
    produceAnimal(parent1.kind,
                  child_chromosome,
                  parent1.chromosome_structure,
                  std::max(parent1.generation, parent2.generation) + 1,
                  "",
                  (parent1.X + parent2.X) / 2,
                  (parent1.Y + parent2.Y) / 2);
}

static bool checkPairsExisting(const std::vector<std::vector<stat_type>>& test_subjects)
{
    bool flag1 = false, flag2 = false;
    for(const auto& i : test_subjects)
    {
        if(std::get<unsigned int>(i[5]) == 0)
            flag1 = true;
        else
            flag2 = true;
        if(flag1 && flag2)
            return true;
    }
    return false;
}

void God::happyNewYear()
{
    std::vector<std::pair<Animal, double>> animals_vec;

    for(const auto& animal : animals)
    {
        animals_vec.push_back({animal.second, 0.0});
    }

    // sorted by fitness

    std::sort(std::execution::par, animals_vec.begin(), animals_vec.end(), [](const std::pair<Animal, double>& x, const std::pair<Animal, double>& y){
        return x.first.fitness <= y.first.fitness;
    });

    // marked for death

    std::for_each(std::execution::par, animals_vec.begin(), animals_vec.end(), [](std::pair<Animal, double>& x){
        x.second = helper::weighted_prob(std::exp(-x.first.fitness));
    });

    // Remove these animals from the f****** universe

    std::vector<std::string> animals_to_be_slaughtered;

    for(const auto& animal_pair : animals_vec)
    {
        if(animal_pair.second == 1)
        {
            // Being dragged to the slaughterhouse

            animals_to_be_slaughtered.push_back(animal_pair.first.name);
        }
    }

    animals_vec.clear(); animals_vec.shrink_to_fit();

    db.deleteRows(animals_to_be_slaughtered);
    for(const auto& lamb_to_slaughter : animals_to_be_slaughtered)
        animals.erase(lamb_to_slaughter);

    // yearly mating season

    std::unordered_map<std::string, std::vector<Animal>> animalsByKind;
    for(const auto& animal : animals)
        animalsByKind[animal.second.kind].push_back(animal.second);

    int index_parent1, index_parent2;
    for(auto& animal_pair : animalsByKind)
    {
        auto& animal_list = animal_pair.second;
        std::vector<Animal> males, females;
        for(const auto& animal : animal_list)
            if(animal.gender == 0)
            {
                males.push_back(animal);
            }
            else
            {
                females.push_back(animal);
            }

        std::mt19937_64 rng; rng.seed(std::random_device()());

        while(males.size() > 0 && females.size() > 0)
        {
            std::uniform_int_distribution<int> dis(0, std::min(females.size(), males.size()) - 1);
            index_parent1 = dis(rng);
            index_parent2 = dis(rng);
            const auto& parent1 = males[index_parent1];
            const auto& parent2 = females[index_parent2];
            mate(parent1.name, parent2.name, 0.01);

            males.erase(males.begin() + index_parent1);
            females.erase(females.begin() + index_parent2);
            males.shrink_to_fit();
            females.shrink_to_fit();
        }
    }

    // yearly age incrementing todo: parallel v sequential performance test

    std::for_each(std::execution::par, animals.begin(), animals.end(), [](std::pair<std::string, Animal> item){
        item.second.increment_age();
    });
}
