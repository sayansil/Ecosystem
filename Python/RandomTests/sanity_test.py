import sys
import os

current_dir = os.path.dirname(os.path.abspath(__file__))
root_dir = os.path.join(current_dir, '..', '..')
sys.path.insert(1, os.path.join(root_dir, 'Cpp', 'build', 'python'))
sys.path.insert(1, os.path.join(root_dir, 'Python'))

import pyecosystem as pyeco
from helper import SpeciesModel

monitor_variables = ["height", "weight"]

years_to_simulate = 20

obj = pyeco.pyecosystem(False)
obj.set_disable_deaths(True)
full_species_name = 'animal/deer'
obj.reset_species(full_species_name)

res = obj.test_organism(full_species_name, 10, monitor_variables, years_to_simulate)

for var in monitor_variables:
    assert(len(res[var]) == years_to_simulate)

print(res)
