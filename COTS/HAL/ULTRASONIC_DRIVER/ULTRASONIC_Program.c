/*
 * ULTRASONIC_Program.c
 *
 *  Created on: Apr 7, 2024
 *      Author: Abdelrahman (Ta'laab) Mohammed
 *      Github: https://github.com/AbdelrahmanMohammed002
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "ULTRASONIC_Interface.h"
#include "ULTRASONIC_Config.h"

#include "../../MCAL/DIO_DRIVER/DIO_Interface.h"
#include "../../MCAL/GIE_DRIVER/GIE_Interface.h"
#include "../../MCAL/TIMER1_DRIVER/TIMER1_Interface.h"
#include <util/delay.h>

// Global Variables
volatile u8 global_u8Flag = 0;
volatile u16 global_u16HighTime = 0;

// Function Prototypes
void ICU_ISR_UltrasonicEdgeProcessing(void);

// Function Definitions

/**
 * @brief Initialize the ultrasonic sensor module.
 *
 * This function initializes the ultrasonic sensor by configuring the trigger
 * pin as OUTPUT and the echo pin as INPUT. It also configures Timer1 for Input
 * Capture Unit (ICU) mode to measure the echo signal.
 */
void HUltrasonic_vInit(void) {
    // Configure Timer1 for ICU mode
    MTIMER1_vInit();
    // Configure ICU edge capture on Rising
    MTIMER1_vSetInputCaptureEdge(RISING);
    // Set the value of the ICR
    MTIMER1_vSetICR1Value(0);
    // Set the value of the TCNT1
    MTIMER1_vSetOVFRegister(0);
    // Set the callback function for the ICU's interrupt
    MTIMER1_vSetInputCaptureCallBack(&ICU_ISR_UltrasonicEdgeProcessing);
    // General interrupts must be enabled in main for the ICU interrupt
    MTIMER1_vEnableInputCaptureINT();
    // Configure the pins of the trigger and the echo pins
    MDIO_vSetPinDirection(ULTRASONIC_PORT, TRIGGER, OUTPUT);
    MDIO_vSetPinDirection(ULTRASONIC_PORT, ECHO, INPUT);
}

/**
 * @brief Trigger the ultrasonic sensor to start distance measurement.
 *
 * This function triggers the ultrasonic sensor by sending a 10 microseconds
 * pulse on the trigger pin.
 */
void HUltrasonic_vTrigger(void) {
    MDIO_vSetPinValue(ULTRASONIC_PORT, TRIGGER, HIGH);
    _delay_us(10);
    MDIO_vSetPinValue(ULTRASONIC_PORT, TRIGGER, LOW);
}

/**
 * @brief Read the distance measured by the ultrasonic sensor.
 *
 * This function reads the distance measured by the ultrasonic sensor based
 * on the high time captured by the Input Capture Unit (ICU).
 *
 * @note   - Sound velocity = 340.00 m/s = 34000 cm/s
 *         - As Signal travels from HCSR04 to object and returns to the module HCSR04
 *           The distance of Object (in cm) = (340000 * Time)/2
 *           = 17000 * Time
 *
 *         - As an internal 8MHz oscillator frequency is selected for ATmega32, with Prescaler F_CPU/8 for timer frequency.
 *         Then F(timer) = FCPU/8 = 1 MHz ---> T(timer) = 1us.
 *
 *         - Distance = 17000 x (TIMER value) x 1 x 10^-6 cm
 *         = 0.017 x (TIMER value) cm
 *
 *         Note TIMER VALUE is the pulse width time calculated by the ICU
 *
 * @return Unsigned 16-bit integer representing the distance in centimeters.
 */
u16 HUltrasonic_u16ReadDistance(void) {
    HUltrasonic_vTrigger(); // Trigger the ultrasonic sensor
    f32 local_u16Distance = 0;

    // Wait for the measurement to complete
    while (global_u8Flag != 0) {
        // busy wait
    }

    // Calculate distance based on high time and speed of sound
    local_u16Distance = (global_u16HighTime) * (0.0173);
    // Round up the distance for more accurate reading
    return local_u16Distance ;
}

/**
 * @brief Interrupt Service Routine for handling ultrasonic sensor echo signal.
 *
 * This ISR is triggered on both rising and falling edges of the echo signal
 * to measure the high time which corresponds to the distance.
 */
void ICU_ISR_UltrasonicEdgeProcessing(void) {
    global_u8Flag++;

    if (global_u8Flag == 1) {
        MTIMER1_vSetOVFRegister(0);
        MTIMER1_vSetInputCaptureEdge(FALLING);
    } else if (global_u8Flag == 2) {
        global_u16HighTime = MTIMER1_u16InputCaptureValue();
        MTIMER1_vSetInputCaptureEdge(RISING);
        global_u8Flag = 0;
    }
}
