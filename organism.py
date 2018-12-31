import numpy as np
class herbivore:
    def __init__(
        self, 
        chromosome, 
        generation):
        # Chromosome structure of a species
        self.chromosome_structure = {
            'bv':{
                'start':0,
                'length':10
            },
            'vm':{
                'start':10,
                'length':8
            },
            'bs':{
                'start':18,
                'length':10
            },
            'sm':{
                'start':28,
                'length':8
            },
            'mh':{
                'start':36,
                'length':10
            },
            'mw':{
                'start':46,
                'length':10
            },
            'bh':{
                'start':56,
                'length':10
            },
            'bw':{
                'start':66,
                'length':10
            },
            'hm':{
                'start':76,
                'length':8
            },
            'wm':{
                'start':84,
                'length':8
            },
            'bp':{
                'start':92,
                'length':10
            },
            'pm':{
                'start':102,
                'length':8
            },
            'ba':{
                'start':110,
                'length':10
            },
            'im':{
                'start':120,
                'length':10
            },
            'gn':{
                'start':130,
                'length':1
            }
        }

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
        self.theoretical_maximum_base_vitality = 10000
        self.theoretical_maximum_vitality_multiplier = 1
        self.theoretical_maximum_base_stamina = 10000
        self.theoretical_maximum_stamina_multiplier = 1
        self.theoretical_maximum_height = 5
        self.theoretical_maximum_weight = 400
        self.theoretical_maximum_base_height = self.theoretical_maximum_height * 0.1
        self.theoretical_maximum_base_weight = self.theoretical_maximum_weight * 0.05
        self.theoretical_maximum_height_multiplier = 1
        self.theoretical_maximum_weight_multiplier = 1
        self.theoretical_maximum_speed = 150
        self.theoretical_maximum_base_speed = self.theoretical_maximum_speed * 0.1
        self.theoretical_maximum_speed_multiplier = 1
        self.theoretical_maximum_base_appetite = self.theoretical_maximum_weight * 25

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

    def getValueFromChromosome(self, start, length):
        return int(self.chromosome[start:start+length], 2)

    def get_base_vitality(self):
        bv = self.getValueFromChromosome(
            self.chromosome_structure['bv']['start'],
            self.chromosome_structure['bv']['length'])
        bv = self.theoretical_maximum_base_vitality * (
            bv, np.power(2, self.chromosome_structure['bv']['length']))
        return bv

    def get_base_stamina(self):
        bs = self.getValueFromChromosome(
            self.chromosome_structure['bs']['start'],
            self.chromosome_structure['bs']['length'])
        bs = self.theoretical_maximum_base_stamina * (
            bs, np.power(2, self.chromosome_structure['bs']['length']))
        return bs
        
    def get_vitality_multiplier(self):
        vm = self.getValueFromChromosome(
            self.chromosome_structure['vm']['start'],
            self.chromosome_structure['vm']['length'])
        vm = self.theoretical_maximum_vitality_multiplier * (
            vm, np.power(2, self.chromosome_structure['vm']['length']))
        return vm

    def get_stamina_multiplier(self):
        sm = self.getValueFromChromosome(
            self.chromosome_structure['sm']['start'],
            self.chromosome_structure['sm']['length'])
        sm = self.theoretical_maximum_stamina_multiplier * (
            sm, np.power(2, self.chromosome_structure['sm']['length']))
        return sm

    def get_max_height(self):
        mh = self.getValueFromChromosome(
            self.chromosome_structure['mh']['start'],
            self.chromosome_structure['mh']['length'])
        mh = self.theoretical_maximum_height * (
            mh, np.power(2, self.chromosome_structure['mh']['length']))
        return mh

    def get_base_height(self):
        bh = self.getValueFromChromosome(
            self.chromosome_structure['bh']['start'],
            self.chromosome_structure['bh']['length'])
        bh = self.theoretical_maximum_base_height * (
            bh, np.power(2, self.chromosome_structure['bh']['length']))
        return bh

    def get_height_multiplier(self):
        hm = self.getValueFromChromosome(
            self.chromosome_structure['hm']['start'],
            self.chromosome_structure['hm']['length'])
        hm = self.theoretical_maximum_height_multiplier * (
            hm, np.power(2, self.chromosome_structure['hm']['length']))
        return hm

    def get_max_weight(self):
        mw = self.getValueFromChromosome(
            self.chromosome_structure['mw']['start'],
            self.chromosome_structure['mw']['length'])
        mw = self.theoretical_maximum_weight * (
            mw, np.power(2, self.chromosome_structure['mw']['length']))
        return mw

    def get_base_weight(self):
        bw = self.getValueFromChromosome(
            self.chromosome_structure['bw']['start'],
            self.chromosome_structure['bw']['length'])
        bw = self.theoretical_maximum_base_weight * (
            bw, np.power(2, self.chromosome_structure['bw']['length']))
        return bw

    def get_weight_multiplier(self):
        wm = self.getValueFromChromosome(
            self.chromosome_structure['wm']['start'],
            self.chromosome_structure['wm']['length'])
        wm = self.theoretical_maximum_weight_multiplier * (
            wm, np.power(2, self.chromosome_structure['wm']['length']))
        return wm

    def get_immunity(self):
        # TODO
        im = self.getValueFromChromosome(
            self.chromosome_structure['im']['start'],
            self.chromosome_structure['im']['length'])
        return im

    def get_base_speed(self):
        bp = self.getValueFromChromosome(
            self.chromosome_structure['bp']['start'],
            self.chromosome_structure['bp']['length'])
        bp = self.theoretical_maximum_base_speed * (
            bp, np.power(2, self.chromosome_structure['bp']['length']))
        return bp

    def get_speed_multiplier(self):
        pm = self.getValueFromChromosome(
            self.chromosome_structure['pm']['start'],
            self.chromosome_structure['pm']['length'])
        pm = self.theoretical_maximum_speed_multiplier * (
            pm, np.power(2, self.chromosome_structure['pm']['length']))
        return pm

    def get_base_appetite(self):
        ba = self.getValueFromChromosome(
            self.chromosome_structure['ba']['start'],
            self.chromosome_structure['ba']['length'])
        ba = self.theoretical_maximum_base_appetite * (
            ba, np.power(2, self.chromosome_structure['ba']['length']))
        return ba
        
    def get_gender(self):
        gn =self.getValueFromChromosome(
            self.chromosome_structure['gn']['start'],
            self.chromosome_structure['gn']['length'])
        return gn

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
