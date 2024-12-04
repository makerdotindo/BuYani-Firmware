#include <Arduino.h>

#define CONFIGURATOR_HOST "configurator.makerindo.co.id"
#define FIRMWARE_VERSION "2.8.7"
#define HARDWARE_VERSION "NF_V1.0_YD_ESP32_S3"
#define DEFAULT_SYNC_INTERVAL "30"
#define DEBUG_PROGRAM false

// MACRO
#define WIFI_SET_AUTOCONNECT 0
#define WIFI_SET_ONDEMAND 1
#define JSON_SIZE 3800
#define DELAY_LCD_UPDATE 50
#define QUEUE_SIZE_ITEMS 10
#define MINUTE 60

// DEVICE DEFAULT CONFIGURATION
#define DEVICE_SERIAL_NUMBER "NF-131124-V1.0-0001" // not actually used, this only an example

#define DEVICE_ID "NF14"
#define MQTT_SERVER "ferads.my.id"
#define MQTT_TOPIC_SUBSCRIBE "ferads/NF14"
#define MQTT_TOPIC_PUBLISH "ferads"
#define WIFI_AP_NAME "NF_Setting"
#define WIFI_AP_PASSWORD "admin1234"

#define DHTPIN1 "19"
#define DHTPIN2 "-1"
#define MAIN_PUMP_PIN "1"
#define WATER_PUMP_PIN "4"
#define NUTRIENT_PUMP_PIN "5"
#define LAHAN1_PUMP_PIN "6"
#define LAHAN2_PUMP_PIN "7"
#define LAHAN3_PUMP_PIN "15"
#define LAHAN4_PUMP_PIN "16"
#define BUZZER_PIN "14"
#define TOTAL_OUTPUT "4"
#define TOTAL_INPUT "-1"

// DEBUG
#define DEBUG_WIFI_MANAGER false
#define DEBUG_MQTT false

#if DEBUG_PROGRAM == true
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINTF(x, y) Serial.printf(x, y)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTF(x, y)
#endif