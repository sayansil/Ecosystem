import sys
import os
ECOSYSTEM_ROOT = os.environ['ECOSYSTEM_ROOT']
sys.path.insert(1, os.path.join(ECOSYSTEM_ROOT, 'Cpp/build'))
import pyecosystem as pyeco

initial_organism_count = 200
years_to_simulate = 100

obj = pyeco.pyecosystem(True)

while initial_organism_count > 0:
    obj.spawn_animal('deer', 10, False)
    initial_organism_count-=1

while years_to_simulate > 0:
    obj.happy_new_year(True)
    obj.remember_species('animal/deer')
    years_to_simulate-=1
