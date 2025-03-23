import serial
import serial.tools.list_ports

# Listes pour stocker les données
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
    try:
        ligne = ser.readline().decode('utf-8').strip()  # Lire une ligne et nettoyer
        if ligne:
            if "Température" in ligne:
                valeur = float(ligne.split(":")[1].strip().split(" ")[0])
                temperatures.append(valeur)
            elif "Humidité" in ligne:
                valeur = float(ligne.split(":")[1].strip().split(" ")[0])
                humidities.append(valeur)
            elif "Vapeur" in ligne:
                valeur = float(ligne.split(":")[1].strip().split(" ")[0])
                vapors.append(valeur)
            elif "Gaz" in ligne:
                valeur = float(ligne.split(":")[1].strip().split(" ")[0])
                gases.append(valeur)

            # Affichage des valeurs et des moyennes
            print(f"Moyenne températures : {round(moyenne(temperatures), 2)} °C")
            print(f"Moyenne Humidités : {round(moyenne(humidities), 2)} %")
            print(f"Moyenne Vapeurs : {round(moyenne(vapors), 2)} %")
            print(f"Moyenne Gaz : {round(moyenne(gases), 2)} %")
            print("-" * 50)

    except KeyboardInterrupt:
        print("\nArrêt du programme")
        ser.close()
        break
