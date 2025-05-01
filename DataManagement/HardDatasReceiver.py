import serial
import serial.tools.list_ports

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


def moyenne(liste):
    return sum(liste) / len(liste) if liste else 0


userSerPort = port_detector()
if userSerPort:
    print(f"ESP connecté sur le port {userSerPort}")
    ser = serial.Serial(userSerPort, 115200)
else:
    print("ESP non détecté !")

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

    print(f"Moyenne Températures : {round(moyenne(temperatures), 2)} °C")
    print(f"Moyenne Humidités : {round(moyenne(humidities), 2)} %")
    print(f"Moyenne Vapeurs : {round(moyenne(vapors), 2)} %")
    print(f"Moyenne Gaz : {round(moyenne(gases), 2)} %")
    print("-" * 50)
