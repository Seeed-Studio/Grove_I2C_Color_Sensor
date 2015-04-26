/****************************************************************************/	
//	Hardware: Grove - I2C Color Sensor
//  Arduino IDE: Arduino-1.6
//  
//  Author: Isaac Drafton
//	Version: 1.0
//  Refactored version of the library by FrankieChu - www.seeedstudio.com
//	
/******************************************************************************/
#ifndef GROVECOLORSENSOR
#define GROVECOLORSENSOR
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <Registers.h>
#include <PreMeasuredColors.h>

// Modify the following macro to disable color coordinates processing
// 1 for enabled and 0 for disabled
#define COORDINATES_ENABLED 0
// Margin of error used in calculating the color based on pre-measured data
#define COLOR_CALC_MARGIN_OF_ERROR 0.25
// Accuracy of the color calculation based on pre-measured data. Maximum is 4.
#define COLOR_CALC_ACCURACY 3

class GroveColorSensor
{
public:

	// Initializer function
	void init();
	
	void clearInterrupt();
	// Reads the raw data from the sensor for red, green, blue and clear channels
	void readRGBC();

	void printRGBC();
	// Calculates the current color based on comparison with pre-measured RGBC data.
	void calculateColor();
	void setColor(const PreMeasuredColor& color);
	PreMeasuredColor getColor();
	
#if COORDINATES_ENABLED
	void calculateCoordinates();
	void printCoordinates();
#endif

private:
	
	short int m_RGBC[4];
	PreMeasuredColor m_color;
	
	// Configuration items
	// Set trigger mode. Including free mode, manually mode, single synchronization mode or so.
	void setTimingReg();
	// Set interrupt source
	void setInterruptSourceReg();
	// Set interrupt mode
	void setInterruptControlReg();
	// Set gain value and pre-scaler value
	void setGain();
	// Start ADC of the colour sensor
	void setEnableADC();

#if COORDINATES_ENABLED
	// Color coordinates
	// (x, y)
	double m_xy[2];
	// (X, Y, Z)
	double m_XYZ[3];
#endif

};

#endif