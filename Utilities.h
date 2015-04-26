/****************************************************************************/	
//  Hardware: Grove - I2C Color Sensor
//  Arduino IDE: Arduino-1.6
//  
//  Author: Isaac Drafton
//  Version: 1.0
//  Based on the library by FrankieChu - www.seeedstudio.com
//
/******************************************************************************/
#ifndef UTILITIES
#define UTILITIES

#define SERIAL_PORT_SPEED  9600

#define SELECT_PIN1 12
#define SELECT_PIN2 11

void setupMuxPins()
{
	pinMode(SELECT_PIN1, OUTPUT);
    pinMode(SELECT_PIN2, OUTPUT);
}

void ActivateLeftColorSensor()
{
  digitalWrite(SELECT_PIN2,LOW);
  digitalWrite(SELECT_PIN1,HIGH);
}

void ActivateRightColorSensor()
{
  digitalWrite(SELECT_PIN2,LOW);
  digitalWrite(SELECT_PIN1,LOW);
}

#endif
