from bottle import run, post, request, get
import matplotlib.pyplot as plt
import Monsoon.HVPM as HVPM
import Monsoon.Operations as op
import Monsoon.sampleEngine as sampleEngine

import json
import os
import sys
import time
import numpy as np
import pandas as pd

from processSignal import get_signal

has_measurement = False
init_row = 1
elapsed_time = sys.maxsize
energy_consumption = sys.maxsize
begin_time = time.time()


@get('/startMeasurement')
def startMeasurement():
    global end_time
    end_time = time.time()
    print("Starting measurement...")
    return {"REQUEST": "GOOD"}

@get('/endMeasurement')
def endMeasurement():
    global energy_consumption, elapsed_time, has_measurement, begin_time

    df = pd.read_csv("./measurement.csv", delimiter=',', skiprows=3600 * int(end_time - begin_time),
                     names=['Time(ms)', 'Main(mA)', 'Main Voltage(V)', ''])
    begin_time = time.time()

    df.to_csv("./aux.csv", index=False)

    df = get_signal(df)

    times = df['Time(ms)']
    currents = df['Main(mA)']
    voltages = df['Main Voltage(V)']
    not_timestamps = times.diff()

    try:
        # SW run time in seconds (s)
        elapsed_time = times.iloc[-1] - times.iloc[0]

        # SW energy consumption in Joules (J)
        energy_consumption = (currents * voltages * not_timestamps).sum() / 1000
    except:
        energy_consumption = sys.maxsize
        elapsed_time = sys.maxsize

    df.to_csv("./signal.csv")
    df.drop(df.index, inplace=True)
    df.to_csv("./measurement.csv", index=False)

    has_measurement = True
    print(elapsed_time)

    print("Ending measurement...")
    return {"REQUEST": "GOOD"}


@get('/getMeasurement')
def getMeasurement():
    global elapsed_time, energy_consumption, start_measurement, has_measurement

    while not has_measurement:
        pass

    obj2, obj1 = elapsed_time, energy_consumption

    elapsed_time, energy_consumption = sys.maxsize, sys.maxsize

    print("Sending measurement...")
    has_measurement = False
    return {"REQUEST": "GOOD", "EC": obj1, "RUNTIME": obj2}


if __name__ == '__main__':
    run(host='192.168.0.194', port=8089, debug=False, server='paste')
