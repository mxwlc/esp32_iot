/**
 * @file device.hpp
 * @author Corey Maxwell (corey.maxwell18@proton.me)
 * @brief Classes implementing logic for parsing Sensor Reading and ID Data into a JSON object
 * @version 0.1
 * @date 2024-08-28
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef DEVICE_HH
#define DEVICE_HH
#pragma once

#include <ArduinoJson.h>
#include <map>
#include <vector>

namespace device
{

// Parser Abstract
/**
 * @brief Abstract Class Implementing Functions for Parsing Readings into JSON Objects
 */
class Parser
{
  public:
    virtual ~Parser();
    /**
     * @brief  Encodes data readings/id object into a JSON object
     * @return JsonDocument Object
     */
    virtual JsonDocument parse() = 0;
    /**
     * @brief Returns the IP address of the Board
     * @return Arduino String object containing the IP Address of the
     */
    virtual String get_address() = 0;
};

// Sensor inherits Parser Public
/**
 * @brief Class encapsulating logic for handling sensors connected to the Board
 */
class Sensor : public Parser
{
  public:
    /**
     * @brief Sensor Constructor
     * @param _sensor_name Name of sensor
     * @param _sensor_description Short Description of Sensor
     * @param _unit Unit of the sensor readings
     * @param _sensor_address the address of the sensor the board
     */
    Sensor(String _sensor_name, String _sensor_description, String _unit, String _sensor_address);
    ~Sensor();
    JsonDocument parse();
    String get_address();

  private:
    /**
     * @brief Onboard address of the sensor, of the form XX\:XX\:XX\:XX\:XX\:XX\:00-FF where the Xs represent the Mac
     * address of the device and the final 8 bits are sequientially assigned to each address.
     */
    String sensor_address;
    String sensor_name;
    String sensor_description;
    String unit;
};

// Device inherits Parser Public
class Device : public Parser
{
  public:
    /**
     * @brief
     * @param _mac_address MAC Address of the Network interface on the Board
     * @param _device_type Name of the Device
     */
    Device(String _mac_address, String _device_type);
    /**
     * @brief Device Constructor
     * @param _mac_address MAC Address of the Network interface on the Board
     * @param _device_type Name of the Device
     * @param _sensor_vector A Vector containing Sensor Objects
     */
    Device(String _mac_address, String _device_type, std::vector<Sensor> _sensor_vector);
    ~Device();
    JsonDocument parse();
    String get_address();
    /**
     * @brief Adds A sensor object to the internal sensor vector
     * @param _sensor Sensor Object
     */
    void add_sensor(Sensor _sensor);

  private:
    String device_address;
    String device_name;
    std::vector<Sensor> sensor_vector;
};

// Reading inherits Parser Public
/**
 * @brief Class encoding logic for encoding Sensor Readings into a Json oject
 */
class SensorReading : public Parser
{
  public:
    /**
     * @brief Constructor for SensorReading
     * @param _data_value Reading Value
     * @param _sensor_address Address of the Sensor
     */
    SensorReading(double _data_value, String _sensor_address);
    JsonDocument parse();
    ~SensorReading();
    String get_address();

  private:
    String sensor_address;
    double data_value;
};
}; // namespace device
#endif
