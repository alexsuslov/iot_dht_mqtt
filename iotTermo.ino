#include <DHT.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <PubSubClient.h>


#define POWER_PIN 12
#define DHTPIN 14
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float h; // Humidity
float t; //Temperature
float hic;
int error;
// wifi
char* AP = "Amber";
char* AP_KEY = "b7f3bbYD9yAP2Wii";
// mqtt
IPAddress MQTT_HOST(192,168,88,100);
int   MQTT_PORT = 20000;
char* MQTT_USER = "test";
char* MQTT_PASS = "test";
#define TIMEOUT 60 // 1min
char tmpTOPIC[64] ;
char tmpMSG[64];
char* MQTT_TOPIC = "/belostrov/place1/";
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  Serial.println("\n Starting dht22 sensor");
  pinMode(POWER_PIN, OUTPUT);
}


//////////////////////////////////////////
void data_get(){

  digitalWrite(POWER_PIN, 1);
  delay(1000);
  h = dht.readHumidity();
  t = dht.readTemperature();
  digitalWrite(POWER_PIN, 0);
  if (isnan(h) || isnan(t)) {
    error = 1; // 1 dht error
    return;
  }
  hic = dht.computeHeatIndex(t, h, false);
}


//////////////////////////////////////////
void data_display(){
  if (error){
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");

  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.println(" *C ");
}
///////////////////////////////////////////
void wifi_connect(){
  if (error){
    return;
  }
  if (WiFi.status() == WL_CONNECTED){
    return;
  }

  WiFi.begin(AP, AP_KEY);
  Serial.print("Connecting");
  int i = 50;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (!i--){
      error = 2; // 2 wifi error
      return;
    }
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void mqtt_connect(){
  if (error || client.connected()){
    return;
  }
  Serial.println("connected mqtt");

  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(mqtt_callback);

  int i = 60;
  while (!client.connected()) {
    if (client.connect("dht22_v", MQTT_USER, MQTT_PASS)){
      Serial.println("mqtt connected");
    } else {
      delay(500);
      Serial.print(".");
      if (!i--){
        error = 3; // 3 mqtt error
        return;
      }
    }
  }
}

void mqtt_pub(){
  if (error || !client.connected()){
    return;
  }
  Serial.println("publish mqtt");
  sprintf(tmpTOPIC,"%st", MQTT_TOPIC);
  dtostrf(t, 2, 2, tmpMSG);
  client.publish(tmpTOPIC, tmpMSG);
  sprintf(tmpTOPIC,"%sh", MQTT_TOPIC);
  dtostrf(h, 2, 2, tmpMSG);
  client.publish(tmpTOPIC, tmpMSG);
  dtostrf(hic, 2, 2, tmpMSG);
  sprintf(tmpTOPIC,"%shic", MQTT_TOPIC);
  client.publish(tmpTOPIC, tmpMSG);
}

// tasks
void loop() {
  error = 0;
  data_get();
  data_display();
  wifi_connect();
  mqtt_connect();
  mqtt_pub();

  if (error) {
    Serial.print("Failed: ");
    Serial.println(error);
    return;
  }
  delay( TIMEOUT * 1000);
}