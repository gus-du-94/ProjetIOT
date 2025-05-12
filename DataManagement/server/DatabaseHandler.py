import os
import sqlite3

class DatabaseHandler():
    def __init__(self, database_name : str):
        self.connect = sqlite3.connect(f"{os.path.dirname(os.path.abspath(__file__))}/{database_name}",
                                       check_same_thread=False)
        self.connect.row_factory = sqlite3.Row

    def add_data(self, temperature, humidity, gas, steam, date, hour):
        cursor = self.connect.cursor()
        query = f"INSERT INTO datas (temperatures, humidity, gas, steam, date, hour) VALUES (?, ?, ?, ?, ?, ?)"
        cursor.execute(query, (temperature, humidity, gas, steam, date, hour, ))
        cursor.close()
        self.connect.commit()

    def get_data(self):
        cursor = self.connect.cursor()
        query = f"SELECT * FROM datas"
        cursor.execute(query)
        data = cursor.fetchall()
        cursor.close()
        self.connect.commit()
        return data

