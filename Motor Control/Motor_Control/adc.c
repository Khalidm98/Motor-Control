/* Driver for Atmega16 ADC module */

#include "adc.h"


volatile uint16 g_ADC = 0;


ISR(ADC_vect) {
	g_ADC = ADC;			/* Read ADC data after conversion complete */
}


void ADC_init(const ADC_ConfigType * const config_ptr) {
	/* Initialize ADMUX Register:
	 * REFS1:0 = xx		set reference voltage
	 * ADLAR   = 0		right adjust the conversion result
	 * MUX4:0  = 00000	select channel (trivial)
	 */
	
	/* set the reference voltage */
	ADMUX = (config_ptr->reference << 6);

	/* Initialize ADCSRA Register:
	 * ADEN    = 1		enable ADC module
	 * ADSC    = 0		don't start conversion
	 * ADATE   = x		ADC auto trigger
	 * ADIF    = 0		ADC interrupt flag
	 * ADIE    = x		ADC interrupt
	 * ADPS2:0 = xxx	prescaler
	 */

	/* ADC operates in range (50 : 200) kHz */
	{
		uint8 prescaler = 1;
		uint32 F_ADC = F_CPU;
		while (prescaler < 7) {
			F_ADC /= 2;
			if (F_ADC < 200000)
				break;
			prescaler++;
		}
		ADCSRA = prescaler;
	}

	/* enable / disable ADC interrupt */
	if (config_ptr->interrupt)
		SET_BIT(ADCSRA,ADIE);

	/* enable / disable ADC auto trigger and select its source */
	if (config_ptr->trigger) {
		SET_BIT(ADCSRA,ADATE);									/* enable auto trigger */
		SFIOR = (SFIOR & 0x1F) | (config_ptr->source << 5);		/* set trigger sources */
	}

	/* enable ADC */
	SET_BIT(ADCSRA,ADEN);
}

#ifdef POLLING
uint16 ADC_readChannel(const uint8 channel_num) {
	ADMUX = (ADMUX & 0xE0) | (channel_num & 0x1F);		/* set the channel number */
	SET_BIT(ADCSRA,ADSC); 								/* start conversion */
	while(BIT_IS_CLEAR(ADCSRA,ADIF));					/* wait for conversion to complete */
	SET_BIT(ADCSRA,ADIF);								/* clear ADIF */
	return ADC;											/* return the data register */
}
#else
void ADC_readChannel(const uint8 channel_num) {
	ADMUX = (ADMUX & 0xE0) | (channel_num & 0x1F);		/* set the channel number */
	SET_BIT(ADCSRA,ADSC); 								/* start conversion */
}
#endif

void ADC_deInit(void) {
	ADMUX = 0;
	ADCSRA = 0;
}
