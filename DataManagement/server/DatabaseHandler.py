import os
import sqlite3

class DatabaseHandler():
    def __init__(self, database_name : str):
        self.connect = sqlite3.connect(f"{os.path.dirname(os.path.abspath(__file__))}/{database_name}")
        self.connect.row_factory = sqlite3.Row

    def add_data(self, temperature, humidity, gas, steam, date, hour):
        cursor = self.connect.cursor()
        query = ""
        cursor.execute(query, (temperature, humidity, gas, steam, date, hour, ))
        cursor.close()
        self.connect.commit()
