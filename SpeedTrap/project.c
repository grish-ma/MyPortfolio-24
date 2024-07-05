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

volatile uint8_t display = 0;  // flag to update lcd display only every 0.1 seconds
uint32_t count = 0; // timer count to display
volatile uint16_t overtime = 0;

extern volatile uint8_t new_state, old_state;
volatile uint16_t buzzer_count;
volatile uint8_t timer_started = 0;

volatile uint8_t startchar_received = 0;
volatile uint8_t speed_received = 0;
volatile char remote_buffer[5];
volatile uint8_t remote_counter = 0;


volatile uint32_t speed = 0;
volatile uint8_t buzzer_played = 0;

volatile uint8_t threshold;
uint8_t remote_speed = 0;


int main(void) {
    // Initialize DDR and PORT registers and LCD
	lcd_init();
	DDRB |= (1 << PB5)| (1<< PB3)| (1 << PB4); // LED to indiciate timing in progress
		// Servo (PB3) should be output
		// Tri-state buffer (PB4) should be an output
	DDRC |= (1<< PC1); // Buzzer should be output
	DDRC |= (1 << PC2) | (1 << PC3); // RGB red and green segments (typo?)


	PORTB &= ~(1<<PB5); // Turn off timing LED
	PORTD |= (1 << PD2) | (1 << PD3); // Pull-up resistors
	PORTB &= ~(1<<PB4); // put a zero in the corresponding PORT bit for the Tri-State b
	PORTC &= ~(1 << PC3);	// Turn on Green on RGB LED by default
	PORTC |= (1 << PC2);	// Turn off red on RGB LED by default

	lcd_writecommand(1);

	/* Set the PCIE1 bit in the PCICR register to enable the pin change interrupts on Port C (PCINT1).
    Set the bits in the PCMSK1 mask register to enable interrupts for the PC1 and PC5 I/O lines. 
    (PCINT9 and PCINT13)
    Enable global interrupts.*/
    PCICR |= (1 << PCIE1);
    PCMSK1 |= ((1 << PCINT12) | (1 << PCINT13) | (1 << PCINT9)); // PortC is PCINT8-PCINT14 
	// Enables pin change interrupts for PC 4 and 5 (phototransistors) and PC1 (buzzer)

	PCICR |= (1 << PCIE2);
	PCMSK2 |= ((1 << PCINT18) | (1 << PCINT19)); // enable pin change interrupt for pd2 and 3 (rotary encoder)

    // Write a splash screen to the LCD
	lcd_stringout("EE109 Project");
    lcd_moveto(1, 0);
    lcd_stringout("Grishma Shukla");
    _delay_ms(2000);
    lcd_writecommand(1);
	sei();
	timer2_init();
	timer1_init();	
	timer0_init();
	serial_init();


	// Read the threshold value from the EEPROM
	threshold = eeprom_read_byte((void *) 100);
		// Check if value that was read is valid threshold value
	if (threshold > 99 || threshold < 1)
		threshold = 15; // Default value

	
	// Read the rotary encoder value for the first time.
    uint8_t rotX = PIND;
    uint8_t rotA = (rotX & (1 << PD2)) >> PD2;
    uint8_t rotB = (rotX & (1 << PD3)) >> PD3;

	if (!rotB && !rotA)
	    old_state = 0; //if b and a is 0
    else if (!rotB && rotA)
	    old_state = 1; //if b is 0 and a is 1
    else if (rotB && !rotA)
	    old_state = 2; //if b is 1 and a is 0
    else
	    old_state = 3; //if b and and a is 1

    new_state = old_state;


    while (1) { // Loop forever		

		lcd_moveto(1, 14);
		char buf1[20];
		snprintf(buf1, 20, "%2u", threshold);
		lcd_stringout(buf1);
		change_led();
		if (display)
		{
		// Displaying count value
			lcd_writecommand(1);
			lcd_moveto(0,0);
			char buf2[20];
			// snprintf(buf2, 20, "%u", count);
			// lcd_stringout(buf2);
			count = count*64/1000; // count now holds the time value in ms
			snprintf(buf2, 20, "%u ms", count);
			lcd_stringout(buf2);

			// display the speed calculated. distance = 58.42 mm
			speed = 58000/count;
			buzzer_played = 0;
			lcd_moveto(1,0);
			char buf3[15];
			// // speed is in mm/s
			snprintf(buf3, 15, "%u", speed/10);
			lcd_stringout(buf3);
			snprintf(buf3, 15, ".%u", speed%10);
			lcd_stringout(buf3);

			// minimum pulse width of 0.75ms, and the value for the maximum pulse width of 2.25ms
			// OCR2A = 11.72
			// OCR2A = 35.16
			// slope = -0.24
			OCR2A = ((-24)*(speed/10)+3500)/100;
			char send_buf [15];
			snprintf(send_buf, 15, "{%u}", speed);
			int count = 0;
			while (send_buf[count] != NULL)
			{
				tx_char(send_buf[count]); // speed value is in mm/s
				count++;
			}
			display = 0;	
		}
		if (overtime)
		{
			lcd_writecommand(1);
			lcd_moveto(0,0);
			lcd_writecommand(1);
			lcd_stringout("Overtime!");
			overtime = 0;
		}
		if ((int)(speed)/10 > (int)(threshold) && !buzzer_played)
		{
			// Start the timer running. 	// prescaler = 64 --> 011
			TCCR0B |= (1 << CS01)|(1 << CS00); // Now the corresponding ISR is being called every 50 ms
			buzzer_played = 1;
		}
		if (speed_received)
		{
			speed_received = 0;
			convert_speed();
		}
	}
}

