// Libraries
#include <DHT.h> // include dht adafruit library - download via arduino ide

// Constants
#define DHTPIN 3 //defines pin number to which the sensor is connected
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE) ;

// Variables
const int TRIG_PIN_1 = 11;
const int ECHO_PIN_1 = 12;
const int TRIG_PIN_2 = 6;
const int ECHO_PIN_2 = 7;

const int BUZZER_PIN = 2;
const int DISTANCE_THRESHOLD = 50; 
const int MOTOR_PIN = 10;

float hum; float temp;
float speed_of_sound;
float travel_distance_1;
float travel_distance_2;

float buzz_distance_cm;


float duration_us_1, distance_cm_1;
float duration_us_2, distance_cm_2;


int motorState = LOW;  // motorState used to set the MOTOR_PIN

unsigned long currentMillis = 0;    // current milliseconds
unsigned long previousMillis = 0;  // will store last time MOTOR_PIN was updated

long interval = 1000000000;  // interval at which to vibrate the motor (milliseconds)



int gradual(float dist)  // change the frequency of the buzzer based on the measured distance
{
  if (dist>40) return 500;
  if (dist>30) return 1000;
  if (dist>20) return 1500;
  if (dist>10) return 2000;
  if (dist<=10) return 3000;
}

void millisDelay( long int delayTime){ // delay sensor
  long int start_time = millis();
  while ( millis() - start_time < delayTime) ;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  
  pinMode(TRIG_PIN_1, OUTPUT);
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(TRIG_PIN_2, OUTPUT);
  pinMode(ECHO_PIN_2, INPUT);

  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);


}

void loop() {
  // put your main code here, to run repeatedly:


    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      // save the last time the motor state changed
      previousMillis = currentMillis;
      Serial.print("blink mils");
  
      // if the motor is off turn it on and vice-versa:

        if (motorState == HIGH) {
          motorState = LOW;
      } else {
          motorState = HIGH;
      }
  
      tone(BUZZER_PIN, gradual(buzz_distance_cm)); // set the tone of the buzzer
      digitalWrite(MOTOR_PIN,motorState);      // set the MOTOR_PIN with the motorState of the variable:
    }

  // read humidity and temperature values from dht22 sensor, compute the speed of sound
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  speed_of_sound = (331.4 + (0.6 * temp) + (0.0124 * hum)) / 10000; // speed of sound in cm/us
  
  digitalWrite(TRIG_PIN_1, LOW);
  millisDelay(0.002); // Clears the TRIG_PIN_1
  digitalWrite(TRIG_PIN_1, HIGH);
  millisDelay(0.01);   // Sets the TRIG_PIN_1 on HIGH state for 10 micro seconds
  digitalWrite(TRIG_PIN_1, LOW);

   // measure duration of pulse from ECHO pin by the first sensor
  duration_us_1 = pulseIn(ECHO_PIN_1, HIGH); // the travel time of the ultrasonic wave
  travel_distance_1 = speed_of_sound * duration_us_1;  // the travel distance of the ultrasonic wave in cm
  distance_cm_1 = travel_distance_1/2;   // calculate the distance

  millisDelay(60); // delay so the sensors do not interfere, 60 ms based on the info in datasheet

  digitalWrite(TRIG_PIN_2, LOW);
  millisDelay(0.002); // Clears the TRIG_PIN_2
  digitalWrite(TRIG_PIN_2, HIGH);
  millisDelay(0.01); // Sets the TRIG_PIN_2 on HIGH state for 10 micro seconds
  digitalWrite(TRIG_PIN_2, LOW);


  // measure duration of pulse from ECHO pin by the second sensor
  duration_us_2 = pulseIn(ECHO_PIN_2, HIGH); // the travel time of the ultrasonic wave
  travel_distance_2 = speed_of_sound * duration_us_2;  // the travel distance of the ultrasonic wave in cm
  distance_cm_2 = travel_distance_2/2;   // calculate the distance  

  if (distance_cm_1 <= distance_cm_2){ // set the buzzer frequency based on the distance to the closest object
    buzz_distance_cm = distance_cm_1;
    }
  else{
    buzz_distance_cm = distance_cm_2;
    }


if(distance_cm_1 < DISTANCE_THRESHOLD && distance_cm_2 < DISTANCE_THRESHOLD ) { // if the object is detected by both sensors
    interval = 250;
    
  }

  else if (distance_cm_1 < DISTANCE_THRESHOLD) { // if the object is detected by the first sensor
  interval = 1250;
    
  }
  
  else if (distance_cm_2 < DISTANCE_THRESHOLD) { // if the object is detected by the second sensor
  interval = 750;
    
  }
  
  else{ // no object detected
    noTone(BUZZER_PIN);
    motorState = HIGH;
    digitalWrite(MOTOR_PIN,motorState);
    interval = 1000000000; // delay, no tone and no buzz
  }

 // print the values to Serial Monitor
  Serial.print("distance 1 in cm: ");
  Serial.print(distance_cm_1);
  Serial.println(" cm");

  Serial.print("distance 2 in cm: ");
  Serial.print(distance_cm_2);
  Serial.println(" cm");

}
