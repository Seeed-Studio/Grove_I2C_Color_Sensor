/****************************************************************************/	
//  Hardware: Grove - I2C Color Sensor
//  Arduino IDE: Arduino-1.6
//  
//  Author: Isaac Drafton
//  Version: 1.0
//  Based on the library by FrankieChu - www.seeedstudio.com
//	
/******************************************************************************/
#include <GroveColorSensor.h>
#include <Math.h>
#include <Wire.h>

void GroveColorSensor::init()
{	
	GroveColorSensor::setTimingReg(); 
	GroveColorSensor::setInterruptSourceReg();  
	GroveColorSensor::setInterruptControlReg(); 
	GroveColorSensor::setGain(); 
	GroveColorSensor::setEnableADC(); 
}

void GroveColorSensor::setTimingReg()
{
	Wire.beginTransmission(COLOR_SENSOR_ADDR);
	Wire.write(REG_TIMING);
	Wire.write(INTEG_MODE_FREE);
	Wire.endTransmission();  
}

void GroveColorSensor::setInterruptSourceReg()
{
	Wire.beginTransmission(COLOR_SENSOR_ADDR);
	Wire.write(REG_INT_SOURCE);
	Wire.write(INT_SOURCE_GREEN);
	Wire.endTransmission();  
}

void GroveColorSensor::setInterruptControlReg()
{
	Wire.beginTransmission(COLOR_SENSOR_ADDR);
	Wire.write(REG_INT);
	Wire.write(INTR_LEVEL | INTR_PERSIST_EVERY);
	Wire.endTransmission();  
}

void GroveColorSensor::setGain()
{
	Wire.beginTransmission(COLOR_SENSOR_ADDR);
	Wire.write(REG_GAIN);
	Wire.write(GAIN_16 | PRESCALER_8);
	Wire.endTransmission();
}

void GroveColorSensor::setEnableADC()
{
	Wire.beginTransmission(COLOR_SENSOR_ADDR);
	Wire.write(REG_CTL);
	Wire.write(CTL_DAT_INIITIATE);
	Wire.endTransmission();  
}

void GroveColorSensor::clearInterrupt()
{
	Wire.beginTransmission(COLOR_SENSOR_ADDR);
	Wire.write(CLR_INT);
	Wire.endTransmission(); 
}

void GroveColorSensor::readRGBC()
{
	Wire.beginTransmission(COLOR_SENSOR_ADDR);
	Wire.write(REG_BLOCK_READ);
	Wire.endTransmission();
	
	Wire.beginTransmission(COLOR_SENSOR_ADDR);
	Wire.requestFrom(COLOR_SENSOR_ADDR, 8);
	
	short int readingdata[8];
	
	// if two bytes were received
	if(8 <= Wire.available())
	{
		for(short int i = 0; i < 8; ++i)
		{
			readingdata[i] = Wire.read();
		}
	}
	
	m_RGBC[0] = readingdata[3] * 256 + readingdata[2];
	m_RGBC[1] = readingdata[1] * 256 + readingdata[0];
	m_RGBC[2] = readingdata[5] * 256 + readingdata[4];
	m_RGBC[3] = readingdata[7] * 256 + readingdata[6];
}

void GroveColorSensor::printRGBC()
{
	Serial.println("The RGB and Clear values are: ");
	for (int i = 0; i < 4; ++i)
	{
		Serial.print(m_RGBC[i],DEC);
		Serial.print(" ");
	} 
}

