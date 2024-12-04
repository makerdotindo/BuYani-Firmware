#include <Arduino.h>

#define CONFIGURATOR_HOST "configurator.makerindo.co.id"

// MACRO
#define WIFI_SET_AUTOCONNECT 0
#define WIFI_SET_ONDEMAND 1
#define JSON_SIZE 3800
#define DELAY_LCD_UPDATE 50
#define QUEUE_SIZE_ITEMS 10
#define MINUTE 60

#define DEVICE_NUMBER 11

#define DEVICE_FIRMWARE_VERSION "2.6.3"



#if DEVICE_NUMBER == 10
#define DEVICE_ID "NF10"
#define MQTT_ID "NUTRIFERADS_NF10"
#define MQTT_SERVER "ferads.my.id"
#define MQTT_TOPIC_SUBSCRIBE "ferads/NF10"
#define MQTT_TOPIC_PUBLISH "ferads"
#define WIFI_AP_NAME "NF_SETTING"
#define WIFI_AP_PASSWORD "admin1234"

// BITANIC PRO PCB
#define DHTPIN1 33
#define DHTPIN2 32
#define DHTTYPE DHT22
#define MAIN_PUMP_PIN 2
#define WATER_PUMP_PIN 5
#define NUTRIENT_PUMP_PIN 18
#define LAHAN1_PUMP_PIN 19
#define LAHAN2_PUMP_PIN 14
#define LAHAN3_PUMP_PIN 26
#define LAHAN4_PUMP_PIN 25
#define BUZZER_PIN 27
#define LED_PIN_PENYIRAMAN 12
#define LED_PIN_PEMUPUKAN 13
#elif DEVICE_NUMBER == 11
#define DEVICE_ID "NF11"
#define MQTT_ID DEVICE_ID
#define MQTT_SERVER "ferads.my.id"
#define MQTT_TOPIC_SUBSCRIBE "ferads/NF11"
#define MQTT_TOPIC_PUBLISH "ferads"
#define WIFI_AP_NAME "NF_SETTING"
#define WIFI_AP_PASSWORD "admin1234"

// PIN CONFIGURATION (BITANIC JONGGOL PCB)
#define DHTPIN1 19
#define DHTPIN2 -1
#define DHTTYPE DHT22
#define MAIN_PUMP_PIN 1
#define WATER_PUMP_PIN 4
#define NUTRIENT_PUMP_PIN 5
#define LAHAN1_PUMP_PIN 6
#define LAHAN2_PUMP_PIN 7
#define LAHAN3_PUMP_PIN 15
#define LAHAN4_PUMP_PIN 16
#define BUZZER_PIN 14
#define LED_PIN_PENYIRAMAN 12
#define LED_PIN_PEMUPUKAN 13
#define SDA_1 8
#define SCL_1 9
#endif

// #define DEVICE_ID "NF09"

// MQTT broker ferads host
// mqtt://ferads.my.id:1883

// DEBUG
#define DEBUG_WIFI_MANAGER false
#define DEBUG_MQTT false


// NUTRIFERADS PCB
// #define DHTPIN1 33
// #define DHTPIN2 32
// #define DHTTYPE DHT22
// #define MAIN_PUMP_PIN 25
// #define WATER_PUMP_PIN 15
// #define NUTRIENT_PUMP_PIN 5
// #define LAHAN1_PUMP_PIN 18
// #define LAHAN2_PUMP_PIN  19
// #define LAHAN3_PUMP_PIN 12
// #define LAHAN4_PUMP_PIN 13
// #define BUZZER_PIN 27
// #define LED_PIN_PENYIRAMAN 12
// #define LED_PIN_PEMUPUKAN 13