"""!Creates a sqlite database"""
## @package dbr
# @file db.py
# @author Corey Maxwell (corey.maxwell18@proton.me)
# @version 0.1
# @date 2024-08-28
# @copyright Copyright (c) 2024
#
import sqlite3 as sql

## Creates a database according to a schema
# @param schema_path path to the .sql file containing the schema for the database
# @param db_path intended path for the .db file
#
def create_tables(schema_path : str, db_path:str) -> None:
    """Creates a database according to a schema"""
    with open(schema_path, 'r', encoding="UTF-8") as sql_file:
        sql_script = sql_file.read()

    try:
        with sql.connect(db_path) as con:
            con = sql.connect(db_path)
            cur = con.cursor()
            # for statement in sql_statements:
            #     cur.execute(statement)
            cur.executescript(sql_script)
            con.commit()
    except sql.Error as e:
        print(e)

## @brief Creates a database using the schema saved at "db/iot_schema.sql" and saving it to "db/iot.db"
#
def main():
    """
    Generates A SQL database with a given schema
    """
    create_tables("db/iot_schema.sql", "db/iot.db")

if __name__ == "__main__":
    main()
