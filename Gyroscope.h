/**
 * Gyroscope.h
 * Purpose: To declare data members and functions for the Gyroscope class.
 *          This provides functionality for an ITG-3200 using I2C communication.
 *          Datasheet - https://www.sparkfun.com/datasheets/Sensors/Gyro/PS-ITG-3200-00-01.4.pdf
 *
 * @author Joe Barrett
 * @version 0.2 24/03/2016
 */

#pragma once
#include "Arduino.h"
#include "Sensor.h"
//#include <Wire.h>
#include <i2c_t3.h>

class Gyroscope:
  public Sensor {

  private:

    /**
     * Constants for register addresses on the chip
     */

    /**
     * Contains the I2C address of the device
     * Hex: 0
     * Dec: 0
     */
    const char WHO_AM_I;

    /**
     * Determines the sample rate of the gyroscopes
     * Hex: 15
     * Dec: 21
     */
    const char SMPLRT_DIV;

    /**
     * Contains parameters related to data acquisition
     * Hex: 16
     * Dec: 22
     */
    const char DLPF_FS;

    /**
    * Sets low pass digital filter configuration and determines digital
    * sampling rate
    * Contained in DLPF_FS register
    */
    const char DLPF_CFG_0;
    const char DLPF_CFG_1;
    const char DLPF_CFG_2;

    /**
     * Sets full scale range of the gyroscopes
     * Contained in DLPF_FS register
     */
    const char DLPF_FS_SEL_0;
    const char DLPF_FS_SEL_1;

    /**
     * Registers containing data from the X axis gyroscope
     * H Hex: 1D
     * H Dec: 29
     * L Hex: 1E
     * L Dec: 30
     */
    const char GYRO_XOUT_H;
    const char GYRO_XOUT_L;

    /**
    * Registers containing data from the Y axis gyroscope
    * H Hex: 1F
    * H Dec: 31
    * L Hex: 20
    * L Dec: 32
    */
    const char GYRO_YOUT_H;
    const char GYRO_YOUT_L;

    /**
    * Registers containing data from the Z axis gyroscope
    * H Hex: 21
    * H Dec: 33
    * L Hex: 22
    * L Dec: 34
    */
    const char GYRO_ZOUT_H;
    const char GYRO_ZOUT_L;

    /**    
     *  Address of the ITG-3200 on the I2C bus.
     */
    char itgAddress;

    /**
     * Value for conversion of ADC readings to degrees per second
     */
    const double SCALE_FACTOR;

    /**
     * Writes data to sensor registers on the I2C bus
     * 
     * @param address - char - address of the sensor on the I2C bus
     * @param registerAddress - char - address of the register to be written to 
     * @param data - char - data to be written to the register
     */
    void itgWrite(char address, char registerAddress, char data);

    /**
     * Reads data from a specified register
     * 
     * @param address - char - address of the sensor on the I2C bus
     * @param registerAddress - char - address of the register to be written to 
     * @return unsigned char - the read value from the specified register
     */
    unsigned char itgRead(char address, char registerAddress);

    /**
     * Read raw ADC data from a gyroscope
     * 
     * @param high - char - H register for the gyroscope
     * @param low - char - L register for the gyroscope
     * @return int16_t - 16 bit reading from the gyroscope register
     */
    int16_t readRaw(char high, char low);

  public:
    Gyroscope();

    /**
     * Sets up the gyroscope for reading
     */
    void setupGyro();

    /**
     * Reads data from each axis.
     * Overrides Sensor::readPins()
     */
    void readPins() override;

    /**
     * Prints averaged raw ADC data to serial
     */
    void printRaw() override;

    /**
     * Converts ADC readings to degrees per second
     */
    void calculate() override;

    /**
     * Prints calculated data to serial
     * in degrees per second
     */
    void printCalculated() override;

};




