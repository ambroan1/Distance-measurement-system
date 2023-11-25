// Libraries
#include <DHT.h> // include dht adafruit library - download via arduino ide

// Constants
#define DHTPIN 5 //defines pin number to which the sensor is connected
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE) ;

// Variables
const int TRIG_PIN = 6;
const int ECHO_PIN = 7;
const int BUZZER_PIN = 3;
const int DISTANCE_THRESHOLD = 50;
const int MOTOR_PIN = 10;

float hum; float temp;
float speed_of_sound;
float travel_distance;

float duration_us, distance_cm;

int gradual(float dist)
{
  if (dist>40) return 500;
  if (dist>30) return 1000;
  if (dist>20) return 1500;
  if (dist>10) return 2000;
  if (dist<=10) return 3000;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  // read humidity and temperature values from dht22 sensor
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  speed_of_sound = (331.4 + (0.6 * temp) + (0.0124 * hum)) / 10000; // speed of sound in cm/us
  
  
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

    // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH); // the travel time of the ultrasonic wave
  travel_distance = speed_of_sound * duration_us;  // the travel distance of the ultrasonic wave in cm
  distance_cm = travel_distance/2;   // calculate the distance

  if(distance_cm < DISTANCE_THRESHOLD) {
    //digitalWrite(BUZZER_PIN, HIGH); // turn on Piezo Buzzer

    tone(BUZZER_PIN, gradual(distance_cm));
    digitalWrite(MOTOR_PIN,LOW);
  }
  else{
    //digitalWrite(BUZZER_PIN, LOW);  // turn off Piezo Buzzer
    noTone(BUZZER_PIN);
    digitalWrite(MOTOR_PIN,HIGH);
  }
 // print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  Serial.print("humidity: ");
  Serial.print(hum);
  Serial.print(" %, temperature: ");
  Serial.print(temp);
  Serial.println(" Celsius");

  
  
  delay(1000);
}
