#include <god.hpp>

int main()
{
    God allah;
    for(int i = 0; i < 500; i++)
    {
        allah.spawnAnimal(Animal("deer"));
    }
    // 50 deers spawned

    std::cout << "Initially: " << allah.animals.size() << '\n';

    for(int i = 0; i < 100; i++)
    {
        std::cout << "\nYear: " << i + 1 << '\n';
        allah.happyNewYear();
    }

    // fast forwarded to 30 years

    std::cout << "\n\nFinally:\n";

    auto sorted_animals = allah.animalSort([](const Animal& x, const Animal& y){
        return std::less<unsigned int>()(x.age, y.age);
    });

    std::cout << "After sorting:\n";
    std::cout << sorted_animals[0].age << ' ' << sorted_animals[sorted_animals.size() - 1].age << '\n';
}
