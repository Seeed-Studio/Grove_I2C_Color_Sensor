#include "GroveColorSensor.h"
#include <Wire.h>

// Default constructor
GroveColorSensor::GroveColorSensor()
	: triggerMode_(INTEG_MODE_FREE)
	, interruptSource_(INT_SOURCE_GREEN)
	, interruptMode_(INTR_LEVEL | INTR_PERSIST_EVERY)
	, gainAndPrescaler_(GAIN_1 | PRESCALER_4)
	, sensorAddress_(COLOR_SENSOR_ADDR)
{	
	GroveColorSensor::setTimingReg(); 
	GroveColorSensor::setInterruptSourceReg();  
	GroveColorSensor::setInterruptControlReg(); 
	GroveColorSensor::setGain(); 
	GroveColorSensor::setEnableADC(); 
}

// Constructor with parameters
GroveColorSensor::GroveColorSensor(
	  const int& triggerMode
	, const int& interruptSource
	, const int& interruptMode
	, const int& gainAndPrescaler
	, const int& sensorAddress)
	: triggerMode_(triggerMode)
	, interruptSource_(interruptSource)
	, interruptMode_(interruptMode)
	, gainAndPrescaler_(gainAndPrescaler)
	, sensorAddress_(sensorAddress) 
{}

void GroveColorSensor::setTimingReg()
{
	Wire.beginTransmission(sensorAddress_);
	Wire.write(REG_TIMING);
	Wire.write(triggerMode_);
	Wire.endTransmission();  
	delay(10); 
}

void GroveColorSensor::setInterruptSourceReg()
{
	Wire.beginTransmission(sensorAddress_);
	Wire.write(REG_INT_SOURCE);
	Wire.write(interruptSource_);
	Wire.endTransmission();  
	delay(10);
}

void GroveColorSensor::setInterruptControlReg()
{
	Wire.beginTransmission(sensorAddress_);
	Wire.write(REG_INT);
	Wire.write(interruptMode_);
	Wire.endTransmission();  
	delay(10);
}

void GroveColorSensor::setGain()
{
	Wire.beginTransmission(sensorAddress_);
	Wire.write(REG_GAIN);
	Wire.write(gainAndPrescaler_);
	Wire.endTransmission();
}

void GroveColorSensor::setEnableADC()
{
	Wire.beginTransmission(sensorAddress_);
	Wire.write(REG_CTL);
	Wire.write(CTL_DAT_INIITIATE);
	Wire.endTransmission();  
	delay(10);  
}

void GroveColorSensor::clearInterrupt()
{
	Wire.beginTransmission(sensorAddress_);
	Wire.write(CLR_INT);
	Wire.endTransmission(); 
}

void GroveColorSensor::readRGB()
{
	Wire.beginTransmission(sensorAddress_);
	Wire.write(REG_BLOCK_READ);
	Wire.endTransmission();
	
	Wire.beginTransmission(sensorAddress_);
	Wire.requestFrom(sensorAddress_, 8);
	delay(100);
	
	// if two bytes were received
	if(8 <= Wire.available())
	{
		int i;
		for(i = 0; i < 8; ++i)
		{
			readingdata_[i] = Wire.read();
			//Serial.println(readingdata_[i], BIN);
		}
	}
	green_	= readingdata_[1] * 256 + readingdata_[0];
	red_ 	= readingdata_[3] * 256 + readingdata_[2];
	blue_	= readingdata_[5] * 256 + readingdata_[4];
	clear_	= readingdata_[7] * 256 + readingdata_[6];
	
	Serial.println("The RGB value and Clear channel value are: ");
	Serial.println(red_,DEC);
	Serial.println(green_,DEC);
	Serial.println(blue_,DEC);
	Serial.println(clear_,DEC);  
}

void GroveColorSensor::readRGB(int *red, int *green, int *blue)
{
	Wire.beginTransmission(sensorAddress_);
	Wire.write(REG_BLOCK_READ);
	Wire.endTransmission();
	
	Wire.beginTransmission(sensorAddress_);
	Wire.requestFrom(sensorAddress_, 8);
	delay(100);
	
	// if two bytes were received
	if(8 <= Wire.available())
	{
		int i;
		for(i = 0; i < 8; ++i)
		{
			readingdata_[i] = Wire.read();
			//Serial.println(readingdata_[i], BIN);
		}
	}
	green_	= readingdata_[1] * 256 + readingdata_[0];
	red_ 	= readingdata_[3] * 256 + readingdata_[2];
	blue_	= readingdata_[5] * 256 + readingdata_[4];
	clear_	= readingdata_[7] * 256 + readingdata_[6];
	
	*red   = red_;
	*green = green_;
	*blue  = blue_;
	
	Serial.println("The RGB value are: ");
	Serial.println(red_,DEC);
	Serial.println(green_,DEC);
	Serial.println(blue_,DEC);
}

void GroveColorSensor::calculateCoordinate()
{
	double X;
	double Y;
	double Z;
	double x;
	double y;
	
	X = (-0.14282) * red_ + (1.54924) * green_ + (-0.95641) * blue_;
	Y = (-0.32466) * red_ + (1.57837) * green_ + (-0.73191) * blue_;
	Z = (-0.68202) * red_ + (0.77073) * green_ + (0.563320) * blue_;
	
	x = X / (X + Y + Z);
	y = Y / (X + Y + Z);
	
	if( (X > 0) && ( Y > 0) && ( Z > 0) )
	{
		Serial.println("The x,y values are(");
		Serial.print(x, 2);
		Serial.print(" , ");
		Serial.print(y, 2);
		Serial.println(")");
	}
	else
		Serial.println("Error: overflow!");
}