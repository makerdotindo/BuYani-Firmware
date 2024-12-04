#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ConfiguratorMKR.h>
#include <OverTheAir.h>


// to use these templates, you need to define the following variables in the void setup() function
// and also every variable that you want to use as underlaying of your program to syncronize with the server
// to keep the fimrware version and hardware version match with the server
// u need to define the FIRMWARE_VERSION and HARDWARE_VERSION in the dev_config.h file
// for the Serial Number, you can define it in the dev_config.h file or you can define it in the constructor of the ConfiguratorMKR class
// Copyrigth 2024 Makerindo, Developer: Muhamad Nur Yasin Amadudin

// to access the configurator web interface, you can access it by using the following url
// https://configurator.makerindo.co.id
// for the username and password, you can ask the administrator

ConfiguratorMKR configurator(CONFIGURATOR_HOST, true, DEVICE_ID);
OverTheAir ota;

void syncLoop();
void syncronize();

unsigned long long lastSync = 0;
unsigned long long secondStamp = 0;
unsigned long long lastSecondStamp = 0;
int counterSecond = 0;

/**
 * @brief Initializes the setup for the program.
 * 
 * This function sets up the serial communication and connects to WiFi.
 * It also initializes the configurator and adds variables to the params.
 * Finally, it synchronizes the program.
 */
void setup()
{

  Serial.begin(115200);

  Serial.println("Connecting to WiFi..");
  WiFi.begin("SETTING", "00001111");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  configurator.init();

  // add variables the params
  configurator.addVariable("HOST_SERVER", "192.168.1.3");
  configurator.addVariable("CALIBRATION_SENSOR_A", "200");
  configurator.addVariable("CALIBRATION_SENSOR_B", "100");
  configurator.addVariable("SYNC_INTERVAL", "30");

  syncronize();

  // put your setup code here, to run once:

  // example of getting the variable value
  // String hostServer = configurator.getVariable("HOST_SERVER");

  // example of updating the variable value
  // configurator.updateVariable("HOST_SERVER", "192.168.1");

  // example of removing the variable
  // configurator.removeVariable("HOST_SERVER");

  // example of loading the variables
  // configurator.loadVariables();

  // example of clearing the variables
  // configurator.clearVariables();
}

/**
 * @brief The main loop function.
 *
 * This function is responsible for executing the main code of the program.
 * It should be called repeatedly to ensure continuous execution of the program.
 * Any necessary synchronization or other operations should be performed within this function.
 */
void loop()
{
  // Main code goes here

  syncLoop();
}

/**
 * @brief Synchronizes the loop based on a specified interval.
 * 
 * This function checks if a specified interval has passed and performs synchronization if necessary.
 * It increments a counter every second and when the counter reaches 5, it checks if the sync interval
 * condition is satisfied. If the condition is met, it performs synchronization and resets the counter.
 * 
 * @note This function relies on the `configurator` object to retrieve the sync interval and perform synchronization.
 * 
 * @see configurator
 */
void syncLoop()
{
  secondStamp = millis() / 1000;
  if (secondStamp != lastSecondStamp)
  {
    lastSecondStamp = secondStamp;
    counterSecond++;
  }

  // sync every 5 seconds and if the SYNC_INTERVAL is satisfied
  if (counterSecond >= 5)
  {
    int syncInterval = configurator.getVariable("SYNC_INTERVAL").toInt();
    if (configurator.syncEvery(syncInterval > 10 ? syncInterval : 300))
    {
      lastSync = millis();
      lastSecondStamp = secondStamp;
      syncronize();
    }
    counterSecond = 0;
  }
}

/**
 * @brief Synchronizes the device with the server.
 * 
 * This function will be called every x seconds to synchronize the device with the server.
 * You can put your code here to perform the synchronization.
 * 
 * If the WiFi is connected, it checks if the server is reachable by pinging it.
 * If the server is reachable, it checks for firmware updates using the firmware version and hardware version.
 * If there are firmware updates available, it updates the firmware.
 * After the firmware update, it performs additional actions.
 * Finally, it synchronizes the variables with the server.
 * 
 * @note This function assumes the existence of the following objects: WiFi, configurator, and ota.
 * 
 * @note The server URL and API endpoints used in this function are placeholders and should be replaced with the actual server information.
 */
void syncronize()
{
  // syncronize the device with the server
  // this function will be called every x seconds
  // you can put your code here
  // do something

  if (WiFi.status() == WL_CONNECTED)
  {

    // check if the server is reachable
    bool res = configurator.pingServer("/api/device-sync/ping");

    if (res)
    {
      // check for firmware updates

      res = configurator.syncFirmware(FIRMWARE_VERSION, HARDWARE_VERSION, "/api/device-sync/firmware-version");

      if (!res)
      {
        // update firmware
        // res = configurator.updateFirmware("/api/device-sync/firmware-update");
        res = ota.update("configurator.makerindo.co.id", 443, (String) "/api/device-sync/firmware?serial_number=" + "PIM-080824-0001", true);

        if (res)
        {
          // do something after firmware update
          ESP.restart();
        }
      }

      // sync variables
      res = configurator.syncVariables("/api/device-sync/check");
    }
    else
    {
      Serial.println("Server not reachable");
    }
  }
}