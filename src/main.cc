#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <memory>

#include <map>
#include <vector>

#include "constants.hh"
#include "credentials.hh"
#include "device.hh"
#include "random_walk.hh"
using namespace device;
using namespace random_walk;

#define ONBOARDLED 2
#define NUMBER_OF_SENSORS 1

/*
 * --- TODO ---
 *  Possibly refactor how the Device objects are stored
 *  Generate device and sensor objects on startup
 *  Refactor the commencted block at (~51)
 *   Ideas
 *   - Include in .h file -> Code still in space
 *
 *
 *
 */
 
/**
 * @brief Struct Containing The Device and sensor Names and addresses
 */
struct device_name_to_address
{
    /**
     * @brief The key value mapping the device name to its macAddress
     */
    std::pair<String, String> device_address;
    /**
     * @brief A vector containing the addresses of the sensors connected to the DEvice
     */
    std::vector<std::pair<String, String>> sensor_address_vector;
    /**
     * @brief Constructor for generating sensor Adddresses
     * @param _sensor_no The number of connected sensors
     */
    device_name_to_address(int _sensor_no)
    {
        device_address = std::pair<String, String>(DEVICE_NAME, WiFi.macAddress().c_str());
        for (int i = 0; i <= _sensor_no; i++)
        {
            String identifier = (i < 10) ? device_address.second + String(":0") + String(i)
                                         : device_address.second + String(":") + String(i);
            sensor_address_vector.push_back(std::pair<String, String>(SENSOR_NAME[i], identifier));
        }
    }
};
std::unique_ptr<struct device_name_to_address> device_info(new struct device_name_to_address(NUMBER_OF_SENSORS));

Device dv(device_info->device_address.second, device_info->device_address.first);

RandomWalk rw(15);

/* Redundant ?
 * String dev_mac = WiFi.macAddress().c_str();
 * String sens_addr_1 = dev_mac + String(":01");
 * String sens_addr_2 = dev_mac + String(":02");
 * Sensor s1 = Sensor(String("Random Walk"), String("50/50 Chance of +/- 0.1"), String("degC"), sens_addr_1);
 * Sensor s2 = Sensor(String("Random Walk"), String("50/50 Chance of +/-  0.1"), String("cm"), sens_addr_2);
 * std::vector<Sensor> sv = {s1, s2};
 * Device dv = Device(dev_mac, "ESP32", sv);
 */

WiFiClient espclient;
PubSubClient client(espclient);

// Timing Settings
unsigned long startTimeID = millis() - TIMER_DELAY_ID + 200;
unsigned long startTimeReadings = millis() - TIMER_DELAY_READINGS + 200;

/**
 * @brief Connects the ESP32 to WIFI
 */
void setup_wifi()
{
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.print(SSID);

    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("WiFi Connected");
    Serial.println(WiFi.localIP());
}
/**
 * @brief Call back function for MQTT communication
 * 
 * @param topic MQTT Topic
 * @param payload Recieved Payload
 * @param length length of payload
 */
void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

/**
 * @brief Sets up WiFi and MQTT communications
 */
void setup()
{
    // LED is another colour during setup
    // LED config
    pinMode(ONBOARDLED, OUTPUT);
    digitalWrite(ONBOARDLED, HIGH);

    // Begin Serial Communication
    Serial.begin(115200);

    // Wifi Setup -- Connects to wifi

    //  setup_wifi();

    // MQTT Client
    //  client.setServer(BROKER_IP, 1883);
    //  client.setCallback(callback);
    //  client.setBufferSize(1024);
    //  client.setKeepAlive(7200);
    //  while (!client.connected()) {
    //    String client_id = "esp32-client-";
    //    client_id += String(WiFi.macAddress());
    //    Serial.printf("The client %s connects to the public MQTT broker\n",
    //                  client_id.c_str());
    //    if (client.connect(client_id.c_str())) {
    //      Serial.println("Public EMQX MQTT broker connected");
    //    } else {
    //      Serial.print("failed with state ");
    //      Serial.print(client.state());
    //      delay(2000);
    //    }
    //  }
    digitalWrite(ONBOARDLED, LOW);
}
/**
 * @brief After a delay send An Identification packet or after another delay send sensor readings object to MQTT Broker
 *  
 */
void loop()
{
    if (millis() - startTimeID > TIMER_DELAY_ID)
    {
        startTimeID = millis();
        char buffer[1024];

        JsonDocument doc = dv.parse();
        serializeJsonPretty(doc, buffer);
        Serial.println(buffer);
        int len = strlen(buffer);
        Serial.print("[Serial] Packet length ");
        Serial.print(len);
        Serial.println(" bytes");
        client.publish(MQTT_TOPICS["id"], buffer, len);
        Serial.print("[Pub] ");
        Serial.print(buffer);
        Serial.print(" to Topic :");
        Serial.println(MQTT_TOPICS["id"]);
    }
    if (millis() - startTimeReadings > TIMER_DELAY_READINGS)
    {
        startTimeReadings = millis();
        char buffer[1024];
        SensorReading sensor_reading = SensorReading(rw.get_price(), device_info->sensor_address_vector[0].second);
        JsonDocument doc = sensor_reading.parse();
        serializeJsonPretty(doc, buffer);
        int len = strlen(buffer);
        Serial.print("[Serial] Packet length ");
        Serial.print(len);
        Serial.println(" bytes");
        client.publish(MQTT_TOPICS["readings"], buffer, len);
        Serial.print("[Pub] ");
        Serial.print(buffer);
        Serial.print(" to Topic :");
        Serial.println(MQTT_TOPICS["readings"]);
    }
    // Serial.print("[Serial] ");
    // Serial.println(rw.get_price());
    rw.iteration();
}