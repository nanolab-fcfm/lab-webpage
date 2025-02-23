import sqlite3
import os
import re
import logging
from glob import glob
from typing import Dict, Set

from .. import config
from ..utils import read_file_parameters

log = logging.getLogger(__name__)
csv_files = glob(config['Filename']['directory'] + '/**/*.csv', recursive=True)
all_params: Dict[str, Set[str]] = {}

def create_table_if_not_exists(cursor: sqlite3.Cursor, table_name: str, columns: set):
    """Creates the table if it doesn't exist, adding the columns from all_params."""
    # Create the table with columns from all_params
    cursor.execute(f'''
        CREATE TABLE IF NOT EXISTS "{table_name}" (
            "table_name" TEXT PRIMARY KEY,
            {', '.join(f'"{column}" TEXT' for column in columns)}
        )
    ''')

def insert_data_with_columns(cursor: sqlite3.Cursor, table_name: str, row_name: str, params: dict, all_params: Dict[str, Set[str]]):
    """Inserts data into the table, filling missing columns with None."""
    # Update all_params with any new keys from params, adding columns as necessary
    for key in params.keys():
        if key not in all_params[table_name]:
            all_params[table_name].add(key)
            cursor.execute(f'ALTER TABLE "{table_name}" ADD COLUMN "{key}" TEXT')

    # Prepare the SQL query
    param_names = ', '.join(f'"{name}"' for name in all_params[table_name])
    param_placeholders = ', '.join('?' * len(all_params[table_name]))

    # Prepare values with None for missing keys
    row_filled = [params.get(name, None) for name in all_params[table_name]]

    # Insert the row of data into the table
    cursor.execute(f'''
        INSERT INTO "{table_name}" ("table_name", {param_names})
        VALUES (?, {param_placeholders})
    ''', [row_name] + row_filled)

def add_to_parameters_db(csv_file, conn):
            params = read_file_parameters(csv_file)

            # Use the base filename as the row name
            basename = os.path.basename(csv_file).split('.csv')[0]

            # Extract the procedure type from the table name
            procedure_type = re.match(r'\D*', basename).group()

            # Add the first parameters to all_params to create the table
            if procedure_type not in all_params:
                all_params[procedure_type] = set(params.keys())

            # Create the table for this procedure type if it doesn't exist
            create_table_if_not_exists(conn, procedure_type, all_params[procedure_type])

            # Insert the data into the parameters table
            insert_data_with_columns(conn, procedure_type, basename, params, all_params)


def create_db(parent=None):
    new_db = config['Filename']['directory'] + '/parameters.db'
    if os.path.exists(new_db):
        if os.path.exists(new_db + '.bak'):
            log.debug(f'Removing old backup file {new_db}.bak')
            os.remove(new_db + '.bak')

        os.rename(new_db, new_db + '.bak')

    with sqlite3.connect(new_db) as conn:
        for i, csv_file in enumerate(csv_files):
            if parent is not None:
                parent.status_bar.showMessage(f'Processing database ({i+1}/{len(csv_files)})')
            else:
                print(f'Processing database ({i+1}/{len(csv_files)})', end='\r')

            try:
                add_to_parameters_db(csv_file, conn)
            except Exception as e:
                log.error(f"Error reading file {csv_file}: {e}. Skipping.")
                continue

        if parent is not None:
            parent.status_bar.showMessage('Done', 3000)
        else:
            print('Done' + ' '*30)


def main():
    create_db(parent=None)
