import sys
import sqlite3
import pandas as pd
import argparse

def get_table(table_name):
    conn = sqlite3.connect('../data/ecosystem_master.db')
    cursor = conn.cursor()
    cursor.execute("SELECT * FROM {}".format(table_name.replace('"', '""')))
    rows = cursor.fetchall()
    cols = list(map(lambda x: x[0], cursor.description))
    df = pd.DataFrame(rows, columns=cols).dropna()
    return df

def generate_report(full_species_name: str, output_file: str):
    kind = full_species_name[full_species_name.find('/') + 1 :]
    kingdom = full_species_name[0: full_species_name.find('/')]
    table_name = 'STATS_' + kind.upper()

    df = get_table(table_name)

    if kingdom == 'animal':
        import helper.report_animal as rep
    elif kingdom == 'plant':
        import helper.report_plant as rep
    else:
        raise Exception("kingdom not defined")

    rep.generate_pdf(df, kind, output_file)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Visualise reports for last simulation")
    parser.add_argument('-s', '--species', help='Full name of the species (kingdom/species)', required=True)
    parser.add_argument('-o', '--output', help='Output filename', default='')
    args = parser.parse_args()

    generate_report(args.species, args.output)
