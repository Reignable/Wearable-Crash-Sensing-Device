/**
 * Accelerometer.cpp
 * Purpose: Allows for reading of accelerometer data from the sensor.
 * 			Converts ADC data to G force.
 *
 * @author Joe Barrett
 * @version 0.3 24/03/2016
 */

#include "Accelerometer.h"

/**
 *	Constructor. Takes and sets the pins each axis is connected to and the values required
 *	for conversion of ADC data to G force.
 *
 *	@param _xPin - int - The pinout the X axis is connected to
 *	@param _yPin - int - The pinout the Y axis is connected to
 *	@param _zPin - int - The pinout the Z axis is connected to
 *	@param _refVolt - double - The reference voltage of the ADC used, normally 3.30 or 5.00
 *	@param _zeroGVolt - double - The voltage output by the sensor when it is not subject to movement.
 *									This can be found in the sensor's datasheet
 *	@param _sensitivity - double - The sensitivity of the sensor expressed in mV/g. Found in the sensor's datasheet
 */
Accelerometer::Accelerometer(
  String _name,
  int _xPin,
  int _yPin,
  int _zPin,
  double _refVolt,
  double _adcBits,
  double _zeroGVolt,
  double _sensitivity):
  Sensor() {
  name = _name;
  xPin = _xPin;
  yPin = _yPin;
  zPin = _zPin;
  refVolt = _refVolt;
  adcBits = _adcBits;
  zeroGVolt = _zeroGVolt;
  sensitivity = _sensitivity;
}

/**
 *	Reads the raw data from each axis and sends it to
 *	the super readPins method to be smoothed.
 */
void Accelerometer::readPins() {
  //Serial.println("ACC reading pins");
  Sensor::readPins(analogRead(xPin), analogRead(yPin), analogRead(zPin));
}

/**
 *	Converts ADC data to G force. The ADC value is multiplied by the
 *	reference voltage of the ADC, this is then divided by 2^ of the ADC size.
 * 	e.g. a 10bit ADC uses the value 2^10 = 1023.
 * 	The zero voltage is then taken from this value and the entire result is
 *	divided by the sensor's sensitivity.
 */
void Accelerometer::calculate() {
  readPins();
  xCalculated = (xAverage * refVolt / adcBits - zeroGVolt) / sensitivity;
  yCalculated = (yAverage * refVolt / adcBits - zeroGVolt) / sensitivity;
  zCalculated = (zAverage * refVolt / adcBits - zeroGVolt) / sensitivity;
}

/**
 *	Appends the sensor type to the printRaw function
 *	in the Sensor class.
 */
void Accelerometer::printRaw() {
  Serial.print(name+" =\t");
  Sensor::printRaw();
}

/**
 *	Appends the sensor type to the printCalculated function
 *	in the Sensor class.
 */
void Accelerometer::printCalculated() {
  Serial.print(name+" =\t");
  Sensor::printCalculated();
}

