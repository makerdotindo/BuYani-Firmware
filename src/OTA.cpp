#include <Arduino.h>
#include "dev_config.h"
#include <header/connection.h>
#include <header/StorageHandler_EEPROM.h>
#include <header/processSchedule.h>
#include <header/displayLCDHandler.h>
#define _TASK_SLEEP_ON_IDLE_RUN
#include <TaskScheduler.h>
#include <SPI.h>
#include <OneButton.h>
#include <OverTheAir.h>
#include <ConfiguratorMKR.h>
#include "esp_task_wdt.h"

ConfiguratorMKR configurator(CONFIGURATOR_HOST, true);


extern outputHandler mainPump;
extern outputHandler waterPump;
extern outputHandler nutrientPump;
extern outputHandler lahan1;
extern outputHandler lahan2;
extern outputHandler lahan3;
extern outputHandler lahan4;
extern outputHandler buzzer;
extern outputHandler ledPenyiraman;
extern outputHandler ledPemupukan;

// #define SET_LOOP_TASK_STACK_SIZE 4096

StaticJsonDocument<JSON_SIZE> doc;
processSchedule processor;
Connection con;
StorageHandler_EEPROM storage;
extern TimeHandler time_h;
extern statusHandler status_h;
extern inputHandler input_h;
displayLCDHandler display_h;
Scheduler runner;
OverTheAir ota;

TaskHandle_t Task1;
TaskHandle_t Task2;

bool stateConBefore = false;

OneButton button(0, true);

unsigned long long lastSync = 0;
unsigned long long secondStamp = 0;
unsigned long long lastSecondStamp = 0;
int counterSecond = 0;

extern outputHandler lahan2;

void Task1code(void *pvParameters);
void checkMem();

void restartWiFiManager();

void syncronize();

void syncLoop();

