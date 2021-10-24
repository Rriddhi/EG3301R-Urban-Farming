import serial
import time
import json
from datetime import datetime

# read from Arduino
# expected input: "sensor_type: NUMBER\n"
def read():
    input = ser.readline()
    print(parse(input))
    return input

# parse Arduino input
def parse(input):
    return input.decode("utf-8")

def get_sensor(received_data):
    strings = received_data.split(" ", 1)
    return strings[0]

def get_value(received_data):
    strings = received_data.split(" ", 1)
    return float(strings[1])

# save data in a dictionary
data = {}
SAVE_FILE_PATH = 'save_data.json'
def read_from_json():
    with open(SAVE_FILE_PATH) as json_file:
        data = json.load(json_file)
    print(data)

def write_to_json():
    with open(SAVE_FILE_PATH) as json_file:
        json.dump(data, json_file, indent=4)

def write_to_data(received_data):
    sensor_type = get_sensor(received_data)
    value = get_value(received_data)
    time = datetime.now().strftime("%m/%d/%Y, %H:%M:%S")
    if not (sensor_type in data):
        raise AssertionError("Data does not contain sensor type")
    data[sensor_type].append({"time": time, "value": value})



SERIAL_PATH = "/dev/ttyacm0"
POLL_RATE = 9600
ser = serial.Serial(SERIAL_PATH, POLL_RATE, timeout=5)

# Define the parameters for the loop (such as the number of times it should be run)
ITERATIONS = 10
def main_loop():
    for i in range(0, ITERATIONS):
        sensor_input = read()
        time.sleep(1)

main_loop()
