/*
  Program by Brandon Huzil
  detects change in distance using an ultrasonic sensor
  and lights up a led after a change of 1 cm
*/
const int pingPin = 7;  //trigger a ping
const int echoPin = 6;  // receive
const int ledPin = 13;
float pointDistance;

void setup()
{
  // put your setup code here, to run once:
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pointDistance = 0;
  Serial.begin(9600);
}


void loop()
{
  float newPointDistance;
  // put your main code here, to run repeatedly:
  newPointDistance = sendPing();
  Serial.println(newPointDistance);
  
  if(newPointDistance < pointDistance - 1.0 || newPointDistance > pointDistance + 1.0)
  {
    pointDistance = newPointDistance;
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
  }

}

// function triggers the sensor to detect distance
float sendPing()
{
  float duration = 0;

  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);

  duration += pulseIn(echoPin, HIGH);
  
  return microsecondsToCentimeters(duration);
}

// function converts microseconds to centimeters
float microsecondsToCentimeters(float micro)
{
  return (micro / 29.0 / 2.0);
}
