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
 no code from the original project
*/

#include <Servo.h>
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <EEPROM.h>

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
int rollPosition = 10;

// set the servo to different channels
uint8_t yawServoNum = 0;
uint8_t pitchServoNum = 1;
uint8_t rollServoNum = 2;

//adafruit servo related values
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// Pins for ultrasonic sensor
int trigPin = 9;
int echoPin = 10;

// variables for making pings with the ultrasonic sensor
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
const int YAW_RANGE = 45;
const int PITCH_RANGE = 11;
const int ROLL_BASE = 90;
int pointsArray[YAW_RANGE][PITCH_RANGE];

// class for managing the map of distances
class PointMap
{
  public:
    PointMap()
    {
      for(int x = 0; x < YAW_RANGE; x++)
      {
        for(int y = 0; y < PITCH_RANGE; y++)
        {
          EEPROM.get((x*4) + (y*YAW_RANGE*4), pointsArray[x][y]);
        }
      }
    }
    
    ~PointMap()
    {}

    // function sets the value at (x, y) in the
    // 2d array collection to value
    void setPoint(int x, int y, int value)
    {
        pointsArray[x][y] = value;
        EEPROM.put((x*4) + (y*YAW_RANGE*4), value);
    }

    // function sets the value at (x, y) in the
    // 2d array collection
    int getPoint(int x, int y)
    {
      return pointsArray[x][y];
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

//point of interest is used when our Buddy finds something new
struct pointOfInterest
{
  int yaw;
  int pitch;
  int roll;
};

moveCommand MC;
pointOfInterest POI;


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
//seed the random function with noise form A0
randomSeed(analogRead(A0));

// ultrasonic sensor pin setup
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
pinMode(5, OUTPUT);

pinMode(3, INPUT);

//initializeDistance();

}

void loop()
{
  // distanceStatus comes from makePing through calculateMovement
  // 1 means a new object distance, 2 means no change in distance
  // 3 means the distance is over 50 cm and therefore too far for us to care
  int distanceStatus;
  int dice;

  randomMove();
  distanceStatus = calculateMovement(MC);
  delay(1000);

  if(distanceStatus == 1)
  {
    // rolls the dice to see if our buddy wants to explore his new object
    // 80% chance he will
    dice = random(5);
    if(dice != 0)
    {
      POI.yaw = MC.yaw;
      POI.pitch = MC.pitch;
      curiousBuddy();
    }
  }
}

// Begining of function definitions
//////////////////////////////////////////////

//function is usually called when buddy finds a new object
//determines how buddy wants to explore
void curiousBuddy()
{
  int i;
  int yawChange;
  int pitchChange;
  int distanceStatus;
  int dice;
  int curiosity = 18;
  int boredom = 1;

  // buddy will look at a point near the point of interest
  // he may then loop this with the same point of interest
  // or mark the nearby point as the new point of interest
  // if he sees an old point or a far away point his curiosity
  // will go down and be more likly to go back to looking around randomly
  do
  {
    Serial.print("My curiosity: ");
    Serial.println(curiosity);
    Serial.print("My boredom: ");
    Serial.println(boredom);
    i = random(-3, 4);
    yawChange = POI.yaw + i;
    i = random(-2, 3);
    pitchChange = POI.pitch + i;
    i = random(2);
    if (i == 1)
    {
      i = random(10, 25);
      MC.roll = i;
      headTilt(MC.roll, 1);
    }
    
    if(yawChange >= 0 && yawChange < YAW_RANGE)
      MC.yaw = yawChange;

    if(pitchChange >= 0 && pitchChange < PITCH_RANGE)
      MC.pitch = pitchChange;

    distanceStatus = calculateMovement(MC);

    if(distanceStatus == 1) // new
    {
      dice = random(2);
        if(dice == 0)
        {
          POI.yaw = MC.yaw;
          POI.pitch = MC.pitch;
        }
        if(curiosity < 20)// prevents buddy from getting too curious with too low a liklyhood to leave loop
          curiosity++;
      if(boredom > 1)
        boredom--;
    }
    else if(distanceStatus == 2) // old
    {
      if(curiosity > 2)
        curiosity -= boredom;
      boredom++;
    }
    else // far
    {
      if (curiosity > 3)
        curiosity -= 2*boredom;
      boredom++;
    }
    
    if (curiosity > 1)
      i = random(curiosity);
    else
      i = 2; 

    Serial.print("my choice to keep exploring: ");
    Serial.println(i);
  }while(i > 2);
  return;
}

//sets our Buddys move command to go somewhere randomly
void randomMove()
{
  MC.yaw = random(YAW_RANGE);
  MC.pitch= random(PITCH_RANGE);
  MC.roll = ROLL_BASE;
}

// function call is not called in loop and is commented out in setup
// can be uncommented to have our buddy analize the entire area but can
// take awhile, can be used if the programer deems it better to have this then not
// we think it is not but was usefull in debugging
void initializeDistance()
{
  MC.roll = ROLL_BASE;
  for(int x = 0; x < PITCH_RANGE; x++)
  {
    MC.pitch = x;
    if(x % 2 == 0)
    {
      for(int y = 0; y < YAW_RANGE; y++)
      {
        MC.yaw = y;
        calculateMovement(MC);
        delay(1);
      }
    }
    else
    {
      for(int y = YAW_RANGE - 1; y >= 0; y--)
      {
        MC.yaw = y;
        calculateMovement(MC);
        delay(1);
      }
    }
  }
}

// triggers the ultrasonic sensor to measure the distance to the point in front of it
// it then stores this value in the pointmap
// returns 1, 2, or 3 for a new distance, a distance within 2cm of previous(old), or over 60cm (too far to care) respectivly
// also contains serial.prints so a programmer can see the values and what our buddy considers then, new, old, or far
int makePing()
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
  if(distance >250 || distance <4){delay(10);}

