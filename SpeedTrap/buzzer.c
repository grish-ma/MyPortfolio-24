/********************************************
 *
 *  Name: Grishma Shukla
 *  Email: grishmas@usc.edu	
 *  Section: Wednesday 2:00 PM
 *  Assignment: Project: Speed Trap
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "lcd.h"

extern volatile uint16_t buzzer_count = 0;

// This is the timer for the buzzer
timer0_init()
{
	// CTC MODE
	TCCR0B |= (1 << WGM02);
	// frequency = 349 --> period is 0.002 --> we want to call the isr twice the amount of times (every 0.001 seconds)
	// Prescalar: 16,000,000 * 0.001 / 64 = 250
	OCR0A = 250;
	// Enable Timer Interrupt - "Output Compare Match Interrupt"
    TIMSK0 |= (1 << OCIE0A);
}

// ISR for the buzzer
ISR(TIMER0_COMPA_vect)
{
	// called every 1/(2*349) seconds
	// Toggle buzzer on and off
	PORTC ^= (1<<PC1);
	buzzer_count++; 
	if (buzzer_count >= 349)
	{
		buzzer_count = 0;
		TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00)); // Turn off timer
		PORTC &= ~(1<<PC1); // Making sure buzzer is off.
	}
}