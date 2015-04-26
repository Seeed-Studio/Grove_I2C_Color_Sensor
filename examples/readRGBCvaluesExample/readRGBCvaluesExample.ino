/****************************************************************************/	
//  Hardware: Grove - I2C Color Sensor
//  Arduino IDE: Arduino-1.6
//  
//  Author: Isaac Drafton
//  Version: 1.0
//  Based on the library by FrankieChu - www.seeedstudio.com
//
/******************************************************************************/
//
//  This example will initialize the Grove Color Sensor, read the RGB and 
//  clear channel values and then print the values.
//  SERIAL_PORT_SPEED is defined in Utilities.h
//
/******************************************************************************/

#include <Wire.h>
#include <GroveColorSensor.h>
#include <Utilities.h>

GroveColorSensor colorSensor;

void setup()
{
	Serial.begin(SERIAL_PORT_SPEED);
	Wire.begin();
	colorSensor.init();
}

void loop()
{
	colorSensor.readRGBC();
	colorSensor.printRGBC();		
	delay(1000);
}