void setup()
{
  Serial.begin(115200);


  // delay(2000);

  configurator.mountLittleFS();

  // this is only for the first time flash of the device after that it will be commented
  configurator.writeSerialNumber("NF-111124-V1.0-0001"); // NF11

  // configurator.clearVariables();
  configurator.loadSerialNumber();

  configurator.addVariable("id", DEVICE_ID);
  configurator.addVariable("mqtt_serv", MQTT_SERVER);
  configurator.addVariable("mqtt_topic_pub", MQTT_TOPIC_PUBLISH);
  configurator.addVariable("mqtt_topic_sub", MQTT_TOPIC_SUBSCRIBE);
  configurator.addVariable("wifi_ap_name", WIFI_AP_NAME);
  configurator.addVariable("wifi_ap_pass", WIFI_AP_PASSWORD);
  configurator.addVariable("sync_interval", DEFAULT_SYNC_INTERVAL);
  configurator.addVariable("dht1_pin", DHTPIN1);
  configurator.addVariable("dht2_pin", DHTPIN2);
  configurator.addVariable("main_pump_pin", MAIN_PUMP_PIN);
  configurator.addVariable("water_valve_pin", WATER_PUMP_PIN);
  configurator.addVariable("nutrient_valve_pin", NUTRIENT_PUMP_PIN);
  configurator.addVariable("lahan1_valve_pin", LAHAN1_PUMP_PIN);
  configurator.addVariable("lahan2_valve_pin", LAHAN2_PUMP_PIN);
  configurator.addVariable("lahan3_valve_pin", LAHAN3_PUMP_PIN);
  configurator.addVariable("lahan4_valve_pin", LAHAN4_PUMP_PIN);
  configurator.addVariable("buzzer_pin", BUZZER_PIN);
  configurator.addVariable("total_output", TOTAL_OUTPUT);

  configurator.loadVariables();

  mainPump.init(configurator.getVariable("main_pump_pin").toInt());
  waterPump.init(configurator.getVariable("water_valve_pin").toInt());
  nutrientPump.init(configurator.getVariable("nutrient_valve_pin").toInt());
  lahan1.init(configurator.getVariable("lahan1_valve_pin").toInt());
  lahan2.init(configurator.getVariable("lahan2_valve_pin").toInt());
  lahan3.init(configurator.getVariable("lahan3_valve_pin").toInt());
  lahan4.init(configurator.getVariable("lahan4_valve_pin").toInt());
  buzzer.init(configurator.getVariable("buzzer_pin").toInt());
  ledPenyiraman.init(configurator.getVariable("led_penyiraman_pin").toInt());
  ledPemupukan.init(configurator.getVariable("led_pemupukan_pin").toInt());

  lahan2.off();


  Serial.println("Starting...");
  status_h.setID(configurator.getVariable("id"));
  status_h.setFirmwareVersion(FIRMWARE_VERSION);
  status_h.setActivation(true);
  status_h.setTotalOutput(configurator.getVariable("total_output").toInt());

  status_h.setTotalInput(((String)TOTAL_INPUT).toInt());

  display_h.initLCD();
  processor.buzzerBeep(500);
  delay(500);
  processor.buzzerBeep(500);
  display_h.displayWifiConnecting();

  Serial.println("Connecting to WiFi...");
  Serial.println(con.getSavedWifiSSID());
  Serial.println(con.getSavedWifiPassword());
  WiFi.mode(WIFI_STA);
  WiFi.begin(con.getSavedWifiSSID().c_str(), con.getSavedWifiPassword().c_str());
  delay(2000);
  if (WiFi.status() != WL_CONNECTED)
  {
    processor.buzzerBeep(1000);
    Serial.println("WiFi failed to connect");
    display_h.displayWifiNotConnected();
    display_h.displayWifiManagerStarted(configurator.getVariable("wifi_ap_name"), 60);
    con.startWiFiManager(configurator.getVariable("wifi_ap_name"), configurator.getVariable("wifi_ap_pass"), WIFI_SET_ONDEMAND, 60);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    processor.buzzerBeep(200);
    delay(100);
    processor.buzzerBeep(200);
    delay(100);
    processor.buzzerBeep(200);
    display_h.displayWifiConnected();
    Serial.println("WiFi connected");
    stateConBefore = true;

    // sync
    syncronize();

    con.connectMQTT(configurator.getVariable("mqtt_serv"), 1883, configurator.getVariable("id"));
    con.mqttSubscribe(configurator.getVariable("mqtt_topic_sub"));
  }
  else
  {
    stateConBefore = false;
    processor.buzzerBeep(1000);
    display_h.displayContinueWithoutWiFi();
    Serial.println("WiFi failed to connect");
    Serial.println("Continue without WiFi...");
    // WiFi.disconnect();
  }

  configurator.loadVariables();

  Serial.println("Device ID: " + configurator.getVariable("id"));
  Serial.println("MQTT Server: " + configurator.getVariable("mqtt_serv"));
  Serial.println("MQTT Topic Pub: " + configurator.getVariable("mqtt_topic_pub"));
  Serial.println("MQTT Topic Sub: " + configurator.getVariable("mqtt_topic_sub"));
  Serial.println("WiFi AP Name: " + configurator.getVariable("wifi_ap_name"));
  Serial.println("WiFi AP Pass: " + configurator.getVariable("wifi_ap_pass"));
  Serial.println("Sync Interval: " + configurator.getVariable("sync_interval"));
  Serial.println("DHT1 Pin: " + configurator.getVariable("dht1_pin"));
  Serial.println("DHT2 Pin: " + configurator.getVariable("dht2_pin"));
  Serial.println("Main Pump Pin: " + configurator.getVariable("main_pump_pin"));
  Serial.println("Water Valve Pin: " + configurator.getVariable("water_valve_pin"));
  Serial.println("Nutrient Valve Pin: " + configurator.getVariable("nutrient_valve_pin"));
  Serial.println("Lahan1 Valve Pin: " + configurator.getVariable("lahan1_valve_pin"));
  Serial.println("Lahan2 Valve Pin: " + configurator.getVariable("lahan2_valve_pin"));
  Serial.println("Lahan3 Valve Pin: " + configurator.getVariable("lahan3_valve_pin"));
  Serial.println("Lahan4 Valve Pin: " + configurator.getVariable("lahan4_valve_pin"));
  Serial.println("Buzzer Pin: " + configurator.getVariable("buzzer_pin"));
  Serial.println("Total Output: " + configurator.getVariable("total_output"));


  display_h.initDisplayInformation();
  // con.startWiFiManager(WIFI_AP_NAME, WIFI_AP_PASSWORD, WIFI_SET_AUTOCONNECT, 120);
  delay(100);
  time_h.initRTC();
  if (WiFi.status() == WL_CONNECTED)
  {
    time_h.syncTime();
  }

  delay(100);

  // load config
  input_h.initSensor();

  doc = storage.loadConfig();
  status_h.updateFromLoadConfig(doc);

  xTaskCreatePinnedToCore(
      Task1code, /* Task function. */
      "Task1",   /* name of task. */
      10000,     /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &Task1,    /* Task handle to keep track of created task */
      1);

  // xTaskCreatePinnedToCore(
  //     Task2code, /* Task function. */
  //     "Task2",   /* name of task. */
  //     11000,     /* Stack size of task */
  //     NULL,      /* parameter of the task */
  //     1,         /* priority of the task */
  //     &Task2,    /* Task handle to keep track of created task */
  //     1);

  // runner.startNow();
  // runner.addTask(taskConnection);
  // taskConnection.enable();
  Serial.println(getArduinoLoopTaskStackSize());
  Serial.println(getCpuFrequencyMhz());

  // setCpuFrequencyMhz(80);
  // getCpuFrequencyMhz();
  // Stacksiz
  // CONFIG_ARDUINO_LOOP_STACK_SIZE
  Serial.println("Config loaded");
  button.attachClick(restartWiFiManager);
}

