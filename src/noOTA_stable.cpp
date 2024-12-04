// #include <Arduino.h>
// #include "dev_config.h"
// #include <header/connection.h>
// #include <header/storageHandler.h>
// #include <header/processSchedule.h>
// #include <header/displayLCDHandler.h>
// #define _TASK_SLEEP_ON_IDLE_RUN
// #include <TaskScheduler.h>
// #include <SPI.h>
// #include <OneButton.h>

// // #define SET_LOOP_TASK_STACK_SIZE 4096

// StaticJsonDocument<JSON_SIZE> doc;
// processSchedule processor;
// Connection con;
// StorageHandler storage;
// extern TimeHandler time_h;
// extern statusHandler status_h;
// extern inputHandler input_h;
// displayLCDHandler display_h;
// Scheduler runner;

// TaskHandle_t Task1;
// TaskHandle_t Task2;

// bool stateConBefore = false;

// OneButton button(0, true);

// void Task1code(void *pvParameters);
// void Task2code(void *pvParameters);

// void restartWiFiManager();

// void setup()
// {

//   Serial.begin(115200);

//   Serial.println("Starting...");
//   status_h.setID(DEVICE_ID);
//   status_h.setFirmwareVersion(DEVICE_FIRMWARE_VERSION);
//   status_h.setActivation(true);
//   status_h.setTotalOutput(6);
//   status_h.setTotalInput(4);

//   // outputHandler mainPump(2);

//   // output.addOutput(waterPump, "waterPump");
//   // output.addOutput(NutrientPump, "NutrientPump");
//   // output.addOutput(lahan1, "lahan1");
//   // output.addOutput(lahan2, "lahan2");

//   display_h.initLCD();
//   processor.buzzerBeep(500);
//   delay(500);
//   processor.buzzerBeep(500);
//   display_h.displayWifiConnecting();

//   Serial.println("Connecting to WiFi...");
//   Serial.println(con.getSavedWifiSSID());
//   Serial.println(con.getSavedWifiPassword());
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(con.getSavedWifiSSID().c_str(), con.getSavedWifiPassword().c_str());
//   delay(1000);
//   if (WiFi.status() != WL_CONNECTED)
//   {
//     processor.buzzerBeep(1000);
//     Serial.println("WiFi failed to connect");
//     display_h.displayWifiNotConnected();
//     display_h.displayWifiManagerStarted(WIFI_AP_NAME, 60);
//     con.startWiFiManager(WIFI_AP_NAME, WIFI_AP_PASSWORD, WIFI_SET_ONDEMAND, 60);
//   }

//   if (WiFi.status() == WL_CONNECTED)
//   {
//     processor.buzzerBeep(200);
//     delay(100);
//     processor.buzzerBeep(200);
//     delay(100);
//     processor.buzzerBeep(200);
//     display_h.displayWifiConnected();
//     Serial.println("WiFi connected");
//     stateConBefore = true;
//     con.connectMQTT(MQTT_SERVER, 1883, MQTT_ID);
//     con.mqttSubscribe(MQTT_TOPIC);
//   }
//   else
//   {
//     stateConBefore = false;
//     processor.buzzerBeep(1000);
//     display_h.displayContinueWithoutWiFi();
//     Serial.println("WiFi failed to connect");
//     Serial.println("Continue without WiFi...");
//     // WiFi.disconnect();
//   }

//   display_h.initDisplayInformation();
//   // con.startWiFiManager(WIFI_AP_NAME, WIFI_AP_PASSWORD, WIFI_SET_AUTOCONNECT, 120);
//   delay(100);
//   time_h.initRTC();
//   if (WiFi.status() == WL_CONNECTED)
//   {
//     time_h.syncTime();
//   }



//   delay(100);

//   // load config
//   input_h.initSensor();

//   doc = storage.loadConfig();
//   status_h.updateFromLoadConfig(doc);

//   // xTaskCreatePinnedToCore(
//   //     Task1code, /* Task function. */
//   //     "Task1",   /* name of task. */
//   //     4096,      /* Stack size of task */
//   //     NULL,      /* parameter of the task */
//   //     1,         /* priority of the task */
//   //     &Task1,    /* Task handle to keep track of created task */
//   //     1);

//   // xTaskCreatePinnedToCore(
//   //     Task2code, /* Task function. */
//   //     "Task2",   /* name of task. */
//   //     11000,     /* Stack size of task */
//   //     NULL,      /* parameter of the task */
//   //     1,         /* priority of the task */
//   //     &Task2,    /* Task handle to keep track of created task */
//   //     1);

//   // runner.startNow();
//   // runner.addTask(taskConnection);
//   // taskConnection.enable();
//   Serial.println(getArduinoLoopTaskStackSize());
//   Serial.println(getCpuFrequencyMhz());

//   // setCpuFrequencyMhz(80);
//   // getCpuFrequencyMhz();
//   // Stacksiz
//   // CONFIG_ARDUINO_LOOP_STACK_SIZE
//   Serial.println("Config loaded");
//   button.attachClick(restartWiFiManager);


// }

// unsigned long lastTime = 0;


// void loop()
// {

