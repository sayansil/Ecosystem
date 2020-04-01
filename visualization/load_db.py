import sqlite3

def create_connection(db_file):
    conn = None
    try:
        conn = sqlite3.connect(db_file)
        print("Connected to Database")
    except sqlite3.Error as e:
        print("Could not connect to Database.")
        print(e)
    return conn


def select_all_rows(conn, table):
    cursor = conn.cursor()
    try:
        cursor.execute("SELECT * FROM " + table)
        rows = cursor.fetchall()
        print("Selected all rows from " + table)
    except sqlite3.Error as e:
        print("Could not select rows from " + table)
        print(e)
    return rows

conn = create_connection('../data/ecosystem_master.db')

with conn:
    rows = select_all_rows(conn, 'ECOSYSTEM_MASTER')

print("rows: ", len(rows))
for row in rows:
    print(row)
