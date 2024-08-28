"""!@brief Inserts MQTT packet To Sqlite Database
"""
## @package device_writer
# @file device_writer.py
# @author Corey Maxwell (corey.maxwell18@proton.me)
# @version 0.1
# @date 2024-08-28
# @copyright Copyright (c) 2024
#
from __future__ import annotations

import sqlite3

## @brief Abstract Class containing basic functions for inserting JSON objects ito a database
#
class Writer:
    """Writer Abstract Class"""

    ## @brief Returns an SQL insert query as a str
    # @return object encoded into SQL Query
    #
    def __str__(self) -> str:
        """Returns an SQL insert query as a str"""
        return self.sql_query()

    ## @brief Returns The Object encoded into an "INSERT" SQL statement
	# @return INSERT Statement
    def sql_query(self) -> str:
        """Returns The Object encoded into an "INSERT" SQL statement"""
        return """"""

    ## @brief Inserts the object into the provided database
    # @param Opened Sqlite connection
    def insert(self, db_connection: sqlite3.Connection) -> None:
        """Inserts the object into the provided database
        """
        sql = self.sql_query()
        cur = db_connection.cursor()
        cur.execute(sql)
        db_connection.commit()


## @brief class representing an IoT device extends Writer
#
class DeviceWriter(Writer):
    """A class representing an IoT device"""

    ## @brief Returns the MAC Address of the Device class
    # @return MAC Adress of device
    def address(self) -> str:
        """ Returns the MAC Address of the Device class"""
        return self._device_address

    ## @brief Device Constructor
    # @param device_address  MAC address of the device
    # @param device_type Type of the device
    def __init__(self, device_address: str, device_type: str) -> None:
        """Device Constructor"""
        self._device_address = device_address
        self._device_type = device_type

    def sql_query(self) -> str:
        template = f'''
        INSERT OR IGNORE INTO Devices(device_address, device_type)
        VALUES ("{self._device_address}", "{self._device_type}");'''
        return template

    ## @var _device_address
    # mac address of the device
    ## @var _device_type
    # 	Type of the device

## @brief Class represensting a Sensor Object. Used to update the database as well as creating readings objects that keeps track of the MAC Address of the device the sensor is attached to aswell as the individual Sensor Address.
#
class SensorWriter(Writer):
    """Class represensting a Sensor Object used to update the DB and to create readings objects"""

    ## @brief Constructor for the Sensor
    # @param sensor_address Address of the sensor connected to a device
    # @param device Device object that the sensor is connected to
    # @param sensor_name Sensor name to identify object
    # @param sensor_description Brief description of sensor
    # @param unit Unit of readings
    #
    def __init__(self, sensor_address: str, device: DeviceWriter, sensor_name: str, sensor_description: str, unit: str) -> None:
        """Constructor for the Sensor"""
        self._sensor_address = sensor_address
        self._device = device
        self._sensor_name = sensor_name
        self._sensor_description = sensor_description
        self._unit = unit

    def address(self) -> str:
        return self._sensor_address

    def sql_query(self) -> str:
        template = f"""INSERT OR IGNORE INTO Sensors(sensor_address, device_address, sensor_name, sensor_description, unit)
                        VALUES ("{self._sensor_address}", "{self._device.address()}",
                                "{self._sensor_name}", "{self._sensor_description}", "{self._unit}");
                    """
        return template

    ## @brief Creates a readings object
    # @param data_value Data value recieved
    # @param type data_value float
    # @return Returns readings Object
    #
    def create_readings(self, data_value:  float) -> ReadingsWriter:
        """Creates a readings object

        :param data_value: Data value recieved
        :type data_value: float
        @return Returns 
        """
        return ReadingsWriter(self, data_value)

        ## @var _sensor_address
        # Address of the sensor connected to a device
        ## @var _device
        # Device object that the sensor is connected to
        ## @var _sensor_name
        # Sensor name to identify object
        ## @var _sensor_description
        # Brief description of sensor
        ## @var _unit
        # Unit of readings


## @brief Class Representing readings recieved on a topic
#
class ReadingsWriter(Writer):
    """Class Representing readings recieved on a topic"""
    
    ## @brief ReadingsWriter Constructor
    # @param sensor Sensor that creates the readings object
    # @param data_value Sensor Value
    #
    def __init__(self, sensor: SensorWriter, data_value: float) -> None:
        """ReadingsWriter Constructor"""
        self._sensor = sensor
        self._data_value = data_value

    def sql_query(self) -> str:
        template = f"""INSERT INTO Readings(sensor_address, data_value)
                        VALUES ("{self._sensor.address()}", "{self._data_value}");
        """
        return template
    ## @var _sensor 
    # 	The Sensor that the readings are given by
	## @var _data_value 
 	# 	Data value recieved from sensor
	

## @brief DEBUG FOR TESTING
def main():

    try:
        with sqlite3.connect('test.db') as con:
            dev1 = DeviceWriter("AA:AA:AA:AA:AA:BB", "ESP32-2c3")
            dev2 = DeviceWriter("AA:AA:AA:AA:AA:BB", "ESP32")
            dev1.insert(con)
            dev2.insert(con)
            sens1 = SensorWriter("AA:AA:AA:AA:AA:BB:01",
                                 dev1, "Test", "Test Desc", "cm")
            read = sens1.create_readings("1.0")
            sens1.insert(con)
            read.insert(con)
    except sqlite3.Error as e:
        print(e)


if __name__ == "__main__":
    main()
