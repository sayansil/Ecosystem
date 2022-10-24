import sys
import sqlite3
import pandas as pd
import argparse
from helper import report
from typing import NoReturn
from pathlib import Path
from datetime import datetime
from helper.enums import Kingdom as KingdomE

root_dir = Path(__file__).absolute().parent.parent
ECOSYSTEM_SCHEMA_PATH = root_dir / 'simulation' / 'schema' / 'python'
DB_PATH = root_dir / 'data' / 'ecosystem_master.db'
SAVE_PATH = root_dir / 'outputs'

sys.path.insert(0, str(ECOSYSTEM_SCHEMA_PATH))

from Ecosystem import World, WorldPopulation

def split_data_by_species(df: pd.DataFrame) -> dict:
    species_data = {}
    for index, row in df.iterrows():
        world_b = row["AVG_WORLD"]
        world_obj = World.WorldT.InitFromBuf(world_b, 0)
        population_b = row["POPULATION_WORLD"]
        population_obj = WorldPopulation.WorldPopulationT.InitFromBuf(population_b, 0)

        year = row["YEAR"]
        species_list = world_obj.species
        species_population_list = population_obj.speciesPopulation

        for species in species_list:
            kind = species.kind.decode("utf-8") 
            kingdom = KingdomE(species.kingdom)
            avg_organism = species.organism[0]

            species_population = next(
                (obj for obj in species_population_list if obj.kind == species.kind)
            )

            if kingdom not in species_data:
                species_data[kingdom] = {}
            if kind not in species_data[kingdom]:
                species_data[kingdom][kind] = {}

            species_data[kingdom][kind][year] = {
                "stats": avg_organism,
                "population": species_population,
            }
    return species_data

def get_table() -> pd.DataFrame:
    table_name = 'ECOSYSTEM_MASTER'
    conn = sqlite3.connect(str(DB_PATH))
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM {}".format(table_name.replace('"', '""')))
    rows = cursor.fetchall()
    cols = list(map(lambda x: x[0], cursor.description))
    df = pd.DataFrame(rows, columns=cols).dropna()
    return df

def generate_report(savepath: str) -> NoReturn:
    df = get_table()
    data = split_data_by_species(df)
    report.generate_and_save_report(data, savepath)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Visualise reports for last simulation")
    parser.add_argument('-o', '--output', help='Output filename', default='')
    args = parser.parse_args()
    savepath = args.output

    if not savepath:
        timestamp = datetime.now().strftime("%d-%m-%Y %H_%M_%S")
        savepath = str(SAVE_PATH / 'report {}.pdf'.format(timestamp))

    generate_report(savepath)
