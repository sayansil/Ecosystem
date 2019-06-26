from organism import Animal
from helper import random_binary

class Deer(Animal):

    species_name = 'deer'

    # Chromosome structure for Deer
    chromosome_structure = {
        # Base Vitality
        'bv':{'start':0, 'length':10},
        # Vitality Multiplier
        'vm':{'start':10, 'length':8},
        # Base Stamina
        'bs':{'start':18, 'length':10},
        # Stamina Multiplier
        'sm':{'start':28, 'length':8},
        # Max Height
        'mh':{'start':36, 'length':10},
        # Max Weight
        'mw':{'start':46, 'length':10},
        # Base Height
        'bh':{'start':56, 'length':10},
        # Base Weight
        'bw':{'start':66, 'length':10},
        # Height Multiplier
        'hm':{'start':76, 'length':8},
        # Weight Multiplier
        'wm':{'start':84, 'length':8},
        # Base Speed
        'bp':{'start':92, 'length':10},
        # Speed Multiplier
        'pm':{'start':102, 'length':8},
        # Base appetite
        'ba':{'start':110, 'length':10},
        # Immunity
        'im':{'start':120, 'length':10},
        # Gender
        'gn':{'start':130, 'length':1}
    }

    def __init__(
        self,
        chromosome,
        generation,
        X=None,
        Y=None,
        name=None):

        super(Deer, self).__init__(
            kind=Deer.species_name,
            chromosome=chromosome,
            chromosome_structure=Deer.chromosome_structure,
            generation=generation,
            X=X,
            Y=Y,
            name=name
        )

    @classmethod
    def get_random_chromosome_structure(cls):
        chromosome_length = 0
        for _, attribute in cls.chromosome_structure.items():
            chromosome_length = chromosome_length + attribute['length']
        return random_binary(chromosome_length)
