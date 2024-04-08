/*
 * main.c
 *
 *  Created on: Apr 7, 2024
 *      Author: Abdelrahman (Ta'laab) Mohammed
 *		Github: https://github.com/AbdelrahmanMohammed002
 */

#include "../LIB/BIT_MATH.h"
#include "../LIB/STD_TYPES.h"

#include "../MCAL/DIO_DRIVER/DIO_Interface.h"
#include "../MCAL/GIE_DRIVER/GIE_Interface.h"

#include "../HAL/LCD_DRIVER/LCD_Interface.h"
#include "../HAL/ULTRASONIC_DRIVER/ULTRASONIC_Interface.h"

#include <stdlib.h>

#include <util/delay.h>

#define F_CPU 8000000UL

int main(int argc, char **argv) {

	u16 local_u16Distance;

	HLCD_vInit();
	u8 local_u8LCDString[] = "Distance is ";
	HLCD_vSendString(local_u8LCDString);

	HUltrasonic_vInit();

	MGIE_vEnableInterrupts();

	while (1) {

		local_u16Distance = HUltrasonic_u16ReadDistance();
		HLCD_vMoveTo(0, 12);
		HLCD_vINTtoString(local_u16Distance);

	}
}
