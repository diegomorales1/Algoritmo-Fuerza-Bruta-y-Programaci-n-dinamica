import random
import string

def generar_casos(nombre_archivo_s1, nombre_archivo_s2, longitud_s1, longitud_s2):
    caracteres = string.ascii_lowercase  # Solo letras minúsculas
    tipo_caso = random.choice(["vacias", "repetidos", "transposiciones"])  # Selección del caso

    if tipo_caso == "vacias":
        cadena1 = ""
        cadena2 = ""
    elif tipo_caso == "repetidos":
        repetido = random.choice(caracteres)
        cadena1 = repetido * longitud_s1
        cadena2 = repetido * longitud_s2
    elif tipo_caso == "transposiciones":
        cadena1 = "".join(random.choice(caracteres) for _ in range(longitud_s1))
        cadena2 = list(cadena1)
        if len(cadena2) > 1:
            # Generar una transposición aleatoria
            i, j = random.sample(range(len(cadena2)), 2)
            cadena2[i], cadena2[j] = cadena2[j], cadena2[i]
        cadena2 = "".join(cadena2)
    else:
        cadena1 = "".join(random.choice(caracteres) for _ in range(longitud_s1))
        cadena2 = "".join(random.choice(caracteres) for _ in range(longitud_s2))

    # Escribir las cadenas en archivos
    with open(nombre_archivo_s1, "w") as archivo1:
        archivo1.write(cadena1)
    with open(nombre_archivo_s2, "w") as archivo2:
        archivo2.write(cadena2)

    print(f"Casos de prueba generados ({tipo_caso}) en {nombre_archivo_s1} y {nombre_archivo_s2}.")

# Configuración de los casos de prueba
longitud_s1 = 10  # Cambia la longitud según desees
longitud_s2 = 10

generar_casos("s1.txt", "s2.txt", longitud_s1, longitud_s2)
