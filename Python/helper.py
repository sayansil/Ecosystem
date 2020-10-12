class SpeciesModel:
    def __init__(
            self,
            kingdom: str,
            species: str,
            initial_organism_count: int,
            monitoring_ratio: float):
        self.kingdom = kingdom
        self.species = species
        self.initial_organism_count = initial_organism_count
        self.monitoring_ratio = monitoring_ratio
        self.full_species_name = kingdom + '/' + species

        print("Simulating with " + \
            str(initial_organism_count) + " " + species + "(s) of kingdom " + kingdom + ".")

    def spawn(self, obj):
        obj.reset_species(self.full_species_name)

        monitored_count = self.initial_organism_count * self.monitoring_ratio
        non_monitored_count = self.initial_organism_count - monitored_count

        while monitored_count > 0:
            obj.spawn_organism(self.full_species_name, 10, True) # Enable monitoring
            monitored_count -= 1

        while non_monitored_count > 0:
            obj.spawn_organism(self.full_species_name, 10, False) # Disable monitoring
            non_monitored_count -= 1


