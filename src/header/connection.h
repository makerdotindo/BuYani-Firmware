#include "dev_config.h"
#include <PubSubClient.h>
#include <WiFiManager.h>
#include <WiFi.h>
// #include <>
class Connection
{
public:
  bool isMqttConnected();
  Connection();
  String getLastMessage();
  void startWiFiManager(String APName, String APPassword, int Mode, int timeout);
  void connectMQTT(String server, int port, String id, String user = "", String password = "");
  static void mqttCallback(char *topic, byte *payload, unsigned int length);
  void mqttSubscribe(String topic);
  void mqttPublish(String topic, String msg);
  void mqttReconnect();
  void connectionLoop();
  void connectionLoopMQTT();
  void reconnectWiFi();
  void disconnect();
  bool isConnected();
  String getSavedWifiSSID();
  String getSavedWifiPassword();
  void send(String message);
  String receive();

  int counterAttempting = 0;
  int counterAttempting2 = 0;

  static String dataMsg;

private:
  bool isWMStarted = false;

  String saved_wifi_ssid;
  String saved_wifi_password;
  String last_topic_subscribed;
  String last_topic_published;
  String id_mqtt;
  String mqttServer;
  String AP_Name;
  String AP_Password;
  bool wifi_connected;
  bool mqtt_connected;
  WiFiManager wm;
  WiFiClient espClient;
  PubSubClient client;
};