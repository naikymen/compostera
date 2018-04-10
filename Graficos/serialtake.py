#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt
import serial
import serial.tools.list_ports
import time

ports = list(serial.tools.list_ports.comports())
for p in ports:
    print(p)

# Toma dato del serial
time.sleep(2)
ser = serial.Serial('/dev/ttyACM0', 9600)

# Activa ploteo interactivo
plt.ion()
plt.axis([0.1, 100, 0.1, 100000])
plt.semilogy()

# Registrar el tiempo inicial
itime = time.time()

# Para sacar los números del output, depende del formato seteado en el script del arduino.
def findThing(query, target):
    if target.find(query) != -1:
        return target.split(sep=':')[1].split(sep=' ')[1]
    else:
        return -1

# La idea es que busque el nombre del dato, y si el nombre está en la linea que lee, entonces toma el dato.
# En este diccionario tambien están los colores para el gŕafico que van con cada serie.
things = {'Temperature': "red",
          'Humidity': "blue",
          'MQ2 CH4 PPMs': "yellow",
          'Solid moisture EC (% of max)': "grey"
          }


while True:
    # Tomar el tiempo al leer una línea del serial
    ctime = round(time.time() - itime, 1)
    line = str(ser.readline())

    # Buscar los datos en la línea
    for thing in things.keys():
        value = findThing(thing, line)

        # Si encuentra un dato en la línea, entonces lo grafica
        if value != -1:
            print(thing, value)
            plt.scatter(ctime, round(float(value), 1), color=things[thing])

    plt.pause(0.1)

