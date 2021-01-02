/* 
 * Code by Bintang Pradana Erlangga Putra
 * https://github.com/bpradana
 */

// Library
#include "pitches.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial

// Defining Pins
#define ECHO D0
#define TRIGGER D1
#define RELAY D2
#define LED D3
#define BUZZER D5

// Defining VPins
#define MONITOR V0
#define BUTTON V1

// Defining Constants
#define speed_of_sound 0.0343
#define minimum_distance 10.0
#define delay_time 100

// Authentication
char auth[] = "Makwq7xL7LEQdCm5DJLL9kLmznSKOJIO";
char ssid[] = "Kutai_Barat";
char pass[] = "26240214";

float water_level;
BlynkTimer timer;

// Send Data
void eventTimer() {
  Blynk.virtualWrite(MONITOR, distance());
}

// Calculate Distance in CM
float distance() {
  long duration;
  float distance;

  // Send Pulse
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  // Detect Pulse
  duration = pulseIn(ECHO, HIGH);

  //Calculate Distance
  distance = duration * speed_of_sound / 2;

  return distance;
}

// Blink Builtin LED X Times
void blink_LED() {
  for(int i=0; i<10; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    // tone(BUZZER, 500);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    // noTone(BUZZER);
    delay(100);
  }
}

// Play DJI Phantom Startup Sound LMAO
void startup_sound() {
  int melody[] = {NOTE_B5, NOTE_DS6, NOTE_FS6, NOTE_GS6};
  int duration[] = {8, 8, 8, 8};
  int len = 4;
  for(int i=0; i < len; i++) {
    int noteDuration = 1000 / duration[i];
    tone(BUZZER, melody[i], noteDuration);
    int pause = noteDuration * 1.0;
    delay(pause);
    noTone(BUZZER);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Setting Up...");
  Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8080);
  timer.setInterval(1000L, eventTimer);

  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  startup_sound();
  blink_LED();
  Serial.println("Setup Complete!");
}

void loop() {
  // Run Blynk
  Blynk.run();
  timer.run();
  
  // Read HC-SR04
  water_level = distance();
  Serial.println(water_level);
  
  if(water_level > minimum_distance) {
    digitalWrite(LED, LOW);
    digitalWrite(RELAY, HIGH);
  }
  else {
    digitalWrite(LED, HIGH);
    digitalWrite(RELAY, LOW);
    Blynk.notify("FULL!");
  }
  
  // Prevent Overload or Some Other Shits
  delay(delay_time);
}
