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


extern volatile uint8_t startchar_received;
extern volatile uint8_t speed_received;
extern volatile char remote_buffer[5];
extern volatile uint8_t remote_counter;

void serial_init()
{
	// Configuring the Arduino Serial ports and Tri-state buffer
	// UBRR = 16,000,000/(16*BAUD)-1
	// BAUD = 9600, UBRR = 103.1666
	uint32_t MYUBRR = 103;//(16000000/(16*9600))-1;
	UBRR0 = MYUBRR;

	// Enable receiver and/or transmitter
	UCSR0B |= (1 << TXEN0 | 1 << RXEN0); // Enable RX and TX
	UCSR0C = (3 << UCSZ00); // Async., no parity,
	// 1 stop bit, 8 data bits

	// Enable interrupts
	// Receiver interrupts are enabled by setting the RXCIE0 bit to a one in the UCSR0B register. 
	// When a character is received the hardware executes the ISR with the vector name "USART_RX_vect".
	UCSR0B |= (1<<RXCIE0);
}

// Sends data out of serial interface
void tx_char(char next_character)
{
	while ((UCSR0A & (1 << UDRE0)) == 0) { }
	UDR0 = next_character;
}

// This isr is called everytime an character is received.
ISR(USART_RX_vect)
{
	// 1. Need a 5 byte buffer to store the data
	// 2. global var flag to see whether start '{' was received
	// 3. var that counts how many data chars have been received + stored
	// 4. global var flag to see whether end '}' has been received AND valid speed in buffer
		// 0 while receiving the data
	char x;
	x = UDR0;

	// For the following chars: check if valid
	// 
	if (startchar_received)
	{
		if (!  ((x >= 48 && x <= 57) || x == '}')  ) // if 0-9 or } is not received, 
		{
			startchar_received = 0;	// not valid data
			speed_received = 0;
			remote_counter = 0;
		}
		// else: (if valid character is received):
		else
		{
			// if there is still space in the buffer
			if (remote_counter < 5)
			{
				// store the value, increment count
				remote_buffer[remote_counter] = x;
				remote_counter++;
				// if '}' is received AND remote_counter > 0
				if (x = '}')
				{
					speed_received = 1; // valid speed data!
				}
			}
			else // else
			{
				startchar_received = 0; // reset
				remote_counter = 0;
				speed_received = 0;
			}
		}
	}

	// Has message started?
	if (x == '{')
	{
		startchar_received = 1;
		remote_counter = 0;
		speed_received = 0;
	}
}