#include <SoftwareSerial.h>

#define RX 2
#define TX 3
#define RELAY 8
#define ECHO  9
#define TRIGGER 10
#define BUZZER 11

#define speed_of_sound 0.0343
#define minimum_distance 4.0
#define delay_time 800

SoftwareSerial ArduinoUno(RX, TX);

int distance() {
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

void open_valve() {
  digitalWrite(RELAY, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  noTone(BUZZER);
}

void close_valve() {
  digitalWrite(RELAY, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  tone(BUZZER, 500);
}

void setup() {
  Serial.begin(9600);
  ArduinoUno.begin(4800);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  // Check Water Level
  if(distance() < minimum_distance) {
    digitalWrite(RELAY, LOW);
    //digitalWrite(LED_BUILTIN, HIGH);
    ArduinoUno.write(distance());
    Serial.println(distance());
    delay(500);
  }
  else {
    while(distance() > minimum_distance) {
      digitalWrite(RELAY, HIGH);
      //digitalWrite(LED_BUILTIN, LOW);
      ArduinoUno.write(distance());
      Serial.println(distance());
      delay(500);
    }
    digitalWrite(RELAY, LOW);
    //digitalWrite(LED_BUILTIN, HIGH);
    ArduinoUno.write(distance());
    Serial.println(distance());
    delay(500);
  }
}
