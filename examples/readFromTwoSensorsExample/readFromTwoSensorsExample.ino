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
//  This example will initialize two Grove Color Sensors, connected to a MUX,
//  read the RGB and clear channel values and then print the values.
//  SERIAL_PORT_SPEED is defined in Utilities.h
//
//  To use two or more Grove Color sensors, you need to use a multiplexer because
//  all sensors of this type have the same I2C address.
//  The following example is for 74HC4052 MUX.
//  Connections:
//
//  Sensor01 VCC 	----> Arduino VCC
//  Sensor01 GND	----> Arduino GND
//  Sensor02 VCC 	----> Arduino VCC
//  Sensor02 GND	----> Arduino GND
//  74HC4052 VCC	----> Arduino VCC
//  74HC4052 GND	----> Arduino GND
//  74HC4052 EN		----> Arduino GND
//  74HC4052 VEE	----> Arduino GND
//
//  Arduino  SDA 	----> 74HC4052 RX	(+ 4.7K pull-up resistor)
//  Arduino  SCL 	----> 74HC4052 TX 	(+ 4.7K pull-up resistor)
//  Arduino  PIN12 	----> 74HC4052 SEL0 (+ 10K pull-down resistor)
//  Arduino  PIN11 	----> 74HC4052 SEL1 (+ 10K pull-down resistor)
//  Sensor01 SDA 	----> 74HC4052 RX1	(+ 4.7K pull-up resistor)
//  Sensor01 SCL 	----> 74HC4052 TX1	(+ 4.7K pull-up resistor)
//  Sensor02 SDA 	----> 74HC4052 RX2	(+ 4.7K pull-up resistor)
//  Sensor02 SCL 	----> 74HC4052 TX2	(+ 4.7K pull-up resistor)
//
//  Activate functions must be called before initializing and reading from
//  the two sensors.
/******************************************************************************/

#include <Wire.h>
#include <GroveColorSensor.h>
#include <Utilities.h>

GroveColorSensor leftColorSensor;
GroveColorSensor rightColorSensor;

void setup()
{
  setupMuxPins();
  Serial.begin(SERIAL_PORT_SPEED);
  Wire.begin();

  ActivateLeftColorSensor();
  leftColorSensor.init();

  ActivateRightColorSensor();
  rightColorSensor.init();
}

void loop()
{
  ActivateLeftColorSensor();
  Serial.println("Left: ");
  leftColorSensor.readRGBC();
  leftColorSensor.printRGBC();
  delay(1000);

  ActivateRightColorSensor();
  Serial.println("Right: ");
  rightColorSensor.readRGBC();
  rightColorSensor.printRGBC();
  delay(1000);
}
