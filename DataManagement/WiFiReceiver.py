import datetime

import requests
import time
from server.DatabaseHandler import DatabaseHandler

ip = input("Saisissez l'ip de l'esp : ")
url = "http://" + ip + "/"

dernier_json = None
DatabaseHandler = DatabaseHandler("localtest.db")

while True:
    try:
        response = requests.get(url, timeout=50)
        if response.status_code == 200:
            data = response.json()
            if data != dernier_json:
                print("Nouvelles données :", data)
                now = datetime.datetime.now()
                dernier_json = data

                DatabaseHandler.add_data(data['temperature'], data['humidity'], data['gas'], data['steam'],
                                         str(now.date()), str(now.time().strftime("%H:%M:%S")))
        else:
            print("Réponse inattendue :", response.status_code)
    except Exception as e:
        print("Erreur :", e)

    time.sleep(60)
