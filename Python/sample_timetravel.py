import sys
import os

# Adjust path to find pyecosystem module
current_dir = os.path.dirname(os.path.abspath(__file__))
root_dir = os.path.join(current_dir, '..')
# Path based on typical structure seen in other sample files like Python/RandomTests/sanity_test.py
# Assumes 'root_dir' is the project root, and the script is in <project_root>/Python/
py_module_path = os.path.join(root_dir, 'simulation', 'build', 'python')

if not os.path.exists(py_module_path):
    print(f"Error: Python module path does not exist: {py_module_path}")
    print("Please ensure the simulation has been built (e.g., using CMake and make).")
    sys.exit(1)

sys.path.insert(1, py_module_path)

try:
    import pyecosystem as pyeco
except ImportError:
    print(f"Error: Could not import 'pyecosystem' module from {py_module_path}.")
    print("Make sure the module is correctly built and the path is correct.")
    sys.exit(1)

def run_time_travel_test():
    print("Initializing simulation with database recording enabled (gods_eye=True)...")
    sim = pyeco.pyecosystem(True) # True enables gods_eye for database saving

    # It's good practice to ensure the database is clean for a fresh test run.
    # clean_slate() in C++ clears the DB if gods_eye is true.
    # The pyecosystem wrapper for God::cleanSlate() is not explicitly defined in the provided snippets,
    # but assuming it exists or that a new God instance with gods_eye=true starts fresh.
    # Let's assume God's constructor or a new session ensures a clean relevant state for snapshots for this year,
    # or that an explicit call to a wrapper for clean_slate() would be here.
    # For now, we rely on the fact that `God::happy_new_year` saves data for the *current* year,
    # and `God::load_snapshot` loads by year. A truly clean DB would require `db->clear_database()`.
    # The `God` constructor does not clear the database. `God::cleanSlate()` does.
    # Let's assume `pyecosystem` should have a `clean_slate` method.
    # Based on `simulation/python/ecosystem.cpp`, `pyecosystem` does not have `clean_slate`.
    # This is a potential issue for making the test repeatable if the DB isn't cleared.
    # However, `God::load_snapshot` loads a specific year, and `God::happy_new_year` writes for current year.
    # For this test, we'll proceed. If `clean_slate` is needed, it should be added to `pyecosystem`.
    print("Note: Test assumes either a clean database or that snapshot years won't collide unexpectedly.")

    # Define and Spawn Species
    species_full_name = "animal/deer"
    initial_population = 30 # Reduced for faster testing
    initial_age = 10

    # sim.reset_species(species_full_name) # This method was in the snippet, implies it's available.
    # Let's check if reset_species is what we need. It calls God::reset_species which loads from json.
    # For a simple spawn, we might not need reset_species if constants are already default.
    # The `spawn_organism` in `pyecosystem` takes full_species_name, age, monitor.
    # It does not take a count. We need to loop.

    print(f"Spawning initial population of {initial_population} for {species_full_name}...")
    for _ in range(initial_population):
        sim.spawn_organism(species_full_name, initial_age, False, "") # name can be empty
    print(f"Initial population for {species_full_name} spawned.")

    # Run Simulation for an initial period (e.g., 10-20 years)
    initial_run_years = 15 # Reduced for faster testing
    print(f"Running simulation for {initial_run_years} years to create snapshots...")
    for i in range(initial_run_years):
        current_year_before_hny = sim.get_current_year()
        sim.happy_new_year(False) # Log progress: False to reduce verbosity for now
        print(f"Year {sim.get_current_year()} complete (was {current_year_before_hny}). Snapshots should be saved.")

    current_sim_year = sim.get_current_year()
    print(f"Initial simulation run complete. Current year: {current_sim_year}")

    # List Available Snapshots
    print("Fetching available snapshots...")
    available_snapshots = sim.get_list_of_available_snapshots()

    if not available_snapshots:
        print("Error: No snapshots found! Please ensure:")
        print("1. The `RAW_WORLD BLOB` column was manually added to the `ECOSYSTEM_MASTER` table in the database.")
        print("2. Database saving is working correctly.")
        print("3. The simulation ran for enough years to save snapshots.")
        sys.exit(1)
    print(f"Available snapshot years: {available_snapshots}")

    # Load a Snapshot
    load_success = False
    year_to_load = -1

    if available_snapshots:
        # Try to load a year from the middle, but ensure it's not the most recent year
        # to see a change.
        if len(available_snapshots) > 1 and available_snapshots[-1] == current_sim_year:
             # Prefer a year that is not the current one, if possible
            target_index = max(0, len(available_snapshots) // 2 -1)
            year_to_load = available_snapshots[target_index]
        else:
            year_to_load = available_snapshots[len(available_snapshots) // 2]

        print(f"Attempting to load snapshot for year: {year_to_load}...")
        load_success = sim.load_snapshot_from_year(year_to_load)

        if load_success:
            print(f"Successfully loaded snapshot for year {year_to_load}.")
            loaded_year = sim.get_current_year()
            print(f"Current simulation year after loading: {loaded_year}")
            if loaded_year != year_to_load:
                print(f"Error: Year after loading ({loaded_year}) does not match target year ({year_to_load})!")
                sys.exit(1)
        else:
            print(f"Failed to load snapshot for year {year_to_load}.")
            print("This could be due to issues with reading the snapshot or data integrity.")
            sys.exit(1)
    else:
        print("Skipping snapshot load test as no snapshots were found (should have exited already).")


    # Run Simulation for a few more years from the restored state
    if load_success:
        further_run_years = 5
        print(f"Running simulation for {further_run_years} more years from restored state (year {sim.get_current_year()})...")
        for i in range(further_run_years):
            sim.happy_new_year(False) # Log progress: False
            print(f"Year {sim.get_current_year()} complete after restore.")
        print(f"Simulation complete. Final year: {sim.get_current_year()}")
        expected_final_year = year_to_load + further_run_years
        if sim.get_current_year() != expected_final_year:
             print(f"Error: Final year ({sim.get_current_year()}) does not match expected year ({expected_final_year}) after running from snapshot!")
             sys.exit(1)
        print("Time travel test appears successful!")
    else:
        print("Skipping further simulation run as snapshot loading failed or was skipped.")

if __name__ == "__main__":
    run_time_travel_test()
