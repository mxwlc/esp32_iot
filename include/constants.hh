/**
 * @file constants.hh
 * @author Corey Maxwell (corey.maxwell18@proton.me)
 * @brief Useful constants for Main Program
 * @version 0.1
 * @date 2024-08-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <map>
#include <Arduino.h>

/**
 * @brief The name of the Board
 */
String const DEVICE_NAME = "ESP32-devkit-V4";
/**
 * @brief The name of the sensor
 */
String const SENSOR_NAME = "RANDOM WALK";
/**
 * @brief The numer of sensors connected to the board
 */
int const NUMBER_OF_SENSORS  = 1;

/**
 * @brief The Delay (in ms) before the ESP32 sends out and identification packet 
 */
const unsigned long TIMER_DELAY_ID = 3600000;
/**
 * @brief The delay (in ms) before the ESP32 send out Data Readings
 */
const unsigned long TIMER_DELAY_READINGS = 10000;
/**
 * @brief Map containing the MQTT Topics:
 */
std::map<String, const char *> MQTT_TOPICS = {std::pair<String, const char *>(String("readings"), "esp32/walk"),
                                         std::pair<String, const char *>(String("id"), "esp32/id")};

