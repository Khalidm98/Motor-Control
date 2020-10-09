/* Driver for Atmega16 ADC module */

#ifndef ADC_H_
#define ADC_H_


#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"


#define POLLING


typedef enum {
	AREF, AVCC, INTERNAL_2_56 = 3
} ADC_RefVolt;

typedef enum {
	DISABLE_INT, ENABLE_INT
} ADC_Interrupt;

typedef enum {
	DISABLE_TRIG, ENABLE_TRIG
} ADC_AutoTrigger;

typedef enum {
	FREE_RUN, ANALOG_COMP, EXT_INT0, T0_COMP, T0_OVER, T1_COMP_B, T1_OVER, T1_CAPTURE
} ADC_TriggerSource;

typedef struct {
	ADC_RefVolt reference;
	ADC_Interrupt interrupt;
	ADC_AutoTrigger trigger;
	ADC_TriggerSource source;
} ADC_ConfigType;


/* Extern public global variable holding the ADC value */
extern volatile uint16 g_ADC;


/* Initialize the ADC module */
void ADC_init(const ADC_ConfigType * const config_ptr);

/* Read analog data from a certain ADC channel
 * and convert it to digital value */
#ifdef POLLING
uint16 ADC_readChannel(const uint8 channel_num);
#else
void ADC_readChannel(const uint8 channel_num);
#endif

/* Disable the ADC module */
void ADC_deInit(void);


#endif /* ADC_H_ */
