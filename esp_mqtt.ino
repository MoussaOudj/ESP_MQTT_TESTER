#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>  

//WiFi Connection configuration
char ssid[] = "freeboxoudj";     //  le nom du reseau WIFI
char password[] = "868C0D6EA0";  // le mot de passe WIFI
//mqtt server
char mqtt_server[] = "test.mosquitto.org";  //adresse IP serveur 
#define MQTT_USER ""
#define MQTT_PASS ""

WiFiClient espClient;
PubSubClient MQTTclient(espClient);

//Fonctions mqtt
void MQTTsend() {
  static int cpt=0;
  cpt++;
 String reponse="test Mqtt n°"+(String)cpt;
 MQTTclient.publish("sakoutcher/tutomqtt/msg",reponse.c_str());
}

void MQTTconnect() {

  while (!MQTTclient.connected()) {
      Serial.print("Attente  MQTT connection...");
      String clientId = "TestClient-";
      clientId += String(random(0xffff), HEX);

    // test connexion
    if (MQTTclient.connect(clientId.c_str(),"","")) {
      Serial.println("connected");

    } else {  // si echec affichage erreur
      Serial.print("ECHEC, rc=");
      Serial.print(MQTTclient.state());
      Serial.println(" nouvelle tentative dans 5 secondes");
      delay(5000);
    }
  }
}

void setup() {
Serial.begin(115200);
  // Conexion WIFI
   WiFi.begin(ssid, password);
   Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
   Serial.println("Connected");
   MQTTclient.setServer(mqtt_server, 1883);
  
}

void loop() {
  static uint32_t  lastTimeMqtt= 0;
  // connect serveur MQTT
  if (!MQTTclient.connected()) {
    MQTTconnect();
  }

  if (millis() - lastTimeMqtt >= 10000)  // toutes les 20 secondes
   {
     lastTimeMqtt = millis();
     MQTTsend();
   }
}
