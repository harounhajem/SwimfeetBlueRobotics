#include <Servo.h>
Servo thruster;

// Pinout declaration
#define thrusterPort 12

#define buttonLv1	 8
#define buttonLv2	 9
#define buttonLv3	 10

#define joystickPort 14

#define ledLampRed1	 2
#define ledLampGreen 3
#define ledLampBlue 4
#define ledLampYellow 5
#define ledLampRed2 6
#define ledLampWhite 7


// Variable declaration
bool buttonVal1 = false;
bool buttonVal2 = false;
bool buttonVal3 = false;

// Speed levels
int speedLevel1 = 1580; // Minimum
int speedLevel2 = 1660; // Min-Med
int speedLevel3 = 1740; // Medium
int speedLevel4 = 1820; // Med-Max
int speedLevel5 = 1900; // Maximum
int speedLevelStop = 1500; // Stop

// Thruster values
int delayTime = 100;
int currentSpeed = 1500;
int newSpeed = 1500;
unsigned long timerThruster = 0;


void setup()
{

#pragma region Thruster instructions
	/*

	Thruster communication:
	COMMAND:		MICROSECOND
	Stopped 	    1500 microseconds
	Max forward 	1900 microseconds
	Max reverse 	1100 microseconds

	*/
#pragma endregion


	// Pinout usage declaration 
	thruster.attach(thrusterPort);

	pinMode(joystickPort, INPUT);
	pinMode(ledLampRed1, OUTPUT);
	pinMode(ledLampGreen, OUTPUT);
	pinMode(ledLampBlue, OUTPUT);
	pinMode(ledLampYellow, OUTPUT);
	pinMode(ledLampRed2, OUTPUT);
	pinMode(ledLampWhite, OUTPUT);

	pinMode(buttonLv1, INPUT_PULLUP);
	pinMode(buttonLv2, INPUT_PULLUP);
	pinMode(buttonLv3, INPUT_PULLUP);


	// Thruster activate
	thruster.writeMicroseconds(1500); // send "stop" signal to ESC.
	delay(1000);					  // delay to allow the ESC to recognize the stopped signal


	Serial.begin(19200);		      // Serial debugger begins at 9600bps
}

void loop()
{
	ButtonRead();
	Accelerate();
}

// Stearing
void ButtonRead() {

	// Get values from buttons
	buttonVal1 = digitalRead(buttonLv1);
	buttonVal2 = digitalRead(buttonLv2);
	buttonVal3 = digitalRead(buttonLv3);

	//////////////////////////  TODO: 
	//////////////////////////  Turn around the "if statements" and insert return ex. Check three buttons first, check two button,
	//////////////////////////  then last check individuals
	//////////////////////////


	// Check which buttons is pressed

	TurnOfLight();
	if (buttonVal1)
	{
		newSpeed = speedLevel1; //Button 1 is pressed set new acceleration value;
		digitalWrite(ledLampRed1, HIGH);
	}

	if (buttonVal2)
	{
		newSpeed = speedLevel3; //Button 2 is pressed set new acceleration value;
		digitalWrite(ledLampBlue, HIGH);
	}

	if (buttonVal1 && buttonVal2)
	{
		newSpeed = speedLevel2; //Button 1 and 2 is pressed set new acceleration value;
		digitalWrite(ledLampGreen, HIGH);

	}

	if (buttonVal3)
	{
		newSpeed = speedLevel5; //Button 3 is pressed set new acceleration value;
		digitalWrite(ledLampRed2, HIGH);

	}

	if (buttonVal3 && buttonVal2)
	{
		newSpeed = speedLevel4; //Button 3 and 4 is pressed set new acceleration value;
		digitalWrite(ledLampYellow, HIGH);

	}

	if (buttonVal1 && buttonVal2 && buttonVal3)
	{
		newSpeed = speedLevel3; // All buttons is pressed
	}

	if (!buttonVal1 && !buttonVal2 && !buttonVal3)
	{
		currentSpeed = 1500;
		newSpeed = currentSpeed; // No buttons is pressed
		thruster.writeMicroseconds(currentSpeed);
		digitalWrite(ledLampWhite, HIGH);

	}


	// Print values in debugger
	Serial.print("Button val1: ");
	Serial.print(buttonVal1);
	Serial.print(" val2: ");
	Serial.print(buttonVal2);
	Serial.print(" val3: ");
	Serial.println(buttonVal3);
}


// Thruster method
bool Accelerate() {


	if (millis() - timerThruster > delayTime)  // check for new aceleration value 
	{
		if (newSpeed < currentSpeed)  // Lower speedValue
		{


			currentSpeed -= 1;
			thruster.writeMicroseconds(currentSpeed);
			delayTime = 100;
		}
		else if (newSpeed > currentSpeed) // Higher speedValue
		{


			currentSpeed += 1;
			delayTime = 250;
			thruster.writeMicroseconds(currentSpeed);
		}
		else
		{


			newSpeed == currentSpeed;
			thruster.writeMicroseconds(currentSpeed);
		}
		timerThruster = millis();
	}
}


void TurnOfLight() {
	digitalWrite(ledLampRed1, LOW);
	digitalWrite(ledLampRed2, LOW);
	digitalWrite(ledLampBlue, LOW);
	digitalWrite(ledLampWhite, LOW);
	digitalWrite(ledLampYellow, LOW);
	digitalWrite(ledLampGreen, LOW);
}

// Prototyping
void JoystickReader() {


	int joystickVal = analogRead(joystickPort);			        // Read joystick

	joystickVal = constrain(joystickVal, 512, 1023);		// Constrain joystick value

	int newVal = map(joystickVal, 512, 1023, 1500, 1900);   // map value from 512-1023 to 1500-1900

	thruster.writeMicroseconds(newVal);					    // Send signal to ESC.

}