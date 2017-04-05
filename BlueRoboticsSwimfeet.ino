#include <Servo.h>
Servo thrusterRight;
Servo thrusterLeft;

// Pinout declaration
#define thrusterRightPort 9
#define thrusterLeftPort 10
#define buttonLv1	 5
#define buttonLv2	 6
#define buttonLv3	 7



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

// Thruster default values
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
	thrusterRight.attach(thrusterRightPort);
	thrusterLeft.attach(thrusterLeftPort);
	pinMode(buttonLv1, INPUT_PULLUP);
	pinMode(buttonLv2, INPUT_PULLUP);
	pinMode(buttonLv3, INPUT_PULLUP);

				 
	// Thruster activate
	delay(3250);
	thrusterRight.writeMicroseconds(1500); // send "stop" signal to ESC.
	thrusterLeft.writeMicroseconds(1500); // send "stop" signal to ESC.
	delay(1500);					     // delay to allow the ESC to recognize the stopped signal


	//Serial.begin(9600);		      // Serial debugger begins at 9600bps
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

	//// Print values in debugger
	//Serial.print("Button val1: ");
	//Serial.print(buttonVal1);
	//Serial.print(" val2: ");
	//Serial.print(buttonVal2);
	//Serial.print(" val3: ");
	//Serial.println(buttonVal3);


	// Check which buttons is pressed?

	// Put values into arrays for easy enumrations in loops
	int ledLampsArray[]{ 2,3,4,5,6,7 };
	int speedLevelArray[]{ speedLevel1,speedLevel2, speedLevel3, speedLevel4, speedLevel5 };
	bool buttonValArray[]{ buttonVal1,buttonVal2,buttonVal3 };

	if (!buttonVal1 && !buttonVal2 && !buttonVal3)
	{
		currentSpeed = 1500;		// No buttons is pressed
		newSpeed = currentSpeed;	// STOP
		thrusterRight.writeMicroseconds(currentSpeed);
		digitalWrite(ledLampsArray[5], HIGH);
		return;
	}

	for (size_t i = 0; i < sizeof(buttonValArray); i++)
	{
		if (i < (sizeof(buttonValArray) - 1))
		{
			if (buttonValArray[i] && buttonValArray[i + 1])
			{
				if (buttonValArray[0] && buttonValArray[1])
				{
					newSpeed = speedLevelArray[i+1]; 
					digitalWrite(ledLampsArray[i+1], HIGH);
					return;
				}
				else
				{
					newSpeed = speedLevelArray[i+2]; 
					digitalWrite(ledLampsArray[i+2], HIGH);
					return;
				}
			}
		}
		if (buttonValArray[i])
		{
			newSpeed = speedLevelArray[i*2];
			digitalWrite(ledLampsArray[i*2], HIGH);
			return;
		}
	}
}


// Thruster method
bool Accelerate() {


	if (millis() - timerThruster > delayTime)  // check for new aceleration value 
	{
		if (newSpeed < currentSpeed)  // Lower speedValue
		{
			currentSpeed -= 1;
			delayTime = 2;
			thrusterRight.writeMicroseconds(currentSpeed);
			thrusterLeft.writeMicroseconds(currentSpeed);
		}
		else if (newSpeed > currentSpeed) // Higher speedValue
		{
			currentSpeed += 1;
			delayTime = 5;
			thrusterRight.writeMicroseconds(currentSpeed);
			thrusterLeft.writeMicroseconds(currentSpeed);
		}
		else
		{
			newSpeed == currentSpeed;
			thrusterRight.writeMicroseconds(currentSpeed);
			thrusterLeft.writeMicroseconds(currentSpeed);
		}
		timerThruster = millis();
		Serial.print("newSpeed: ");
		Serial.println(newSpeed);
	}
}

