// Buddy.ino
// CS 207 project
// Brandon Huzil 200379909
// Lukas Hoffman 200358045

/* 
 Buddy is a arduino project for cs 207 
 Building Interactive Gadgets inspired by 
 the buddy project made by Littlebots
 
 Both this code code and our construction
 of our buddy is completly original, taking
 no code from the original project.
*/

#include <Servo.h>
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>


///////////////////////////////////////////////
// Begining of global variable declorations

// the delay after every single degree turn of a servo for yaw and pitch
const int turnSpeed = 2;

// the delay after every single degree tuen of the for roll ie. head tilting
const int tiltSpeed = 10;

// servo that manages movement along a horizontal plane
// sweeping the head from side to side
Adafruit_PWMServoDriver yawServo = Adafruit_PWMServoDriver();
int yawPosition = 0;

// servo that manages movement along a vertical plane
// sweeping the head up and down
Adafruit_PWMServoDriver pitchServo = Adafruit_PWMServoDriver();
int pitchPosition = 0;

// servo that manages movement the titling of the head
Adafruit_PWMServoDriver rollServo = Adafruit_PWMServoDriver();
int rollPosition = 40;

// set the servo to different channels
uint8_t yawServoNum = 0;
uint8_t pitchServoNum = 1;
uint8_t rollServoNum = 2;

#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// Pins for ultrasonic sensor
int trigPin = 9;
int echoPin = 10;

long duration;
int distance;
// End of global variable declorations
///////////////////////////////////////////////


///////////////////////////////////////////////
// Begining of class and structure definitions
// !! with class specific arrays !!

// Arrays used by pointMap class
// having them within pointMap makes
// the class too large for arduino
int pointsArray1[160][40];
int pointsArray2[160][40];
int pointsArray3[160][40];
int pointsArray4[160][40];

// class for managing the map of distances
class PointMap
{
  public:
    PointMap()
    {
      for(int x = 0; x < 160; x++)
      {
        for(int y = 0; y < 40; y++)
        {
          pointsArray1[x][y] = 0;
          pointsArray2[x][y] = 0;
          pointsArray3[x][y] = 0;
          pointsArray4[x][y] = 0;
        }
        pointsArray4[x][40] = 0;
      }

    }
    
    ~PointMap()
    {}

    // function sets the value at (x, y) in the
    // 2d array collection to value
    void setPoint(int x, int y, int value)
    {
      if( y % 40 == 0)
        pointsArray1[x][y] = value;
      else if( y % 40 == 1)
        pointsArray2[x][y] = value;
      else if( y % 40 == 2)
        pointsArray2[x][y] = value;
      else if( y % 40 >= 3)
        pointsArray2[x][y] = value;
    }

    // function sets the value at (x, y) in the
    // 2d array collection
    int getPoint(int x, int y)
    {
      if( y % 40 == 0)
        return pointsArray1[x][y];
      else if( y % 40 == 1)
        return pointsArray2[x][y];
      else if( y % 40 == 2)
        return pointsArray2[x][y];
      else if( y % 40 >= 3)
        return pointsArray2[x][y];
    }

};

// instance of PointMap to be used
PointMap pointMap;

// struct that ecapsulates ints for
// where to move together
struct moveCommand
{
  int yaw; // holds change of yaw
  int pitch; // holds change of pitch
  int roll; // holds change of roll
};


// End of class and structure definitions
///////////////////////////////////////////////

void setup()
{
  Serial.begin(9600);
  
 yawServo.begin();
 yawServo.setOscillatorFrequency(27000000);
 yawServo.setPWMFreq(SERVO_FREQ);

 pitchServo.begin();
 pitchServo.setOscillatorFrequency(27000000);
 pitchServo.setPWMFreq(SERVO_FREQ);

 rollServo.begin();
 rollServo.setOscillatorFrequency(27000000);
 rollServo.setPWMFreq(SERVO_FREQ);

delay(10);
}

void loop()
{
  // put your main code here, to run repeatedly:
 moveCommand mc;
 
  mc.yaw = 20;
  mc.pitch = 44;
  mc.roll = 60;

  calculateMovement(mc);

  mc.yaw = 160;
  mc.pitch = 160;
  mc.roll = 40;
  
  calculateMovement(mc);

  mc.yaw = 0;
  mc.pitch = 0;
  mc.roll = 40;

  calculateMovement(mc);
  
  mc.yaw = 90;
  mc.pitch = 3;
  mc.roll = 40;
  calculateMovement(mc);

  mc.yaw = 129;
  mc.pitch = 150;
  mc.roll = 40;
  calculateMovement(mc);

}