unsigned long lastTime = 0;

void loop()
{
  // syncLoop();
  button.tick();

  if (millis() - lastTime > 500)
  {

    time_h.getSourceTime();
    lastTime = millis();
  }

  // Serial.println("Test Point 1");
  display_h.displayClock(time_h.getDate(), time_h.getTimeHourMinute());
  // Serial.println("Test Point 2");
  display_h.displayConnectionStatus(con.isConnected() ? "+" : "-");
  // Serial.println("Test Point 3");
  display_h.displaySuhu(String(input_h.getDHT1Temp(), 1));
  // Serial.println("Test Point 4");
  display_h.displayKelembaban(String(input_h.getDHT1Hum(), 1));
  // Serial.println("Test Point 5");
  display_h.displayTipe(status_h.getTipeTerdekatHariIni());
  // Serial.println("Test Point 6");
  display_h.displayStatusPendjadwalan(status_h.getPenjadwalanOn() ? "+" : "-");
  // Serial.println("Test Point 7");
  display_h.displayWaktuPendjadwalanTerdekat(status_h.getWaktuPendajwalanTerdekatHariIni());
  // Serial.println("Test Point 8");
  display_h.displayLahan1(status_h.getLahan1() ? "+" : "-");
  // Serial.println("Test Point 9");
  display_h.displayLahan2(status_h.getLahan2() ? "+" : "-");
  // Serial.println("Test Point 10");
  display_h.displayLahan3(status_h.getLahan3() ? "+" : "-");
  display_h.displayLahan4(status_h.getLahan4() ? "+" : "-");
  display_h.setInit(false);
  // Serial.println("Test Point 11");

  
  // Serial.println("Test Point 12");

  // save config
  String temp = con.getLastMessage();
  if (temp != "")
  {
    deserializeJson(doc, temp);
    // serializeJson(doc, temp);
    // Serial.println(temp);
    Serial.println(doc["mode"].as<String>());

    if (!doc["id"].isNull())
    {
      if (doc["id"].as<String>() == DEVICE_ID)
      {
        Serial.println("ID Match");
        display_h.displayUpdating();
        bool result = ota.update("ferads.my.id", 80, "/update/firmware?id=" + (String)DEVICE_ID, false);
        if (result)
        {
          Serial.println("Update Success");
          con.mqttReconnect();
          con.mqttPublish("debug", (String)DEVICE_ID + (String) ",Update Success..,*");
          display_h.showUpdateStatus(true);
          // ledWiFi.off();
          ESP.restart();
        }
        else
        {
          Serial.println("Update Failed");
          con.mqttReconnect();
          con.mqttPublish("debug", (String)DEVICE_ID + (String) ",Update Failed..,*");

          // con.mqttPublish("bitanic/BL000001/debug", "Update Failed..");
          display_h.showUpdateStatus(false);
          display_h.reinitLCD();
        }
      }
    }
    else if (doc["mode"].as<String>() == "auto")
    {
      storage.saveConfig(doc);
      delay(100);
      doc.clear();
      doc = storage.loadConfig();
      String temp;
      serializeJson(doc, temp);
      // Serial.println(temp);
      status_h.updateFromLoadConfig(doc);
      status_h.setIsStatusChanged(true);
      processor.resetQueue();

      processor.buzzerBeep(500);
    }
    else if (doc["mode"].as<String>() == "manual")
    {
      processor.buzzerBeep(500);

      // Todo: manual mode
      processor.processManualMode(doc);
      doc.clear();

      doc = storage.loadConfig();
      /* code */
    }
  }

  if (processor.getIsSeconds2(120))
  {
    Serial.println("Reinit LCD");
    display_h.reinitLCD();
    input_h.initSensor();
  }

  if (processor.getIsSeconds(10))
  {
    if (con.isMqttConnected())
    {
      Serial.println("Sending Data...");
      String temp;

      serializeJson(status_h.getStatusJSON(), temp);
      // Serial.println(temp);
      con.mqttPublish(configurator.getVariable("mqtt_topic_pub"), temp);
      time_h.updateTime();
    }

    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("WiFi Not Connected, Try to reconnecting");
      con.reconnectWiFi();
    }
  }

  if (status_h.isStatusChanged())
  {
    processor.resetCounter();
    if (con.isMqttConnected())
    {
      Serial.println("Sending Data Immedietly...");

      String temp;

      serializeJson(status_h.getStatusJSON(), temp);
      con.mqttPublish(configurator.getVariable("mqtt_topic_pub"), temp);
    }
    else
    {
      Serial.println("internet not connected, Waiting Connecion...");
    }
  }

  if (processor.getIsSecondsBeat())
  {
    // time_h.getSourceTime();
    display_h.updateSecondsTick();

    con.connectionLoopMQTT();
  }

  processor.processAutoMode(doc);
  // runner.execute();
  con.connectionLoop();

  if (con.isConnected() != stateConBefore)
  {
    stateConBefore = con.isConnected();
    if (stateConBefore)
    {
      processor.buzzerBeep(200);
      delay(100);
      processor.buzzerBeep(200);
      delay(100);
      processor.buzzerBeep(200);
    }
    else
    {
      processor.buzzerBeep(1000);
    }
  }
}

