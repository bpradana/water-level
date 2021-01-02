#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial

#define RX D0
#define TX D1

#define minimum_distance 4.0

char auth[] = "Makwq7xL7LEQdCm5DJLL9kLmznSKOJIO";
char ssid[] = "Kutai_Barat";
char pass[] = "26240214";

SoftwareSerial NodeMCU(RX, TX);
BlynkTimer timer;

BLYNK_WRITE(V0) {
  int user_input = param.asInt();
  NodeMCU.write(user_input);
}

float water_level;
void timerEvent() {
  while(NodeMCU.available() > 0) {
    water_level = NodeMCU.read();
    Blynk.virtualWrite(V1, water_level);
    if(water_level < minimum_distance) {
      Blynk.virtualWrite(V0, LOW);
    }
  }
}

void setup() {
  Serial.begin(9600);
  NodeMCU.begin(4800);
  Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8080);
  
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  timer.setInterval(1000L, timerEvent);
}

void loop() {
  Blynk.run();
}
