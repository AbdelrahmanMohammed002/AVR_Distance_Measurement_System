/*
 * ULTRASONIC_Interface.h
 *
 *  Created on: Apr 7, 2024
 *      Author: Abdelrahman (Ta'laab) Mohammed
 *		Github: https://github.com/AbdelrahmanMohammed002
 */

#ifndef COTS_HAL_ULTRASONIC_DRIVER_ULTRASONIC_INTERFACE_H_
#define COTS_HAL_ULTRASONIC_DRIVER_ULTRASONIC_INTERFACE_H_

#include "../../LIB/STD_TYPES.h"

/**
 * @brief Initialize the ultrasonic sensor module.
 *
 * This function initializes the ultrasonic sensor by configuring the necessary GPIO pins
 * and timers for proper operation.
 */
void HUltrasonic_vInit(void);

/**
 * @brief Trigger the ultrasonic sensor to start distance measurement.
 *
 * This function triggers the ultrasonic sensor by sending a pulse on the trigger pin.
 * It initiates the process of measuring distance using the sensor.
 */
void HUltrasonic_vTrigger(void);

/**
 * @brief Read the measured distance from the ultrasonic sensor.
 *
 * This function reads the distance measured by the ultrasonic sensor.
 * It returns the measured distance in centimeters as an unsigned 16-bit integer.
 *
 * @return Unsigned 16-bit integer representing the distance in centimeters.
 */
u16 HUltrasonic_u16ReadDistance(void);

#endif /* COTS_HAL_ULTRASONIC_DRIVER_ULTRASONIC_INTERFACE_H_ */
