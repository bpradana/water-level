// Defining Pins
#define RELAY 4
#define ECHO  9
#define TRIGGER 10
#define BUZZER 11

// Defining Constants
#define speed_of_sound 0.0343
#define minimum_distance 4.0
#define delay_time 800

// Function to Calculate Distance in CM
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

void setup() {
  Serial.begin(9600);
  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(RELAY, OUTPUT);
}

float current_distance;

void loop() {
  current_distance = distance();
  if(current_distance < minimum_distance) {
    Serial.println("triggered");
    digitalWrite(RELAY, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    tone(BUZZER, 500);
    delay(1000);
  }
  else {
    digitalWrite(RELAY, LOW);
    digitalWrite(LED_BUILTIN, LOW);
    noTone(BUZZER);
  }
}
