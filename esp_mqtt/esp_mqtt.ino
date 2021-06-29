#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>  

//WiFi Connection configuration
char ssid[] = "freeboxoudj";     //  le nom du reseau WIFI
char password[] = "868C0D6EA0";  // le mot de passe WIFI
//mqtt server
char mqtt_server[] = "test.mosquitto.org";  //adresse IP serveur (mosquitto publique)
#define MQTT_USER "" 
#define MQTT_PASS ""

WiFiClient espClient;
PubSubClient MQTTclient(espClient);

const int button = D1; //one click button pin
int temp = 0; //status button

//Fonctions blink pour l'envoi
void blinkOnBoardLed(){
   digitalWrite(LED_BUILTIN, LOW);
   delay(200);
   digitalWrite(LED_BUILTIN, HIGH);
}

//Fonctions publication dans topic MQTT
void MQTTsend() {
 //init emulation sonar value
 int sonarNum1 = random(10,255);
 int sonarNum2 = random(10,255);
 int sonarNum3 = random(10,255);
 //Creation d'un payload au format JSON + envoi dans topic
 String payloadTest = "{\"capteurs\":{\"sonar\":["+(String)sonarNum1+","+ (String)sonarNum2+","+ (String)sonarNum3+"],\"isHere\":true}}";
 MQTTclient.publish("sakoutcher/test/payload",payloadTest.c_str());
 delay(100);
 //Publication de la valeur separé aussi
 MQTTclient.publish("sakoutcher/test/sonar1",String(sonarNum1).c_str());
 MQTTclient.publish("sakoutcher/test/sonar2",String(sonarNum2).c_str());
 MQTTclient.publish("sakoutcher/test/sonar3",String(sonarNum3).c_str());
 //Blink pour notifier l'envoi
 blinkOnBoardLed();
}

//Fonction connection MQTT
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
  //Button + LED init
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(button, INPUT_PULLUP ); 
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
  temp = digitalRead(button);
  // connect serveur MQTT
  if (!MQTTclient.connected()) {
    MQTTconnect();
  }

  if ((millis() - lastTimeMqtt >= 10000) && temp == LOW)  // toutes les 20 secondes + etat du boutton (detection de presence)
   {
     lastTimeMqtt = millis();
     MQTTsend();
   }
}
