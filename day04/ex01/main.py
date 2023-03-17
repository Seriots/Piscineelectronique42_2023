# Définir les valeurs de Srh et St en combinant les données d'entrée
Srh = (int('76', 16)) << 12 | int('BA', 16) << 4 | int('A6', 16) >> 4
St = (int('A6', 16) & 0b1111) << 16 | int('53', 16) << 8 | int('CF', 16)

# Calculer l'humidité relative et la température en utilisant les formules fournies
RH = (Srh / 2**20) * 100
T = (St / 2**20) * 200 - 50

# Afficher les résultats
print("Humidité relative: {:.2f}%".format(RH))
print("Température: {:.2f}°C".format(T))

