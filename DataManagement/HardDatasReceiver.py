import serial
import serial.tools.list_ports
import datetime
import time
import requests

temperatures = []
humidities = []
vapors = []
gases = []
ser = 0


def port_detector():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if "USB" in port.description or "UART" in port.description or "ESP" in port.description:
            return port.device
    return None


userSerPort = port_detector()
if userSerPort:
    print(f"ESP connecté sur le port {userSerPort}")
    ser = serial.Serial(userSerPort, 115200)
else:
    print("ESP non détecté !")
    exit()

while True:
    data = {"Température": None, "Humidité": None, "Vapeur": None, "Gaz": None}

    for _ in range(4):
        ligne = ser.readline().decode('utf-8').strip()
        if ligne:
            for key in data.keys():
                if key in ligne:
                    valeur = float(ligne.split(":")[1].strip().split(" ")[0])
                    data[key] = valeur

    if data["Température"] is not None:
        temperatures.append(data["Température"])
    if data["Humidité"] is not None:
        humidities.append(data["Humidité"])
    if data["Vapeur"] is not None:
        vapors.append(data["Vapeur"])
    if data["Gaz"] is not None:
        gases.append(data["Gaz"])

    now = datetime.datetime.now()
    if all(val is not None for val in [data["Température"], data["Humidité"], data["Gaz"], data["Vapeur"]]):
        payload = {
            "temperature": data["Température"],
            "humidity": data["Humidité"],
            "gas": data["Gaz"],
            "steam": data["Vapeur"],
            "date": str(now.date()),
            "hour": str(now.time().strftime("%H:%M:%S"))
        }
        try:
            requests.post("http://<IP_DU_SERVEUR>:5000/add_data", json=payload)
            print("Données envoyées au serveur :", payload)
        except Exception as e:
            print("Erreur d'envoi au serveur :", e)

    time.sleep(60)

