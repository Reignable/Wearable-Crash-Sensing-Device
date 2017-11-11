/**
 * Accelerometer.h
 * Purpose: To declare data members and functions for the Accelerometer class.
 *          This allows for use of any triple axis accelerometer and conversion of
 *			ADC readings to G force.
 *
 * @author Joe Barrett
 * @version 0.3 24/03/2016
 */

#pragma once
#include "Arduino.h"
#include "Sensor.h"

class Accelerometer:
  public Sensor {

  private:
    /**
     *	Pins used for each axis
     */
    int xPin, yPin, zPin;

    /**
     * The reference voltage of the microcontroller's
     * ADC module
     */
    double refVolt;
    double adcBits;

    /**
    *  Values used for conversion of
    *  ADC data to G force. These vary
    *  between sensors.
    */
    double zeroGVolt;
    double sensitivity;

  public:

    /**
     *	Constructor.
     *
     *	@param _xPin - int - The pinout the X axis is connected to
     *	@param _yPin - int - The pinout the Y axis is connected to
     *	@param _zPin - int - The pinout the Z axis is connected to
     *	@param _refVolt - double - The reference voltage of the ADC used, normally 3.30 or 5.00
     *	@param _zeroGVolt - double - The voltage output by the sensor when it is not subject to movement.
     *									This can be found in the sensor's datasheet
     *	@param _sensitivity - double - The sensitivity of the sensor expressed in mV/g. Found in the sensor's datasheet
     */
    Accelerometer(String name, int _xPin, int _yPin, int _zPin, double _refVolt, double _adcBits, double _zeroGVolt, double _sensitivity);

    /**
     * Reads data from each axis.
     * Overrides Sensor::readPins()
     */
    void readPins() override;

    /**
     * Converts ADC readings to G force
     */
    void calculate() override;

    /**
     * Prints averaged raw ADC data to serial
     */
    void printRaw() override;

    /**
     * Prints calculated data to serial
     * in G
     */
    void printCalculated() override;
};


