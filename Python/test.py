import sys
import os
ECOSYSTEM_ROOT = os.environ['ECOSYSTEM_ROOT']
sys.path.insert(1, os.path.join(ECOSYSTEM_ROOT, 'Cpp/build/python'))
import pyecosystem as pyeco

kingdom = 'animal'
species = 'deer'
initial_organism_count = 200
years_to_simulate = 100


full_species_name = kingdom + '/' + species

print("Simulating " + str(years_to_simulate) + " years with " + \
    str(initial_organism_count) + " " + species + "(s) of kingdom " + kingdom + ".")

obj = pyeco.pyecosystem(True)

obj.reset_species(full_species_name)
obj.set_monitor_offsprings(True)

monitoring_ratio = 0.75
monitored_count = initial_organism_count * monitoring_ratio
non_monitored_count = initial_organism_count - monitored_count

while monitored_count > 0:
    obj.spawn_organism(full_species_name, 10, True) # Enable monitoring
    monitored_count -= 1

while non_monitored_count > 0:
    obj.spawn_organism(full_species_name, 10, False) # Disable monitoring
    non_monitored_count -= 1


while years_to_simulate > 0:
    obj.happy_new_year(True)
    data = obj.get_live_data()
    years_to_simulate-=1