  Serial.print(distance);
  Serial.print(" cm  ");
  
  if (distance <= 60)
  {
    // Determine if object sensed is "new"
    if (distance < (pointMap.getPoint(yawPosition, pitchPosition) - 2) || distance > (pointMap.getPoint(yawPosition, pitchPosition) + 2))
    {
      //Senses something new!
      pointMap.setPoint(yawPosition, pitchPosition, distance);
      Serial.println("NEW");
      Serial.println();
      digitalWrite(5, HIGH);
      delay(1000);
      digitalWrite(5, LOW);
      return 1;
    }
    // Senses something, but is not new
    pointMap.setPoint(yawPosition, pitchPosition, distance);
    Serial.println("OLD");
    Serial.println();
    return 2;
  }
  // Too far away to care
  pointMap.setPoint(yawPosition, pitchPosition, distance);
  Serial.println("FAR");
  Serial.println();
  return 3;
}


//helper function to make movement
//calculates which servo needs to spin the most and the directions and distance
// so that make movement can simply use pointers and a value of 1 or -1
int calculateMovement(struct moveCommand mc)
{
  int yawDiference;
  int yawDirection = 1;
  int pitchDiference;
  int pitchDirection = 1;
  //int rollDiference;
  //int rollDirection = 1;


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
  

  int toReturn;
  if(yawDiference > pitchDiference)
    toReturn = makeMovement(yawDiference, pitchDiference, &yawPosition, &pitchPosition, yawDirection, pitchDirection, &yawServo, yawServoNum, &pitchServo, pitchServoNum, 5, 45, 40, 360);
  else
    toReturn = makeMovement(pitchDiference, yawDiference, &pitchPosition, &yawPosition, pitchDirection, yawDirection, &pitchServo, pitchServoNum, &yawServo, yawServoNum, 45, 5, 360, 40);


  return toReturn;
}


//function moves the pitch and yaw servos so that they both reach the desired angle at the same time smoothly
int makeMovement(int furtherDistance, int lesserDistance, int * furtherPosition, int * lesserPosition, int furtherDirection, int lesserDirection, Adafruit_PWMServoDriver * furtherTurningServo, uint8_t furtherTurningServoNum, Adafruit_PWMServoDriver * lesserTurningServo, uint8_t lesserTurningServoNum, int lesserFromHigh, int furtherFromHigh, int lesserToHigh, int furtherToHigh)
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
        lesserTurningServo->setPWM(lesserTurningServoNum, 0, SERVOMIN + map((*lesserPosition), 0, lesserFromHigh, 0, lesserToHigh));
        //lesserTurningServo->write((*lesserPosition));
        scaler++;
        delay(turnSpeed*4);
      }
      else
      {
        (*furtherPosition) += furtherDirection;
        furtherTurningServo->setPWM(furtherTurningServoNum, 0, SERVOMIN + map((*furtherPosition), 0, furtherFromHigh, 0, furtherToHigh));
        //furtherTurningServo->write((*furtherPosition));
        delay(turnSpeed*4);
      }
    }
    delay(500);
    return makePing();
}


//function tilts the head then tilts it back
void headTilt(int rollDistance, int rollDirection)
{
  //artifact of older code
  //sensor was superglued to sevro at bad angle and can only move in positive direction
  if(rollDirection == -1)
    rollDirection = 1;
  int turnHere = rollPosition + (rollDistance*rollDirection);
  int backToHere = rollPosition;

  for(rollPosition; rollPosition != turnHere; rollPosition += rollDirection)
  {
    rollServo.setPWM(rollServoNum, 0, ROLL_BASE + rollPosition);
    delay(tiltSpeed);
  }

  delay(1000);

  for(rollPosition -= rollDirection; rollPosition != backToHere; rollPosition -= rollDirection)
  {
    rollServo.setPWM(rollServoNum, 0, ROLL_BASE + rollPosition);
    delay(tiltSpeed);
  }

return;
}

// End of function definitions
//////////////////////////////////////////////
