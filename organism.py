import numpy as np
class herbivore:
    def __init__(
        self, 
        chromosome, 
        generation):
        # Fixed for the species
        self.max_age = 30
        self.chromosome_number = 256
        self.age_on_death = 5
        self.fitness_on_death = 100
        self.age_fitness_on_death_ratio = 9
        self.height_on_vitality = 0.1
        self.weight_on_vitality = 0.25
        self.height_on_stamina = -0.05
        self.weight_on_stamina = -0.25
        self.height_on_speed = -0.05
        self.weight_on_speed = -0.
        self.vitality_on_appetite = -0.3
        self.stamina_on_appetite = -0.3
        self.vitality_on_speed = 0.0
        self.stamina_on_speed = 0.5

        # Fixed for the organism throughout their lifetime
        self.chromosome = chromosome
        self.generation = generation

        # Changes slowly - once every year (during mating season)
        self.age = 0
        self.max_vitality_at_age = self.get_base_vitality()
        self.max_stamina_at_age = self.get_base_stamina()
        self.max_speed_at_age = self.get_base_speed()
        self.max_appetite_at_age = self.get_base_appetite()
        self.height = self.get_base_height()
        self.weight = self.get_base_weight()

        # Changes dynamically every moment
        self.vitality = self.max_vitality_at_age
        self.stamina = self.max_stamina_at_age
        self.speed = self.max_speed_at_age
        self.appetite = self.max_appetite_at_age

        # Overall survivality of organism
        self.fitness = self.evaluate_fitness()

    def get_base_vitality(self):
        # TODO
        return 123

    def get_base_stamina(self):
        # TODO
        return 123
        
    def get_vitality_multiplier(self):
        # TODO
        return 123

    def get_stamina_multiplier(self):
        # TODO
        return 123

    def get_max_height(self):
        # TODO
        return 123

    def get_base_height(self):
        # TODO
        return 123

    def get_height_multiplier(self):
        # TODO
        return 123

    def get_max_weight(self):
        # TODO
        return 123

    def get_base_weight(self):
        # TODO
        return 123

    def get_weight_multiplier(self):
        # TODO
        return 123

    def get_immunity(self):
        # TODO
        return 123

    def get_base_speed(self):
        # TODO
        return 123

    def get_speed_multiplier(self):
        # TODO
        return 123

    def get_base_appetite(self):
        # TODO
        return 123
        
    def get_gender(self):
        return self.chromosome[self.chromosome_number - 1]

    def evaluate_fitness(self):
        # TODO
        return 123

    def increment_age(self):
        self.age = self.age + 1
        self.height = np.minimum(np.maximum(
            0.5 * (1 + self.get_height_multiplier()) * np.log(self.age + 1) - np.power(self.age / self.max_age, 2) + self.get_base_height(),
            self.get_base_height()),
            self.get_max_height())
        self.weight = np.minimum(np.maximum(
            self.get_weight_multiplier() * self.max_age * np.log(self.age + 1) - (0.5/self.max_age) * np.power(self.age, 2*(self.get_weight_multiplier() + 0.75)) + self.get_base_weight(),
            self.get_base_weight()),
            self.get_max_weight())
        self.max_vitality_at_age = self.get_base_vitality() * (
            self.get_vitality_multiplier() * 0.5 * np.power(self.max_age, 0.5) *
            np.power(np.e, -np.power(self.age - self.max_age*0.5, 2)/(self.max_age * self.get_base_vitality)) + 1)
        self.max_stamina_at_age = self.get_base_stamina() * (
            self.get_stamina_multiplier() * 0.5 * np.power(self.max_age, 0.5) *
            np.power(np.e, -np.power(self.age - self.max_age*0.5, 2)/(self.max_age * self.get_base_stamina)) + 1)
        self.max_vitality_at_age = self.max_vitality_at_age * (1 + 
            self.height_on_vitality * self.height/self.get_max_height() + 
            self.weight_on_vitality * self.weight/self.get_max_weight())
        self.max_stamina_at_age = self.max_stamina_at_age * (1 + 
            self.height_on_stamina * self.height/self.get_max_height() + 
            self.weight_on_stamina * self.weight/self.get_max_weight())
        self.max_speed_at_age = self.get_speed_multiplier() * 100 * np.power(
            np.e, (-1/(self.get_speed_multiplier() * np.power(self.max_age, 1.5))) * np.power(self.age - self.max_age/2.5, 2)) + self.get_base_speed()
        self.max_speed_at_age = self.max_speed_at_age * (1 +
            self.height_on_speed * self.height/self.get_max_height() + 
            self.weight_on_speed * self.weight/self.get_max_weight())
        self.max_appetite_at_age = self.get_base_appetite() + self.get_base_appetite() * np.power(np.e, 
            (-0.5/np.power(self.max_age, 1.25)) * np.power(self.age - self.max_age/3, 2))

    def increment_vitality(self):
        self.vitality = np.minimum(self.vitality + 1, self.max_vitality_at_age)
        self.appetite = self.appetite * (1 + 
            self.vitality_on_appetite * self.vitality/self.max_vitality_at_age)
        self.speed = self.speed * (1 + 
            self.vitality_on_speed * self.vitality/self.max_vitality_at_age)

    def decrement_vitality(self):
        self.vitality = np.maximum(self.vitality - 1, 0)
        self.appetite = self.appetite * (1 + 
            self.vitality_on_appetite * self.vitality/self.max_vitality_at_age)
        self.speed = self.speed * (1 + 
            self.vitality_on_speed * self.vitality/self.max_vitality_at_age)

    def increment_stamina(self):
        self.stamina = np.minimum(self.stamina + 1, self.max_stamina_at_age)
        self.appetite = self.appetite * (1 +
            self.stamina_on_appetite * self.stamina/self.max_stamina_at_age)
        self.speed = self.speed * (1 +
            self.stamina_on_speed * self.stamina/self.max_stamina_at_age)

    def decrement_stamina(self):
        self.stamina = np.maximum(self.stamina - 1, 0)
        self.appetite = self.appetite * (1 +
            self.stamina_on_appetite * self.stamina/self.max_stamina_at_age)
        self.speed = self.speed * (1 +
            self.stamina_on_speed * self.stamina/self.max_stamina_at_age)

    def feed(self, food):
        # TODO
        pass
    

    def will_die_of_age(self):
        return np.minimum(1, np.power(np.e, self.age_on_death*(self.age/self.max_age - 1)))

    def will_die_of_fitness(self):
        return np.minimum(1, np.power(np.e, -1 * self.fitness_on_death * self.fitness))

    def will_die(self):
        self.age_fitness_on_death_ratio = self.age_fitness_on_death_ratio * (2 - self.will_die_of_age())
        return np.average(
            [self.will_die_of_age(), self.will_die_of_fitness()],
            weights=[self.age_fitness_on_death_ratio, 1])
