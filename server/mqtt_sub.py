"""! @brief Subscribes to mqtt"""
## @package mqtt_sub
# @file mqtt_sub.py
# @author Corey Maxwell (corey.maxwell18@proton.me)
# @version 0.1
# @date 2024-08-28
# @copyright Copyright (c) 2024
#
import random
import json
import datetime
import sqlite3

import server.device_writer as device_writer

from paho.mqtt import client as mqtt_client

## @var BROKER
#	MQTT Broker Ip address
BROKER = 'rpi.local'
## @var PORT
#	Port that the broker is running on 
PORT = 1883
## @var MQTT_TOPIC
#	(MQTT topic, QoS level) tuple
MQTT_TOPIC = [("esp32/id", 2), ("esp32/walk", 2)]
## @var DB_PATH
# Path to the
DB_PATH = "db/iot.db"


## @var client_id
# Generate a Client ID with the subscribe prefix.
client_id = f'subscribe-{random.randint(0, 100)}'
## @var devices
# (dev_address, device.Device) MAC address to object dictionary
devices: dict[str, device_writer.DeviceWriter] = {}
## @var sensors
# (sensor_address, device.Sensor) Sensor Address to object dictionary
sensors: dict[str, device_writer.SensorWriter] = {}


##   @brief Parses a JSON Dict into a tuple of device and the sensors connected
#    @param json_dict: JSON Object in the form of a dictionary
#    @return (Device Object, List of Sensor Objects)
#    
def json_parse_device(json_dict: dict) -> tuple[device_writer.DeviceWriter, list[device_writer.SensorWriter]]:
    """Parses a JSON Dict into a tuple of device and the sensors connected"""
    device_address = json_dict["device_address"]
    device_type = json_dict["device_type"]
    dev = device_writer.DeviceWriter(device_address, device_type)
    sensors_dict = json_dict["sensors"]
    sensor_objects = []

    for sensor in sensors_dict:
        sensor_address = sensor["sensor_address"]
        sensor_name = sensor["sensor_name"]
        sensor_description = sensor["sensor_description"]
        unit = sensor["unit"]
        sensor_object = device_writer.SensorWriter(
            sensor_address, dev, sensor_name, sensor_description, unit)
        sensor_objects.append(sensor_object)
    return (dev, sensor_objects)


## @briefParses a reading JSON object.
# @param json_dict JSON object as a dictionary
# @return ReadingsWriter Object
#
def json_parse_reading(json_dict: dict) -> device_writer.ReadingsWriter:
    """Parses a reading JSON object."""
    sensor_address = json_dict["sensor_address"]
    data_value = json_dict["data_value"]
    sensor = sensors[sensor_address]
    reading = sensor.create_readings(data_value)
    return reading

## Parses a JSON Dctionary into a list of Objects
# @param json_dict JSON Object in the form of a dictionary
# @return list List of objects (Device/Sensor/SensorReadings)
#
def parse_json(json_dict: dict) -> list:
    """Parses a JSON Dctionary into a list of Objects"""
    try:
        arr = []
        packet_type = json_dict['packet_type']
        match packet_type:
            case "Device":
                dev, sensor_array = json_parse_device(json_dict)
                if dev.address() not in devices:
                    devices[dev.address()] = dev
                arr.append(dev)
                for sensor_object in sensor_array:
                    if sensor_object.address not in sensors:
                        sensors[sensor_object.address()] = sensor_object
                        arr.append(sensor_object)
            case "SensorReading":
                reading = json_parse_reading(json_dict)
                arr.append(reading)
            case _:
                print("Invalid Packet")
        return arr

    except KeyError:
        print("Invalid Packet")


## @brief Extracts the data from a JSON Object
# @param json_dict A JSON object in the form of a dict object
# @param indent Number of spaces for printing nested JSON Objects (optional)
#
def extract_from_json(json_dict: dict, indent: int = 0) -> None:
    """Extracts the data from a JSON Object"""

    for key in json_dict:
        value = json_dict[key]
        print("[Sub] ", end="")
        for _ in range(0, indent):
            print("   ", end="")
        if isinstance(value, dict):

            print(f"{key} : ")
            extract_from_json(value, indent+1)
        else:
            print(f"{key} : {value}")


## Connects a Client to the MQTT broker
# @return MQTT Client object
#
def connect_mqtt() -> mqtt_client:
    """Connects a Client to the MQTT broker"""
    def on_connect(client, userdata, flags, rc, properties):
        if rc == 0:
            print("Connected to MQTT Broker!")
            print(
                f"client={client}, userdata={userdata}, flags={flags}, properties={properties}")
        else:
            print("Failed to connect, return code %d\n", rc)

    client = mqtt_client.Client(
        mqtt_client.CallbackAPIVersion.VERSION2, client_id)
    # client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(BROKER, PORT, keepalive=7200)
    return client


## Subscribes to a topic on the MQTT broker
# @param client MQTT Client
# @param con Sqlite3 Database connection
#
def subscribe(client: mqtt_client, con: sqlite3.Connection):
    """Subscribes to a topic on the MQTT broker"""

    def on_message(client, userdata, msg):
        print(f"from client={client}, userdata={userdata}")
        dict_json = json.loads(msg.payload.decode())
        time = datetime.datetime.now().strftime("%d/%m/%Y %H:%M:%S")
        print(f"[Sub] From {msg.topic} at {time}")
        arr = parse_json(dict_json)
        print("[Sub] ", end="")
        for obj in arr:
            obj.insert(con)
            print(f" {str(obj)}", end="")
        print()
    client.subscribe(MQTT_TOPIC)
    client.on_message = on_message


## @brief Connects a client to the broker and subscribes to a given topic
def main() -> None:
    """Connects a client to the broker and subscribes to a given topic
    """
    with sqlite3.connect(DB_PATH) as con:
        client = connect_mqtt()
        subscribe(client, con)
        client.loop_forever()


if __name__ == '__main__':
    main()
