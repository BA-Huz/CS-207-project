#include <Servo.h>

// the delay after every single degree turn of a servo for yaw and pitch
const int turnSpeed = 8;

// the delay after every single degree tuen of the for roll ie. head tilting
const int tiltSpeed = 10;

// servo that manages movement along a horizontal plane
// sweeping the head from side to side
Servo yawServo;
int yawPosition = 0;


// servo that manages movement along a vertical plane
// sweeping the head up and down
Servo pitchServo;
int pitchPosition = 0;


// servo that manages movement the titling of the head
Servo rollServo;
int rollPosition = 20;


struct moveCommand
{
  int yaw; // holds change of yaw
  int pitch; // holds change of pitch
  int roll; // holds change of roll
  char moveStyle; // char describes the arc of movement
  // s for straight, c for curved
};


void setup()
{
  // put your setup code here, to run once:
  yawServo.attach(0);
  //yawServo.write(0);
  
  pitchServo.attach(1);
  pitchServo.write(0);
  
  rollServo.attach(2);
  rollServo.write(20);
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
  mc.roll = 10;

  calculateMovement(mc);
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
    makeMovement(yawDiference, pitchDiference, &yawPosition, &pitchPosition, yawDirection, pitchDirection, &yawServo, &pitchServo);
  else
    makeMovement(pitchDiference, yawDiference, &pitchPosition, &yawPosition, pitchDirection, yawDirection, &pitchServo, &yawServo);
  
  if (rollDiference > 0)
    headTilt(rollDiference, rollDirection);
  
  return;
}


//function moves the pitch and yaw servos so that they both reach the desired angle at the same time smoothly
void makeMovement(int furtherDistance, int lesserDistance, int * furtherPosition, int * lesserPosition, int furtherDirection, int lesserDirection, Servo * furtherTurningServo, Servo * lesserTurningServo)
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
        lesserTurningServo->write((*lesserPosition));
        scaler++;
        delay(turnSpeed);
      }
      else
      {
        (*furtherPosition) += furtherDirection;
        furtherTurningServo->write((*furtherPosition));
        delay(turnSpeed);
      }
    }

    return;
}


//function tilts the head then tilts it back
void headTilt(int rollDistance, int rollDirection)
{
  for(int tick = rollPosition + rollDirection; tick != rollPosition + (rollDistance * rollDirection); tick += rollDirection)
  {
    rollServo.write(tick);
    delay(tiltSpeed);
  }
  rollPosition = rollPosition + (rollDistance*rollDirection);
  delay(500);

  for(int tick = rollPosition - rollDirection; tick != 20; tick -= rollDirection)
  {
    rollServo.write(tick);
    delay(tiltSpeed);
  }
  rollPosition = 20;

  return;
}
