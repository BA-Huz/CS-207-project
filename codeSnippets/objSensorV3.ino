// Additional info for ultrasonic sensor: https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf
// Additional info for ultrasonic sensor: https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf
/**************************
*  Scans from left to right and repeats.
* Ultrasonic sensor detects objects at <= 20cm
* Sensor detects 3 different states:
*   1. A new object is detected.
*   2. Object detected, but has 
*   already been detected previously.
*   3. No object <= 20cm has been detected.
**************************/

#include <Servo.h>

// Servo
Servo myservo;
Servo myNeck;
int pos = 0;
int body = 3;
int neck = 4;

// Pins for ultrasonic sensor
int trigPin = 9;
int echoPin = 10;

long duration;
int distance;

int stuff[180];




void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myservo.attach(body);
  myNeck.attach(neck);
  myNeck.write(120);
  for (int i = 0; i < 180; i++)
    stuff[i] = -1;
}

void loop() {




  // Scanning left to right
  for (pos = 0; pos < 180; pos += 1)
  {
    myservo.write(pos);

    if (aThing(pos) == 1)
    {
      Serial.println("wow, a new thing!");
      while (aThing(pos) != 2 || aThing(pos) !=3)
      {
        Serial.println("Been there, done that");
        pos++;
        myservo.write(pos);
        delay(15);
      }
      delay(25);
    }

    else if (aThing(pos) == 2)
    {
      Serial.println("wow, a thing!");
      //   pos += 10;
      delay(15);
    }

    else
      Serial.println("Searching...");
    delay(15);
    //delay(300);

  }






  for (pos = 180; pos >= 1; pos -= 1)
  {
    myservo.write(pos);

    if (aThing(pos) == 1)
    {
      Serial.println("wow, a new thing!");
      //   pos -= 25;
      while (aThing(pos) != 2 || aThing(pos) !=3)
      {
        Serial.println("Been there, done that");
        pos--;
        myservo.write(pos);
        delay(15);
      }
      delay(25);
    }

    else if (aThing(pos) == 2)
    {
      Serial.println("wow, a thing!");
      //   pos += 10;
      delay(15);
    }

    else
      Serial.println("Searching...");
    delay(15);
    //delay(300);
  }

  Serial.println("END OF LOOP\n");
  delay(2000);
  for (int i = 0; i < 180; i++)
    Serial.println(stuff[i]);

  while (1)
    delay(7000);

}


int aThing(int pos) {


  // Set low to ensure trigpin is clear
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  // Emit pulse for 10 microseconds and turn off
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Start pulseIn timer
  duration = pulseIn(echoPin, HIGH);

  // Calculate by multiplaying by speed of sound and dividing by 2
  // since the sound wave emits to object, and then bounces back to sensor
  distance = duration *= 0.034 / 2;
  if(distance >250 || distance <4){}

  if (distance <= 20)
  {
    Serial.println(distance);
    Serial.println(stuff[pos]);

    // Determine if object sensed is "new"
    if (stuff[pos] != distance && (distance < (stuff[pos] - 3) || distance > (stuff[pos] + 3)))
    {
      stuff[pos] = distance;
      return 1;
    }
    // Senses something, but is not new
    return 2;
  }
  // Does not sense anything
  return 3;

}
#include <Servo.h>

// Servo
Servo myservo;
Servo myNeck;
int pos = 0;
int body = 3;
int neck = 4;

// Pins for ultrasonic sensor
int trigPin = 9;
int echoPin = 10;

long duration;
int distance;

int stuff[180];




void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myservo.attach(body);
  myNeck.attach(neck);
  myNeck.write(120);
  for (int i = 0; i < 180; i++)
    stuff[i] = -1;
}

void loop() {




  // Scanning left to right
  for (pos = 0; pos < 180; pos += 1)
  {
    myservo.write(pos);

    if (aThing(pos) == 1)
    {
      Serial.println("wow, a new thing!");
      while (aThing(pos) != 2 || aThing(pos) !=3)
      {
        Serial.println("Been there, done that");
        pos++;
        myservo.write(pos);
        delay(15);
      }
      delay(25);
    }

    else if (aThing(pos) == 2)
    {
      Serial.println("wow, a thing!");
      //   pos += 10;
      delay(15);
    }

    else
      Serial.println("Searching...");
    delay(15);
    //delay(300);

  }






  for (pos = 180; pos >= 1; pos -= 1)
  {
    myservo.write(pos);

    if (aThing(pos) == 1)
    {
      Serial.println("wow, a new thing!");
      //   pos -= 25;
      while (aThing(pos) != 2 || aThing(pos) !=3)
      {
        Serial.println("Been there, done that");
        pos--;
        myservo.write(pos);
        delay(15);
      }
      delay(25);
    }

    else if (aThing(pos) == 2)
    {
      Serial.println("wow, a thing!");
      //   pos += 10;
      delay(15);
    }

    else
      Serial.println("Searching...");
    delay(15);
    //delay(300);
  }

  Serial.println("END OF LOOP\n");
  delay(2000);
  for (int i = 0; i < 180; i++)
    Serial.println(stuff[i]);

  while (1)
    delay(7000);

}


int aThing(int pos) {


  // Set low to ensure trigpin is clear
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  // Emit pulse for 10 microseconds and turn off
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Start pulseIn timer
  duration = pulseIn(echoPin, HIGH);

  // Calculate by multiplaying by speed of sound and dividing by 2
  // since the sound wave emits to object, and then bounces back to sensor
  distance = duration *= 0.034 / 2;
  if(distance >250 || distance <4){}

  if (distance <= 20)
  {
    Serial.println(distance);
    Serial.println(stuff[pos]);

    // Determine if object sensed is "new"
    if (stuff[pos] != distance && (distance < (stuff[pos] - 3) || distance > (stuff[pos] + 3)))
    {
      stuff[pos] = distance;
      return 1;
    }
    // Senses something, but is not new
    return 2;
  }
  // Does not sense anything
  return 3;

}
