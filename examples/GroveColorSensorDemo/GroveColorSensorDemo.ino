#include <Wire.h>
//#include <math.h>
#include <GroveColorSensor.h>

void setup()
{
	Serial.begin(9600);
	Wire.begin();
}

void loop()
{
	GroveColorSensor colorSensor;
	while(1)
	{
		colorSensor.readRGB();
		delay(300);
		colorSensor.clearInterrupt(); 
	}
}