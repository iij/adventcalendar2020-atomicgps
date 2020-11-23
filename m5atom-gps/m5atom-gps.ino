#include "M5Atom.h"
#include "TinyGPS++.h"
#include <HardwareSerial.h>
#include <WiFi.h>
#include <MQTT.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <SD.h>



struct ConfigWlan {
  char  ssid[32];
  char  password[32];
};

struct ConfigMqttLocation {
  char topic[32];
};

struct ConfigMqtt {
  char  deviceId[32];
  char  server[32];
  int   port;
  char  user[32];
  char  password[32];
  ConfigMqttLocation location;
};

struct Config {
  ConfigWlan  wlan;
  ConfigMqtt  mqtt;
  int         interval;
};

const char *filename = "/config.json";
Config config;

HardwareSerial GPSRaw(2);
TinyGPSPlus gps;
WiFiClient wifiClient;
MQTTClient mqttClient;

unsigned long lastMillis = 0;
int cnt=0;


void loadConfiguration(const char *filename, Config &config) {
  File file = SD.open(filename);

  StaticJsonDocument<512> doc;

  DeserializationError error = deserializeJson(doc, file);
  if (error){
    Serial.println(F("Failed to read file, using default configuration"));
  }

  // 無線LAN SSID
  strlcpy(config.wlan.ssid,
          doc["wlan"]["ssid"] | "test",
          sizeof(config.wlan.ssid));

  // 無線LAN パスワード
  strlcpy(config.wlan.password,
          doc["wlan"]["password"] | "******",
          sizeof(config.wlan.password));

  // MQTT デバイスID
  strlcpy(config.mqtt.deviceId,
          doc["mqtt"]["deviceId"] | "M5Atom",
          sizeof(config.mqtt.deviceId));

  // MQTT サーバー名
  strlcpy(config.mqtt.server,
          doc["mqtt"]["server"] | "example.com",
          sizeof(config.mqtt.server));

  // MQTT ポート番号
  config.mqtt.port = doc["mqtt"]["port"] | 1883;

  // MQTT ユーザ名
  strlcpy(config.mqtt.user,
          doc["mqtt"]["user"] | "example.com",
          sizeof(config.mqtt.user));

  // MQTT パスワード
  strlcpy(config.mqtt.password,
          doc["mqtt"]["password"] | "example.com",
          sizeof(config.mqtt.password));
       
  // MQTT 位置情報トピック
  strlcpy(config.mqtt.location.topic,
          doc["mqtt"]["location"]["topic"] | "location",
          sizeof(config.mqtt.password));

  // アップロード間隔
  config.interval = doc["interval"] | 5000;

  file.close();
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");

  while (!mqttClient.connect(config.mqtt.deviceId, config.mqtt.user, config.mqtt.password)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

}

void setup() {
  M5.begin(true, false, true);
  Serial.begin(115200);

  // Initialize SD library
  SPI.begin(23,33,19,-1);
  while (!SD.begin(-1, SPI, 40000000)) {
    Serial.println(F("Failed to initialize SD library"));
    delay(1000);
  }
  Serial.println(F("Loading configuration..."));
  loadConfiguration(filename, config);

  
  GPSRaw.begin(9600, SERIAL_8N1, 22, -1);

  WiFi.begin(config.wlan.ssid, config.wlan.password);

  mqttClient.begin(config.mqtt.server, config.mqtt.port, wifiClient);

  connect();
  
}

char* gpsDateTime(TinyGPSPlus *gps) {
  char* p_datetime = NULL;

  p_datetime = (char*)calloc(50, sizeof(char));

  sprintf(p_datetime,"%04d-%02d-%02dT%02d:%02d:%02d.000Z",
    gps->date.year(),
    gps->date.month(),
    gps->date.day(),
    gps->time.hour(),
    gps->time.minute(),
    gps->time.second());

  Serial.printf("%s\n", p_datetime );

  return p_datetime;
}


void loop() {
  char  payload[100];
  char* p_datetime = NULL;
  
  mqttClient.loop();
  if (!mqttClient.connected()) {
    connect();
  }


   
  Serial.printf("### GPS TEST %d\n", cnt++);
  while(GPSRaw.available()>0) {
//    char c = GPSRaw.read();
//    Serial.write(c);

    if(gps.encode(GPSRaw.read())) {
      break;
    }

  }

  if(gps.location.isValid()) {
/*
    Serial.printf("LAT:%.6f\n", gps.location.lat() );
    Serial.printf("LNG:%.6f\n", gps.location.lng() );
    Serial.printf("ALT:%.2f\n", gps.altitude.meters() );
    Serial.printf("DATE:%d\n", gps.date.value() );
    Serial.printf("TIME:%d\n", gps.time.value() );
    Serial.printf("SATELLITES:%d\n", gps.satellites.value() );
    Serial.printf("HDOP:%d\n", gps.hdop.value() );
*/
    memset(&payload[0],0x00,sizeof(payload));
    p_datetime = gpsDateTime(&gps);

    sprintf(payload,"{\"lat\":\"%.6f\",\"lon\":\"%.6f\",\"alt\":\"%.2f\",\"gpstime\":\"%s\"}",gps.location.lat(),gps.location.lng(),gps.altitude.meters(),p_datetime);
    Serial.printf("payload:%s\n", payload );


    char topic[50] = "";
    sprintf(topic,"/%s/%s",config.mqtt.deviceId,config.mqtt.location.topic);
    mqttClient.publish(topic, payload);
  
  } else {
    Serial.printf("INVALID\n");

  }

  if( p_datetime != NULL ){
    free(p_datetime);
    p_datetime = NULL;
  }
  
  delay(config.interval);
}
