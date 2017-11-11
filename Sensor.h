/**
 * Sensor.h
 * Purpose: To declare data members and functions for the Sensor class.
 *          The Sensor class provides calibration and data smoothing to
 *          any class which inherits it.
 *
 * @author Joe Barrett
 * @version 0.3 24/03/2016
 */

#pragma once
#include "Arduino.h"

class Sensor {

  protected:

    const int NUM_READINGS;
    int valueIndex;
    double  xReadings[8], xTotal, xAverage, xZero,
            yReadings[8], yTotal, yAverage, yZero,
            zReadings[8], zTotal, zAverage, zZero;

    /**
     * Prints averaged raw ADC data to serial
     */
    virtual void printRaw();

    /**
     * Prints calculated data to serial
     * in units specific to sensor type.
     */
    virtual void printCalculated();

    /**
     * Provides data smoothing for each axis on the sensor.
     * Sets *Average data member to be used for calculations.
     *
     * @param readX - double - reading for the X axis
     * @param readY - double - reading for the Y axis
     * @param readZ - double - reading for the Z axis
     */
    void readPins(double readX, double readY, double readZ);

    /**
     * Prints provided values to serial.
     *
     * @param xToPrint - double - value from the X axis to print
     * @param yToPrint - double - value from the Y axis to print
     * @param zToPrint - double - value from the Z axis to print
     */
    void print(double xToPrint, double yToPrint, double zToPrint);

  public:

    String name;
    double xCalculated, yCalculated, zCalculated;

    Sensor();
    virtual void readPins() = 0;
    virtual void calculate() = 0;

    /**
     * Sets zero data members for each axis from corresponding
     * pre-calculated average.
     */
    void setZero();

};
