/************************** Introduction

-OurBuddy is a stationary, “anthropomorphized” robot that scans and reacts to objects nearby in front of it.
While scanning, there are three different states it reads:

	1. New -> object scanned is new
	2. Old -> object scanned has been previously scanned
	3. Far -> object must be 60cm or nearer for a reaction

-When scanning an object it stores its location and distance by servo position.
Different audio clips plays depending on the state that is read by the sensor.
	
******************************************************************************/


/************************** Building

-List of components needed for this version:
   
	• 1x  Elegoo HC-SR04 ultrasonic sensor.
    • 1x elastic band
    • 1x HXT900 plastic gear micro servo 
    • 2x  MG996R metal gear servo
    • 1x 111m x 55m styrofoam solid dome base (base for head & neck)
    • 1x Arduino Uno Rev3
    • 1x LED
    • 1x Breadboard 
    • 1x SunFounder PCA9685 16-channel servo driver
    • 1x AA Battery holder with barrel jack
    • 5x AA 1.5v battery
    • Jumper wires
    • Glue
    • Popsicle sticks
    • Electrical tape
	
-To build this version -> refer to documentation in 
	section 2. Design & Building Phase 
	
-Recommended to install resistor portion of the circuit first if incorporating audio.
	Audio is not needed for OurBuddy to work.
	
-Install servo components together

-Install ultrasonic sensor after last servo is prepared

-Connect necessary wires to pins last
		
	
******************************************************************************/

/************************** Code
	Getting started

-Libraries used:

	#include <Adafruit_PWMServoDriver.h>
		-> For dedicated servo driver 
	#include <Wire.h>
		-> I2C connection used with servo driver and Arduino
	#include <avr/pgmspace.h>
		-> Only necessary if including audio
		
-Using the Adafruit library with the servos is slightly different then using the standard <Servo.h> library
	http://wiki.sunfounder.cc/index.php?title=PCA9685_16_Channel_12_Bit_PWM_Servo_Driver

	
-const int turnSPeed
	-> This value determines how fast the "yaw" servo rotates. Can be changed accordingly
	
-const int tiltSpeed
	-> This value determines how fast the "roll" servo rotates. Can be changed accordingly
	
-int *position
	-> These values default position value of servos
	
-Adafruit library. These values are for servo values. With the servo driver, the fuctions use
	pulse width modulation
	
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
	
-makePing() function:
	-> To change values to do with the sensor ie: the max distance it reacts to, change the distance values
		in this function
	-> This is where to include different audio files if included.
	

		

/************************** Audio

-To include audio, add the <avr/pgmspace.h> library.

-When you have your audio file, use Sound2Code 2 to convert sound file into an array type.

-In Sketch, declare an array as type:

	const PROGMEM byte /*arrayName*/[] =
		{
			// Values from Sound2Code here
		};
		
******************************************************************************/




