change_led()
{
	// speed = 0;
	if (abs(remote_speed-speed) <= 30)// within 3 cm/s of each other
	{
		// Green light
		PORTC &= ~(1 << PC3);	// Turn on Green on RGB LED by default
		PORTC |= (1 << PC2);	// Turn off red on RGB LED by default
	}
	else if (remote_speed > speed)
	{
		// Red LED light up
		PORTC &= ~(1 << PC2);
		PORTC |= (1 << PC3);
	}
	else if (remote_speed < speed)
	{
		// Blue LED light up
		// Through hardware --> if red AND green are both 1's, blue is 0.
		PORTC |= (1 << PC2);
		PORTC |= (1 << PC3);
	}
}

// Converting a string to integer
convert_speed()
{
	lcd_moveto(1,7);
	sscanf(remote_buffer, "%d", &remote_speed);

	char bufA[5];
	snprintf(bufA, 5, "%d", remote_speed/10);
	lcd_stringout(bufA);

	char bufB[5];
	snprintf(bufB, 5, ".%d", remote_speed%10);
	lcd_stringout(bufB);
}

// This ISR is called every time there is a pin change interrupt in PORT C!
ISR(PCINT1_vect)
{
	uint8_t a = (PINC & (1 << PC4)) >> PC4;
	uint8_t b = (PINC & (1 << PC5)) >> PC5; // Need to take a new "snapshot" to see if b is blocked

	// a == 0 when the beam is blocked.
	if (!a)
	{
	/*START TIMER:*/
		// Clearing the TCNT1 register and start the timer count.
		TCNT1 = 0;
		PORTB |= (1<<PB5); // Turn on LED
		// Starting the clock: set Clock Source bits (CS12, CS11, CS10) to 
		// the correct values for the prescaler.

		// count = 16MHz * time / prescalar. Max count = 65,536
		// prescaler = 1024 --> 101
		TCCR1B |= (1 << CS12)|(1<<CS10);
		timer_started = 1;
	}
	if (!b && timer_started)
	{
		/*STOP TIMER: */ 
		// lcd_moveto(1, 0); lcd_stringout("Stop!");
		// When no clock source is selected (CS12:0 = 0) the timer is stopped.
		TCCR1B &= ~((1<<CS10)|(1<<CS11)|(1<<CS12)); // Stops the speed timer
		PORTB &= ~(1<<PB5); // Turn off LED
		display = 1;
		count = TCNT1;
		timer_started = 0;
	}
}

void timer2_init()
{
	TCCR2A |= (0b11 << WGM20);  // Fast PWM mode, modulus = 256
	TCCR2A |= (0b10 << COM2A0); // Turn D11 on at 0x00 and off at OCR2A
	// CTC MODE
	// TCCR2B |= (1 << WGM22);
	// Prescaler = 1024
	TCCR2B |= (0b111 << CS20);
	OCR2A = 35;

	// Enable Timer Interrupt - "Output Compare Match Interrupt"
    TIMSK2 |= (1 << OCIE2A);

	// minimum pulse width of 0.75ms, and the value for the maximum pulse width of 2.25ms
	// OCR2A = 11.72
	// OCR2A = 35.16
}

void timer1_init()
{
	// CTC MODE 
	TCCR1B |= (1 << WGM12);

    // Prescalar: 16,000,000 * 4 / 1024 = 62500 --> calls ISR at 4 seconds
    OCR1A = 62500; // modulus value

    // Enable Timer Interrupt - "Output Compare Match Interrupt"
    TIMSK1 |= (1 << OCIE1A);
}

// This method is called at 4 seconds, based on the timer 
// intialization in the timer1_init() function.
ISR(TIMER1_COMPA_vect)
{
	// Terminate the timer because 4 seconds have passed
	TCCR1B &= ~((1<<CS10)|(1<<CS11)|(1<<CS12)); // Stops the timer
	PORTB &= ~(1<<PB5); // Turn off LED
	overtime = 1;
}

// We do not need to perform any actions in this ISR
ISR(TIMER2_COMPA_vect)
{}