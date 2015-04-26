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
//  This example will initialize the Grove Color Sensor, calculate the color based
//  on pre-measured data and then print it.
//  To use this functionality, you must first adapt PreMeasuredColors.h 
//  and calibrate according to your case.
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
	colorSensor.calculateColor();
	Serial.println(colorSensor.getColor());		
	delay(1000);
}