import datetime

import requests
import time

ip = input("Saisissez l'ip de l'esp : ")
url = "http://" + ip + "/"

dernier_json = None

while True:
    try:
        response = requests.get(url, timeout=50)
        if response.status_code == 200:
            data = response.json()
            if data != dernier_json:
                print("Nouvelles données :", data)
                now = datetime.datetime.now()
                dernier_json = data

                payload = {
                    "temperature": data['temperature'],
                    "humidity": data['humidity'],
                    "gas": data['gas'],
                    "steam": data['steam'],
                    "date": str(now.date()),
                    "hour": str(now.time().strftime("%H:%M:%S"))
                }
                try:
                    requests.post("http://45.140.165.126:5000/add_data", json=payload)
                except Exception as e:
                    print("Erreur d'envoi au serveur :", e)
        else:
            print("Réponse inattendue :", response.status_code)
    except Exception as e:
        print("Erreur :", e)

    time.sleep(60)