void GroveColorSensor::calculateColor()
{
	GroveColorSensor::readRGBC();
	
	// Straightforward cases:
	// These cases were calibrated according to my project and can be adapted according to 
	// your specific case or removed. 
	//
	// Example: By looking at the pre-measured data, I observed that for the colors of my interest,
	// black had by far the lowest clear channel values. So when we read a color with it's clear channel
	// values less that 270, it's black.
	//
	// If you remove the following cases, the algorithm will continue with parsing the 
	// pre-measured data and return the most probable color.
	// ---------------------------------------------------------------------------
	// BLACK
	if ( m_RGBC[3] < 270)
	{
		GroveColorSensor::setColor(BLACK);
		return;
	}
	// WHITE
	if ( m_RGBC[3] > 1150)
	{
		GroveColorSensor::setColor(WHITE);
		return;
	}
	// RED
	if ( m_RGBC[0] >= 2 * m_RGBC[2])
	{
		GroveColorSensor::setColor(RED);
		return;
	}
	// BLUE
	if ( ( m_RGBC[0] * 2 <= m_RGBC[2]) && ( abs(m_RGBC[2] - m_RGBC[1]) < 20) )
	{
		GroveColorSensor::setColor(BLUE);
		return;
	}
	// GREEN
	if ( ( m_RGBC[0] * 2 <= m_RGBC[1]) && (m_RGBC[1] > (m_RGBC[2] + 50) ) )
	{
		GroveColorSensor::setColor(GREEN);
		return;
	}
	// ---------------------------------------------------------------------------
	
	// The probability array's items will be incremented when a match is found for each 
	// of the RGBC channel values. 
	// We have one array element for each pre-measured color in PreMeasuredColors.h.
	
	// Initializing all items in the probability array with 0
	short int probabilityArray[sizeOfPreMeasuredColorArray] = {};
	
	// Index corresponding to the highest probability matched color
	short int largestValueIndex = 0;
	short int temp = 0;
	
	// Parse all the pre-measured colors
	for (short int i = 0; i < sizeOfPreMeasuredColorArray ; ++i)
	{
		// Parse the 4 channels
		for (short int j = 0; j < 4; ++j)
		{			
			short int marginOfError = COLOR_CALC_MARGIN_OF_ERROR * m_RGBC[j];
			probabilityArray[i] = (marginOfError >= abs(m_RGBC[j] - preMeasuredColors[i][j])) ?
				(probabilityArray[i] + 1) : probabilityArray[i];
		}
		// If all 4 channel readings match the pre-measured color, store it.
		// Recommendation: In the pre-measured colors array, insert the most common expected colors first
		// so we will find the matched color fast, without parsing the whole pre-measured colors array.
		if (probabilityArray[i] == 4)
		{
			GroveColorSensor::setColor(PreMeasuredColor(i));
		}

		// Serch the probability array and find the largest index. 
		// This is the most probable color match and will be returned at the end.
		if ( temp < probabilityArray[i])
		{
			temp = probabilityArray[i];
			largestValueIndex = i;
		}
	}
	
	// Return the most probable color according to COLOR_CALC_ACCURACY.
	// Example: For a COLOR_CALC_ACCURACY equal to 3, we return the most 
	// probable color if 3 or more of it's channel readings match the pre-measured data.
	// If not, we return UNKNOWN.
	probabilityArray[largestValueIndex] >= 3 ? GroveColorSensor::setColor(PreMeasuredColor(largestValueIndex))
											 : GroveColorSensor::setColor(UNKNOWN);
	return;
}

void GroveColorSensor::setColor(const PreMeasuredColor& color)
{
	m_color = color;
}

PreMeasuredColor GroveColorSensor::getColor()
{
	return m_color;
}
	
#if COORDINATES_ENABLED

void GroveColorSensor::calculateCoordinates()
{	
	m_XYZ[0] = (-0.14282) * m_RGBC[0] + (1.54924) * m_RGBC[1] + (-0.95641) * m_RGBC[2];
	m_XYZ[1] = (-0.32466) * m_RGBC[0] + (1.57837) * m_RGBC[1] + (-0.73191) * m_RGBC[2];
	m_XYZ[2] = (-0.68202) * m_RGBC[0] + (0.77073) * m_RGBC[1] + (0.563320) * m_RGBC[2];
	
	m_xy[0] = m_XYZ[0] / (m_XYZ[0] + m_XYZ[1] + m_XYZ[2]);
	m_xy[1] = m_XYZ[1] / (m_XYZ[0] + m_XYZ[1] + m_XYZ[2]);
}

void GroveColorSensor::printCoordinates()
{
	if( (m_XYZ[0] > 0) && (m_XYZ[1] > 0) && (m_XYZ[2] > 0) )
	{
		Serial.println("The (x,y) values are(");
		Serial.print(m_xy[0], 2);
		Serial.print(" , ");
		Serial.print(m_xy[1], 2);
		Serial.println(")");
	}
	else
		Serial.println("Error: overflow!");
}
#endif
