/*
 * main.c
 *
 *  Created on: Sep 23, 2020
 *      Author: Khalid Muhammad
 */


#include "lcd.h"
#include "adc.h"
#include "timers.h"
#include "external_interrupts.h"


void reverse(void);


int main() {
	uint16 digital;
	ADC_ConfigType ADC_config = {AVCC, DISABLE_INT, DISABLE_TRIG, FREE_RUN};
	TIMERS_ConfigType TIMER0_config = {TIMER0, FAST_PWM, F_CPU_8, NON_INVERTING, 0, 128};
	INT_ConfigType INT1_config = {INT_1, RISING_INT};

	DDRB |= 0x30;				/* set motor pins as output pins */
	SET_BIT(PORTB,PB4);			/* initialize IN2 */
	CLEAR_BIT(PORTB,PB5);		/* initialize IN1 */

	SREG |= (1<<7);
	LCD_init();
	ADC_init(&ADC_config);
	TIMERS_init(&TIMER0_config);
	INT_init(&INT1_config);
	INT_setCallBack(INT_1, reverse);
	LCD_displayString("ADC Value = ");

	while(1) {
		digital = ADC_readChannel(5);
		OCR0 = digital / 4;
		LCD_moveCursorTo(0,12);
		LCD_displayInteger(digital);
		LCD_displayString("   ");
	}
}

void reverse(void) {
	PORTB ^= 0x30;
}
