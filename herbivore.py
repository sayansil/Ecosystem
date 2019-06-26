from organism import Animal

class Deer(Animal):
    def __init__(
        self,
        chromosome,
        generation):

        chromosome_structure = {
            # Base Vitality
            'bv':{
                'start':0,
                'length':10
            },
            # Max Vitality
            'vm':{
                'start':10,
                'length':8
            },
            # Base Stamina
            'bs':{
                'start':18,
                'length':10
            },
            # Max Stamina
            'sm':{
                'start':28,
                'length':8
            },
            # Max Height
            'mh':{
                'start':36,
                'length':10
            },
            # Max Weight
            'mw':{
                'start':46,
                'length':10
            },
            # Base Height
            'bh':{
                'start':56,
                'length':10
            },
            # Base Weight
            'bw':{
                'start':66,
                'length':10
            },
            # Height Multiplier
            'hm':{
                'start':76,
                'length':8
            },
            # Weight Multiplier
            'wm':{
                'start':84,
                'length':8
            },
            # Base Speed
            'bp':{
                'start':92,
                'length':10
            },
            # Max Speed
            'pm':{
                'start':102,
                'length':8
            },
            # Base appetite
            'ba':{
                'start':110,
                'length':10
            },
            # Immunity
            'im':{
                'start':120,
                'length':10
            },
            # Gender
            'gn':{
                'start':130,
                'length':1
            }
        }

        super(Deer, self).__init__('deer', chromosome, chromosome_structure, generation)