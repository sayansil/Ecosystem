import sys
import os

current_dir = os.path.dirname(os.path.abspath(__file__))
root_dir = os.path.join(current_dir, '..')
sys.path.insert(1, os.path.join(root_dir, 'Cpp', 'build', 'python'))

import pyecosystem as pyeco
from helper import SpeciesModel


obj = pyeco.pyecosystem(True)

obj.set_monitor_offsprings(True)


years_to_simulate = 100

species_models = []

species_models.append(SpeciesModel(
    kingdom='plant',
    species='bamboo',
    initial_organism_count=200,
    monitoring_ratio=0.75))

species_models.append(SpeciesModel(
    kingdom='animal',
    species='deer',
    initial_organism_count=150,
    monitoring_ratio=0.50))

for species_model in species_models: 
    species_model.spawn(obj)

while years_to_simulate > 0:
    obj.happy_new_year(True)
    data = obj.get_live_data()
    years_to_simulate-=1
