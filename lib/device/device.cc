/**
 * @file device.cc
 * @author Corey Maxwell (corey.maxwell18@proton.me)
 * @brief Function declaration for device header
 * @version 0.1
 * @date 2024-08-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "device.hh"

/**
 * @brief Namespace containing functions that allow for th conversion of data into JSON Objects
 */
namespace device
{
Parser::~Parser()
{
}
// Device

Device::Device(String _mac_address, String _device_type)
{
    device_address = _mac_address;
    device_name = _device_type;
    sensor_vector = {};
}

Device::Device(String _mac_address, String _device_type, std::vector<Sensor> _sensor_vector)
{
    device_address = _mac_address;
    device_name = _device_type;
    sensor_vector = _sensor_vector;
}

Device::~Device() {};

String Device::get_address()
{
    return device_address;
}

JsonDocument Device::parse()
{
    JsonDocument doc;
    JsonDocument sensorDoc;
    doc["packet_type"] = "Device";
    doc["device_address"] = device_address;
    doc["device_type"] = device_name;
    for (auto sensor : sensor_vector)
    {
        sensorDoc.add(sensor.parse());
    }
    doc["sensors"] = sensorDoc;
    return doc;
}

void Device::add_sensor(Sensor _sensor)
{
    sensor_vector.push_back(_sensor);
}

// Sensor
Sensor::Sensor(String _sensor_name, String _sensor_description, String _unit, String _sensor_addr)
{
    sensor_address = _sensor_addr;
    sensor_name = _sensor_name;
    sensor_description = _sensor_description;
    unit = _unit;
}

String Sensor::get_address()
{
    return sensor_address;
}
Sensor::~Sensor()
{
}

JsonDocument Sensor::parse()
{
    JsonDocument doc;
    // JsonDocument inner_doc;
    doc["sensor_address"] = sensor_address;
    doc["sensor_name"] = sensor_name;
    doc["sensor_description"] = sensor_description;
    doc["unit"] = unit;
    // doc[sensor_addr] = inner_doc;
    return doc;
}

// SensorReading
SensorReading::SensorReading(double _data_value, String _sensor_address)
{
    data_value = _data_value;
    sensor_address = _sensor_address;
}

SensorReading::~SensorReading()
{
}

JsonDocument SensorReading::parse()
{
    JsonDocument doc;
    doc["packet_type"] = "SensorReading";
    doc["sensor_address"] = sensor_address;
    doc["data_value"] = data_value;
    return doc;
}

String SensorReading::get_address()
{
    return sensor_address;
}
}; // namespace device