#ifndef MICRO_CONFIG_H_
#define MICRO_CONFIG_H_

#ifndef F_CPU
#define F_CPU 1000000UL		/* 1MHz Clock frequency */
#endif  

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#endif /* MICRO_CONFIG_H_ */