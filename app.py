from flask import Flask, jsonify, render_template
import serial

app = Flask(__name__)

# Initialize serial connection (change 'COM3' to your Arduino's serial port)
ser = serial.Serial('COM5', 9600)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/data')
def get_data():
    if ser.in_waiting > 0:
        data = ser.readline().decode('utf-8').strip()
        # print(data)
        sensor_values = parse_data(data)
        if sensor_values:
            temperature, humidity, lpg, co, smoke, aqi, dust = sensor_values
            return jsonify(
                temperature=temperature,
                humidity=humidity,
                lpg=lpg,
                co=co,
                smoke=smoke,
                aqi=aqi,
                dust=dust
            )
    return jsonify(
        temperature=None,
        humidity=None,
        lpg=None,
        co=None,
        smoke=None,
        aqi=None,
        dust=None
    )

def parse_data(data):
    try:
        # Strip parentheses and split by comma
        # data = data.strip('()')
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
