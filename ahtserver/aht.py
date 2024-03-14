import time
import board
import adafruit_ahtx0
from flask import Flask, render_template, jsonify, request
from threading import Thread
from datetime import datetime

i2c = board.I2C()
sensor = adafruit_ahtx0.AHTx0(i2c)
app = Flask(__name__)

maxf = 0.0
minf = round(((sensor.temperature * (9/5)) + 32), 2)
starttime = datetime.now()
tempf = 0.0
humidity = 0.0

def findMax(value):
    global maxf
    if value > maxf:
        maxf = value
    
def findMin(value):
    global minf
    if value < minf:
        minf = value
    
def handleData():
    global tempf
    global humidity
    while True:
        tempf = round(((sensor.temperature * (9/5)) + 32), 2)
        humidity = round(sensor.relative_humidity, 2)
        findMax(tempf)
        findMin(tempf)
        time.sleep(1) 

@app.route("/")
def job():
    return render_template('index.html')

@app.route('/stuff', methods=['GET'])
def stuff():
    global starttime
    print(tempf)
    uptime = str(datetime.now() - starttime)[0:7]
    return jsonify(tempf=tempf, humidity=humidity, maxf=maxf, minf=minf, uptime=uptime)
    
    
if __name__ == '__main__':
    sensor_thread = Thread(target=handleData)
    sensor_thread.start()
    app.run(host='0.0.0.0', port=5000, debug=True, use_reloader=False)
