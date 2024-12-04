#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <dev_config.h>
#include <vector>
#include <utility>
#include "StorageHandler.h"
#include <string>

#define MS_TO_S 1000


/**
 * @class ConfiguratorMKR
 * @brief A class for configuring and managing a MKR device.
 * 
 * The ConfiguratorMKR class provides methods for initializing the device, syncing variables, 
 * pinging the server, syncing firmware, updating firmware, adding and removing variables, 
 * loading and clearing variables, getting and updating variables, and syncing at regular intervals.
 * 
 * The class maintains information about the host, WiFi client, HTTP client, security settings, 
 * variable synchronization status, server ping status, firmware synchronization status, 
 * timestamp, serial number, and a vector of key-value pairs representing variables.
 * 
 * Usage:
 * 1. Create an instance of ConfiguratorMKR by providing the host, security flag, and serial number.
 * 2. Call the init() method to initialize the device.
 * 3. Use the various methods to perform desired operations on variables and firmware.
 * 4. Optionally, call the syncEvery() method to sync variables at regular intervals.
 * 5. Call the loop() method in the main loop of your program to handle background tasks.
 */
class ConfiguratorMKR
{
private:
    String _host;

    WiFiClientSecure client;
    HTTPClient http;

    bool isSecure = false;
    bool isVariableSynced = false;
    bool isServerPinged = false;
    bool isFirmwareSynced = false;

    bool isVariableGotUpdated = false;

    unsigned long long timestamp;

    String serialNumber;

    bool isClientConnected;

    std::vector<std::pair<String, String>> variables;

    String _convertToUrl(String host, String endpoint, String params[]);
    std::pair<String, String> PAIR1;

    std::pair<String, String> _parseVariable(String variable);
    String _deparseVariable(std::pair<String, String> variable);

public:
    ConfiguratorMKR(String host, bool isSecure);

    bool init();
    bool mountLittleFS();

    bool syncVariables(String endpoint);
    bool pingServer(String endpoint);
    bool syncFirmware(String firmwareVersion, String hardwareVersion, String endpoint);
    bool updateFirmware(String endpoint);

    bool addVariable(String key, String value);
    bool removeVariable(String key);
    bool loadVariables();
    bool clearVariables();
    String getVariable(String key);
    bool updateVariable(String key, String value);

    bool writeSerialNumber(String value);
    bool loadSerialNumber();
    String getSerialNumber();

    bool getIsVariableGotUpdated();


    bool syncEvery(unsigned long long intervalSeconds);

    void loop();
};