//   if (millis() - lastTime > 500)
//   {
//     time_h.getSourceTime();
//     lastTime = millis();
//   }
  



//   // Serial.println("Test Point 1");
//   display_h.displayClock(time_h.getDate(), time_h.getTimeHourMinute());
//   // Serial.println("Test Point 2");
//   display_h.displayConnectionStatus(con.isConnected() ? "+" : "-");
//   // Serial.println("Test Point 3");
//   display_h.displaySuhu(String(input_h.getDHT1Temp(), 1));
//   // Serial.println("Test Point 4");
//   display_h.displayKelembaban(String(input_h.getDHT1Hum(), 1));
//   // Serial.println("Test Point 5");
//   display_h.displayTipe(status_h.getTipeTerdekatHariIni());
//   // Serial.println("Test Point 6");
//   display_h.displayStatusPendjadwalan(status_h.getPenjadwalanOn() ? "+" : "-");
//   // Serial.println("Test Point 7");
//   display_h.displayWaktuPendjadwalanTerdekat(status_h.getWaktuPendajwalanTerdekatHariIni());
//   // Serial.println("Test Point 8");
//   display_h.displayLahan1(status_h.getLahan1() ? "+" : "-");
//   // Serial.println("Test Point 9");
//   display_h.displayLahan2(status_h.getLahan2() ? "+" : "-");
//   // Serial.println("Test Point 10");
//   // display_h.displayLahan3(status_h.getLahan3() ? "+" : "-");
//   // display_h.displayLahan4(status_h.getLahan4() ? "+" : "-");
//   display_h.setInit(false);
//   // Serial.println("Test Point 11");

//   button.tick();
//   // Serial.println("Test Point 12");

//   // save config
//   String temp = con.getLastMessage();
//   if (temp != "")
//   {
//     deserializeJson(doc, temp);
//     // serializeJson(doc, temp);
//     // Serial.println(temp);
//     Serial.println(doc["mode"].as<String>());
//     if (doc["mode"].as<String>() == "auto")
//     {
//       storage.saveConfig(doc);
//       delay(100);
//       doc.clear();
//       doc = storage.loadConfig();
//       String temp;
//       serializeJson(doc, temp);
//       // Serial.println(temp);
//       status_h.updateFromLoadConfig(doc);
//       status_h.setIsStatusChanged(true);
//       processor.resetQueue();

//       processor.buzzerBeep(500);
//     }
//     else if (doc["mode"].as<String>() == "manual")
//     {
//       processor.buzzerBeep(500);

//       // Todo: manual mode
//       processor.processManualMode(doc);
//       doc.clear();

//       doc = storage.loadConfig();
//       /* code */
//     }
//   }

//   if (processor.getIsSeconds2(120))
//   {
//     Serial.println("Reinit LCD");
//     display_h.reinitLCD();
//   }

//   if (processor.getIsSeconds(10))
//   {
//     if (con.isMqttConnected())
//     {
//       Serial.println("Sending Data...");
//       String temp;

//       serializeJson(status_h.getStatusJSON(), temp);
//       con.mqttPublish(MQTT_TOPIC_PUBLISH, temp);
//       time_h.updateTime();
//     }
    
//     if (WiFi.status() != WL_CONNECTED)
//     {
//       Serial.println("WiFi Not Connected, Try to reconnecting");
//       con.reconnectWiFi();
//     }
    
//   }

//   if (status_h.isStatusChanged())
//   {
//     processor.resetCounter();
//     if (con.isMqttConnected())
//     {
//       Serial.println("Sending Data Immedietly...");

//       String temp;

//       serializeJson(status_h.getStatusJSON(), temp);
//       con.mqttPublish(MQTT_TOPIC_PUBLISH, temp);
//     }
//     else
//     {
//       Serial.println("internet not connected, Waiting Connecion...");
//     }
//   }

//   if (processor.getIsSecondsBeat())
//   {
//     // time_h.getSourceTime();
//     display_h.updateSecondsTick();
    
//     con.connectionLoopMQTT();
//   }

//   processor.processAutoMode(doc);
//   // runner.execute();
//   con.connectionLoop();

//   if (con.isConnected() != stateConBefore)
//   {
//     stateConBefore = con.isConnected();
//     if (stateConBefore)
//     {
//       processor.buzzerBeep(200);
//       delay(100);
//       processor.buzzerBeep(200);
//       delay(100);
//       processor.buzzerBeep(200);
//     }
//     else
//     {
//       processor.buzzerBeep(1000);
//     }
//   }
// }

// // void Task2code(void *pvParameters)
// // {
// //   Serial.print("Task2 running on core ");
// //   Serial.println(xPortGetCoreID());

// //   for (;;)
// //   {

// //   }
// // }

// void restartWiFiManager()
// {
//   con.startWiFiManager(WIFI_AP_NAME, WIFI_AP_PASSWORD, WIFI_SET_ONDEMAND, 60);
// }

// // Task1code: blinks an LED every 1000 ms
// // void Task1code(void *pvParameters)
// // {
// //   Serial.print("Task1 running on core ");
// //   Serial.println(xPortGetCoreID());

// //   for (;;)
// //   {

// //   }
// // }
