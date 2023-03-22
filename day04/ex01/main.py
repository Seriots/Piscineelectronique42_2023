# Définir les valeurs de Srh et St en combinant les données d'entrée
Srh = (int('5B', 16)) << 12 | int('4E', 16) << 4 | int('05', 16) >> 4
St = (int('05', 16) & 0b1111) << 16 | int('F7', 16) << 8 | int('5f', 16)

# Calculer l'humidité relative et la température en utilisant les formules fournies
RH = (Srh / 2**20) * 100
T = (St / 2**20) * 200 - 50

# Afficher les résultats
print("Humidité relative: {:.2f}%".format(RH))
print("Température: {:.2f}°C".format(T))

#52 C7 56 46 17 E9
#5B 4E 05 F7 5F 0F