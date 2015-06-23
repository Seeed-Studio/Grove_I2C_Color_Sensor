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
	int red, green, blue;
	GroveColorSensor colorSensor;
	while(1)
	{
		colorSensor.readRGB(&red, &green, &blue);		//Read RGB values to variables.
		delay(300);
		Serial.print("The RGB value are: RGB( ");
		Serial.print(red,DEC);
		Serial.print(", ");
		Serial.print(green,DEC);
		Serial.print(", ");
		Serial.print(blue,DEC);
	    Serial.println(" )");
		colorSensor.clearInterrupt();
	}
}
