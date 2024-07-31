from flask import Flask, jsonify, render_template
import serial

app = Flask(__name__)

# Initialize serial connection (change 'COM3' to your Arduino's serial port)
ser = serial.Serial('COM5', 9600)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/data')
# gets the data from the arduino's serial
def get_data():
    # wait till the data is written in the serial
    if ser.in_waiting > 0:
        # read the text
        data = ser.readline().decode('utf-8').strip()
        # print(data)

        # split the data
        sensor_values = parse_data(data)
        if sensor_values:
            # unpack the data
            temperature, humidity, lpg, co, smoke, aqi, dust = sensor_values
            # jsonify it
            return jsonify(
                temperature=temperature,
                humidity=humidity,
                lpg=lpg,
                co=co,
                smoke=smoke,
                aqi=aqi,
                dust=dust
            )
    # if there's no data return none
    return jsonify(
        temperature=None,
        humidity=None,
        lpg=None,
        co=None,
        smoke=None,
        aqi=None,
        dust=None
    )

# the data is in the format of "temp,humid,etc"
def parse_data(data):
    try:
        values = data.split(',')
        # Convert string values to floats
        temperature = float(values[1])
        humidity = float(values[2])
        lpg = float(values[3])
        co = float(values[4])
        smoke = float(values[5])
        aqi = float(values[6])
        dust = float(values[7])
        # print(temperature, humidity, lpg, co, smoke, aqi, dust)
        return temperature, humidity, lpg, co, smoke, aqi, dust
    except:
        return None

if __name__ == '__main__':
    app.run(debug=True)