// Begining of function definitions
//////////////////////////////////////////////











int makePing(int pos)
{
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
  //if(distance >250 || distance <4){}

  if (distance <= 20)
  {
    // Determine if object sensed is "new"
    if (pointMap.getPoint(yawPosition, pitchPosition) != distance && (distance < (pointMap.getPoint(yawPosition, pitchPosition) - 2) || distance > (pointMap.getPoint(yawPosition, pitchPosition) + 2)))
    {
      pointMap.setPoint(yawPosition, pitchPosition, distance);
      return 1;
    }
    // Senses something, but is not new
    return 2;
  }
  // Does not sense anything
  return 3;
}


//helper function to make movement
//calculates which servo needs to spin the most and the directions and distance
// so that make movement can simply use pointers and a value of 1 or -1
void calculateMovement(struct moveCommand mc)
{
  int yawDiference;
  int yawDirection = 1;
  int pitchDiference;
  int pitchDirection = 1;
  int rollDiference;
  int rollDirection = 1;


  yawDiference = mc.yaw - yawPosition;
  if(yawDiference < 0)
  {
    yawDiference *= -1;
    yawDirection = -1;
  }
  
  pitchDiference = mc.pitch - pitchPosition;
  if(pitchDiference < 0)
  {
    pitchDiference *= -1;
    pitchDirection = -1;
  }
  
  rollDiference = mc.roll - rollPosition;
  if(rollDiference < 0)
  {
    rollDiference *= -1;
    rollDirection = -1;
  }

  if(yawDiference > pitchDiference)
    makeMovement(yawDiference, pitchDiference, &yawPosition, &pitchPosition, yawDirection, pitchDirection, &yawServo, yawServoNum, &pitchServo, pitchServoNum);
  else
    makeMovement(pitchDiference, yawDiference, &pitchPosition, &yawPosition, pitchDirection, yawDirection, &pitchServo, pitchServoNum, &yawServo, yawServoNum);
  
  if (rollDiference > 0)
    headTilt(rollDiference, rollDirection);
  
  return;
}


//function moves the pitch and yaw servos so that they both reach the desired angle at the same time smoothly
void makeMovement(int furtherDistance, int lesserDistance, int * furtherPosition, int * lesserPosition, int furtherDirection, int lesserDirection, Adafruit_PWMServoDriver * furtherTurningServo, uint8_t furtherTurningServoNum, Adafruit_PWMServoDriver * lesserTurningServo, uint8_t lesserTurningServoNum)
{
  int totalTicks = furtherDistance + lesserDistance;
  float lesserQuotient = (float)totalTicks / (float)lesserDistance;

    //for loop ensures that movement in both axis is done as evenly as possible
    int scaler = 1;
    for(int tick = 1; tick <= totalTicks; tick++)
    {
      if(tick == ceil(scaler * lesserQuotient))
      {
        (*lesserPosition) += lesserDirection;
        furtherTurningServo->setPWM(lesserTurningServoNum, 0, SERVOMIN + (*lesserPosition));
        //lesserTurningServo->write((*lesserPosition));
        scaler++;
        delay(turnSpeed);
      }
      else
      {
        (*furtherPosition) += furtherDirection;
        furtherTurningServo->setPWM(furtherTurningServoNum, 0, SERVOMIN + (*furtherPosition));
        //furtherTurningServo->write((*furtherPosition));
        delay(turnSpeed);
      }
    }

    return;
}


//function tilts the head then tilts it back
void headTilt(int rollDistance, int rollDirection)
{
  Serial.println(rollPosition);
  
  for(int tick = rollPosition + rollDirection; tick != rollPosition + (rollDistance * rollDirection); tick += rollDirection)
  {
    rollServo.setPWM(rollServoNum, 0, SERVOMIN + tick);
    //rollServo.write(tick);
    delay(tiltSpeed);
  }
  rollPosition = rollPosition + (rollDistance*rollDirection);
  Serial.println(rollPosition);
  delay(500);

  for(int tick = rollPosition - rollDirection; tick != 40; tick -= rollDirection)
  {
    rollServo.setPWM(rollServoNum, 0, SERVOMIN + tick);
    //rollServo.write(tick);
    delay(tiltSpeed);
  }
  rollPosition = 40;
  return;
}

// End of function definitions
//////////////////////////////////////////////
