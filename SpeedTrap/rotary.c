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

volatile uint8_t changed = 0; // used in project.c
volatile uint8_t threshold = 1;

volatile uint8_t new_state, old_state;

// Display LCD values
// speed threshold from 1 cm/sec to 99 cm/sec as an integer value
        // make sure rotary encoder values don't go above or below that
// Whenever a new threshold value is set, it is stored in the non-volatile EEPROM

// speed is compared to threshold (all integers) and buzzer sounds if above


// This ISR is called every time there is a pin change interrupt in PORT D
ISR(PCINT2_vect) // Double check method name
{
    uint8_t rotX = PIND;
    uint8_t rotA = (rotX & (1 << PD2)) >> PD2;
    uint8_t rotB = (rotX & (1 << PD3)) >> PD3;
	if (old_state == 0) {
	    // Handle A and B inputs for state 0
		if(rotA && !rotB) {
			new_state = 1;
			threshold++;
		}
		else if(!rotA && rotB) {
			new_state = 2;
			threshold--;
		}

	}
	else if (old_state == 1) {
	    // Handle A and B inputs for state 1
		if(rotA && rotB) {
			new_state = 3;
			threshold++;
		}
		else if(!rotA && !rotB) {
			new_state = 0;
			threshold--;
		}
	}

	else if (old_state == 2) {
	    // Handle A and B inputs for state 2
		if(!rotA && !rotB) {
			new_state = 0;
			threshold++;
		}
		else if(rotA && rotB) {
			new_state = 3;
			threshold--;
		}
	}

	else {   // old_state = 3
	    // Handle A and B inputs for state 3
		if(!rotA && rotB) {
			new_state = 2;
			threshold++;
		}
		else if(rotA && !rotB) {
			new_state = 1;
			threshold--;
		}
	}

	if (threshold > 99)
		threshold = 99;
	else if (threshold < 1)
		threshold = 1;

	// If state changed, update the value of old_state,
	// and set a flag that the state has changed.
	if (new_state != old_state) {
	    old_state = new_state;
		// update threshold value in EEPROM	
		eeprom_update_byte((void *) 100, threshold);
	}
}