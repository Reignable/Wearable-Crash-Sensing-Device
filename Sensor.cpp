/**
 * Sensor.cpp
 * Purpose: Smooths data provided by inheriting sensor and provides
 *          formatted printing of both raw and calculated data.
 *
 * @author Joe Barrett
 * @version 0.3 24/03/2016
 */

#include "Sensor.h"

/**
 * Constructor. Sets constant NUM_READINGS for the number of readings
 * used to create a smooth data average. Initializes valueIndex for
 * data smoothing algorithm.
 */
Sensor::Sensor(): NUM_READINGS(8) {
  valueIndex = 0;
}

/**
 * Prints raw ADC averages to serial using print()
 */
void Sensor::printRaw() {
  Sensor::print(xAverage, yAverage, zAverage);
}

/**
 * Prints calculated values to serial using print()
 */
void Sensor::printCalculated() {
  Sensor::print(xCalculated, yCalculated, zCalculated);
}

/**
 * Function to smooth raw ADC data provided by the sensor.
 * It takes the old reading from the running total. Replaces the
 * old reading with the new one provided. Adds the new reading to the
 * running total, then works out an average from the total and number
 * of readings in the array. The index to use is then incremented and
 * checked if it is out of the bounds of the readings array, if so
 * it is reverted to 0.
 *
 * @param readX - double - reading from the X axis to be added to readings array.
 * @param readY - double - reading from the Y axis to be added to readings array.
 * @param readZ - double - reading from the Z axis to be added to readings array.
 */
void Sensor::readPins(double readX, double readY, double readZ) {
  xTotal -= xReadings[valueIndex];
  yTotal -= yReadings[valueIndex];
  zTotal -= zReadings[valueIndex];
  xReadings[valueIndex] = readX;
  yReadings[valueIndex] = readY;
  zReadings[valueIndex] = readZ;
  xTotal += xReadings[valueIndex];
  yTotal += yReadings[valueIndex];
  zTotal += zReadings[valueIndex];
  xAverage = (xTotal/*-xZero*/) / NUM_READINGS;
  yAverage = (yTotal/*-yZero*/) / NUM_READINGS;
  zAverage = (zTotal/*-zZero*/) / NUM_READINGS;
  valueIndex++;
  if (valueIndex >= NUM_READINGS) {
    valueIndex = 0;
  }
}

/**
 * Prints provided values to serial tab separated
 * 
 * @param xToPrint - double - Value from X axis to print.
 * @param yToPrint - double - Value from Y axis to print.
 * @param zToPrint - double - Value from Z axis to print.
 */
void Sensor::print(double xToPrint, double yToPrint, double zToPrint) {
  Serial.print(xToPrint);
  Serial.print("\t");
  Serial.print(yToPrint);
  Serial.print("\t");
  Serial.print(zToPrint);
  //Serial.println();
}

/**
 * Sets the zero value for each axis from the
 * previously calculated average.
 */
void Sensor::setZero() {
  xZero = xAverage;
  yZero = yAverage;
  zZero = zAverage;
}
