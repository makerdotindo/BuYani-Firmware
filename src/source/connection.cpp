#include "header/connection.h"
#include "EEPROM.h"
#include <ConfiguratorMKR.h>

extern ConfiguratorMKR configurator;

/* Core where the task should run */

String Connection::dataMsg = "";

void Connection::reconnectWiFi()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    // delay(1000);
    WiFi.mode(WIFI_STA);
    WiFi.begin(this->getSavedWifiSSID().c_str(), this->getSavedWifiPassword().c_str());
    Serial.println("SSID: " + this->saved_wifi_ssid + " Password: " + this->saved_wifi_password);
    // delay(1000);
  }
}

Connection::Connection()
{
  this->wifi_connected = false;
  this->mqtt_connected = false;
}

String Connection::getSavedWifiSSID()
{
  EEPROM.begin(4000);
  this->saved_wifi_ssid = EEPROM.readString(2000);
  EEPROM.end();

  return this->saved_wifi_ssid;
}

String Connection::getSavedWifiPassword()
{
  EEPROM.begin(4000);
  this->saved_wifi_password = EEPROM.readString(2050);
  EEPROM.end();
  return this->saved_wifi_password;
}

String Connection::getLastMessage()
{
  String temp;
  // this->dataMsg;
  if (this->dataMsg != "")
  {
    temp = this->dataMsg;
    this->dataMsg = "";
    this->client.flush();
    return temp;
  }
  else
  {
    return "";
  }

  return "";
}

bool Connection::isMqttConnected()
{
  if (client.connected())
  {
    return true;
  }

  return false;
}

bool Connection::isConnected()
{
  return this->wifi_connected && this->mqtt_connected;
}

void Connection::connectionLoopMQTT()
{
  if (!this->client.connected() && WiFi.status() == WL_CONNECTED)
  {
    this->mqttReconnect();
  }
}

void Connection::connectionLoop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    this->wifi_connected = true;
  }
  else
  {

    this->wifi_connected = false;
  }

  if (client.connected())
  {
    this->mqtt_connected = true;
  }
  else
  {
    this->mqtt_connected = false;
  }

  if (this->client.connected() && WiFi.status() == WL_CONNECTED)
  {
    this->client.loop();
  }

  // if (!this->isWMStarted)
  // {
  //   if (WiFi.status() != WL_CONNECTED)
  //   {
  //     delay(3000);
  //     wm.setConfigPortalTimeout(10);
  //     Serial.println("WiFi not connected, Reconnecting...");
  //     wm.autoConnect(this->AP_Name.c_str(), this->AP_Password.c_str());
  //   }
  // }
}

void Connection::disconnect()
{
  this->client.disconnect();
  this->wifi_connected = false;
  this->mqtt_connected = false;
}

void Connection::mqttCallback(char *topic, byte *payload, unsigned int length)
{
#ifdef DEBUG_MQTT
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  // for (int i = 0; i < length; i++)
  // {
  //   Serial.print((char)payload[i]);
  // }
  // Serial.println();
#endif

  for (int i = 0; i < length; i++)
  {
    dataMsg += (char)payload[i];
  }
}

void Connection::mqttReconnect()
{
  // Loop until we're reconnected

  // if (!this->client.connected() && WiFi.status() == WL_CONNECTED)
  // {
  //   this->mqttReconnect();
  // }

  if (!this->client.connected() && WiFi.status() == WL_CONNECTED)
  {
#ifdef DEBUG_MQTT
    Serial.print("Attempting MQTT connection...");
#endif
    // Attempt to connect
    this->client.setClient(this->espClient);
    this->client.setServer(MQTT_SERVER, 1883);
    this->client.setCallback(this->mqttCallback);
    this->client.setBufferSize(16096);
    if (this->client.connect(this->id_mqtt.c_str()))
    {
#ifdef DEBUG_MQTT
      Serial.println("connected");
#endif
      // Once connected, publish an announcement...
      // this->client.publish(this->last_topic_published.c_str(), "hello world");
      // ... and resubscribe
      this->client.subscribe(configurator.getVariable("mqtt_topic_sub").c_str());
      // client.flush();
      // con.mqttSubscribe(configurator.getVariable("mqtt_topic_sub"));

      Serial.println("Subscribed");
    }
    else
    {
#ifdef DEBUG_MQTT
      Serial.print("failed, rc=");
      Serial.print(this->client.state());
      Serial.println(" try again in 5 seconds");
#endif
      this->counterAttempting++;

      if (counterAttempting > 10)
      {
        WiFi.disconnect();
        counterAttempting = 0;
        counterAttempting2++;
      }

      if (counterAttempting2 > 3)
      {
        // restart the esp
        counterAttempting2 = 0;
        Serial.println("Restarting ESP Because MQTT Connection Failed");
        ESP.restart();
      }

      // Wait 5 seconds before retrying
      // delay(5000);
    }
  }
}

void Connection::mqttSubscribe(String topic)
{
  this->last_topic_subscribed = topic;
  this->client.subscribe(topic.c_str());
}

void Connection::mqttPublish(String topic, String msg)
{
  this->last_topic_published = topic;
  this->client.publish(topic.c_str(), msg.c_str());
}

void Connection::connectMQTT(String server, int port, String id, String user, String password)
{
  this->mqttServer = server;
  this->id_mqtt = id;
  this->client.setClient(this->espClient);
  this->client.setServer(mqttServer.c_str(), port);
  this->client.setCallback(this->mqttCallback);
  this->client.setBufferSize(16096);
  this->mqtt_connected = true;
  if (!this->client.connect(this->id_mqtt.c_str()))
  {
#ifdef DEBUG_MQTT
    Serial.println("MQTT not connected, Reconecting...");
#endif
    this->mqttReconnect();
  }
  // this->client.setCredentials(user.c_str(), password.c_str());
}

void Connection::startWiFiManager(String APName, String APPassword, int Mode, int timeout)
{
  bool res;

  this->AP_Name = APName;
  this->AP_Password = APPassword;

#ifdef DEBUG_WIFI_MANAGER
  Serial.println("WiFiManager Started");
#endif

  wm.setConfigPortalTimeout(timeout);
  if (Mode == WIFI_SET_ONDEMAND)
  {
    // Settingan selalu menyalakan WiFi Manager Ketika Menyala
    res = wm.startConfigPortal(this->AP_Name.c_str(), this->AP_Password.c_str());
  }
  else if (Mode == WIFI_SET_AUTOCONNECT)
  {
    // Settingngan autoconnect kalau pernah konek ke wifi sebelumnya jadi ngga akan nyalain WiFi Manager
    res = wm.autoConnect(this->AP_Name.c_str(), this->AP_Password.c_str()); // password protected ap
  }

  if (!res)
  {
#ifdef DEBUG_WIFI_MANAGER
    Serial.println("Failed to connect");
#endif
    // ESP.restart();
  }
  else
  {
// if you get here you have connected to the WiFi
#ifdef DEBUG_WIFI_MANAGER
    this->saved_wifi_ssid = wm.getWiFiSSID();
    this->saved_wifi_password = wm.getWiFiPass();
    Serial.println("connected...yeey :)");
#endif
    Serial.println("WiFi connected");
    Serial.println("Local IP");
    Serial.println(WiFi.localIP());

    EEPROM.begin(4000);
    EEPROM.writeString(2000, wm.getWiFiSSID());
    EEPROM.writeString(2050, wm.getWiFiPass());
    EEPROM.commit();
    EEPROM.end();
    this->wifi_connected = true;
  }

#ifdef DEBUG_WIFI_MANAGER

#endif
}
