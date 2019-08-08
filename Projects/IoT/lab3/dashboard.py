#!/usr/bin/env python3
from flask import Flask, request, jsonify
app = Flask(__name__)

import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)

TRIG = 24
ECHO = 23

GPIO.setup(TRIG, GPIO.OUT)
GPIO.setup(ECHO, GPIO.IN)

GPIO.output(TRIG, 0)
#print("Waiting for sensor to settle...")
time.sleep(2)

def measure():
    pulse_end = 0
    pulse_start = 0
    GPIO.output(TRIG, 1)
    time.sleep(.00001)
    GPIO.output(TRIG, 0)

    while GPIO.input(ECHO) == 0:
        pulse_start = time.time()
    while GPIO.input(ECHO) == 1:
        pulse_end = time.time()

    pulse_duration = pulse_end - pulse_start
    distance = pulse_duration * 17150
    distance = round(distance, 2)
    return distance
        
@app.route('/measure/')
def getMeasurement():
    list = []
    for _ in range(10):
        list.append(str(measure()) + "cm")
    return jsonify({"result":list})

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=9000, threaded=True)