// void Task2code(void *pvParameters)
// {
//   Serial.print("Task2 running on core ");
//   Serial.println(xPortGetCoreID());

//   for (;;)
//   {

//   }
// }

void restartWiFiManager()
{
  con.startWiFiManager(WIFI_AP_NAME, WIFI_AP_PASSWORD, WIFI_SET_ONDEMAND, 60);
}

// Task1code: blinks an LED every 1000 ms
// void Task1code(void *pvParameters)
// {
//   Serial.print("Task1 running on core ");
//   Serial.println(xPortGetCoreID());

//   for (;;)
//   {

//   }
// }

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
  configurator.init();

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
        res = ota.update("configurator.makerindo.co.id", 443, (String) "/api/device-sync/firmware?serial_number=" + configurator.getSerialNumber(), true);

        if (res)
        {
          // do something after firmware update
          ESP.restart();
        }
      }

      // sync variables
      res = configurator.syncVariables("/api/device-sync/check");

      if (configurator.getIsVariableGotUpdated())
      {
        ESP.restart();
      }
      
    }
    else
    {
      Serial.println(F("Server not reachable"));
    }
  }
  else
  {
    Serial.println(F("WiFi not connected"));
  }
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
    int syncInterval = configurator.getVariable("sync_interval").toInt();
    if (configurator.syncEvery(syncInterval > 10 ? syncInterval : 10800))
    {
      Serial.println("Syncing...");

      lastSync = millis();
      lastSecondStamp = secondStamp;
      unsigned long timeStampSync = millis();
      if (WiFi.status() == WL_CONNECTED)
      {
        syncronize();
        Serial.println("WiFi connected");
        /* code */
      }
      else
      {
        Serial.println("WiFi not connected");
      }
      
      Serial.println("Sync Time: " + (String)(millis() - timeStampSync) + " ms");
      Serial.println("Machine Time: " + (String)(millis() / 1000) + " s");
      checkMem();
    }
    counterSecond = 0;
  }
}

void Task1code(void *pvParameters)
{
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  configurator.init();

  // esp_task_wdt_delete(NULL); // Remove this task from the watchdog

  for (;;)
  {
    syncLoop();
  }
}

void checkMem()
{
  Serial.print("Free heap: ");
  Serial.println(ESP.getFreeHeap());
}