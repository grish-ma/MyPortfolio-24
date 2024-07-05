/* 
NOTE:

TO-DO:
1. make finished() funciton
2. make correct() function

*/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcd.h"


// Keeps track of what state the timer is in.
enum states { CORRECT, INCORRECT, INIT, END};
// Variables that keep track of the time
volatile int tenths = 0;
volatile int ones = 0;
volatile int tens = 0;
// This variable ensures that the timer display is updated only every 0.1 seconds.
volatile int flag = 0;

// void rand_init(void);

int main() {
  // Initialize the LCD and TIMER1
  lcd_init();
  ISR(TIMER1_COMPA_vect);
  timer1_init();
  // Initialize the random number function
  // rand_init();
  
  // Initialize any variables here:
  uint8_t pressed; // 0 = no button pressed, 1 = button pressed  
  uint8_t state = INIT; 

  // Check for state
//   if (finished())
//   else if (state == INIT && correct()){
//     state == CORRECT;
//   }
//   else if (state == INIT && !correct()){
//     state == INCORRECT;
//   }
//   else if (state == CORRECT && correct()){
//     state == CORRECT;
//   }
//   else if (state == CORRECT && !correct()){
//     state = INCORRECT;
//   }
//   else if (state == INCORRECT && correct()){
//     state = CORRECT;
//   }
//   else if (state == INCORRECT && !correct()){
//     state = INCORRECT;
//   }

  // Enable Pull up resistors on D3 and D5 for buttons
  PORTD |= (1 << PD3|1<<PD5);

  // Set up otput for buzzer

  /* Need to initialize relevant DDRD and PORT registers. */

  // Show the splash screen
  lcd_writecommand(1);
  lcd_stringout("Welcome to Twinkle!");
  _delay_ms(2000);
  // Enable interrupts
  sei();

  ones = 0;
  tens = 0;
  tenths = 0;
  while(1){
    // Check for button press 
    // if (((PIND & (1 << PD3 || 1 << PD5)) == 0) && ){
      
    // }

    //
   
    // If necessary write time to LCD. Only update the time on the LCD 
    // every 0.1 seconds (flag variable)
    // if (flag == 1)
    // {
    //     flag = 0;
    //     lcd_writecommand(1);
    //     if (tens == 0)
    //         lcd_writedata(' ');
    //     else
    //         lcd_writedata(tens + '0');
    //     lcd_writedata(ones + '0');
    //     lcd_writedata('.');
    //     lcd_writedata(tenths + '0');
    // }
      
  }
  return 0;
}
// **************************** MAIN END *******************************

// **************************** TIMER1_INIT START *******************************
void timer1_init(void)
{
    // Add code to configure TIMER1 by setting or clearing bits in
    // the TIMER1 registers.

    // Set to CTC mode. This tells the hardware to start over at 0 
    // once the counter is reaches your desired values.
    TCCR1B |= (1 << WGM12);

    // Enable Timer Interrupt - "Output Compare Match Interrupt"
    TIMSK1 |= (1 << OCIE1A);

    // Load the MAX count, counting to 1,600,000 because
    // 0.1s delay with 16 MHz clock.
    // Prescalar: 1,600,000 / 256 = 6,250
    OCR1A = 6250; // modulus value

    // Set prescalar = 256 and start counter
    TCCR1B |= (1 << CS12);

}
// **************************** TIMER1_INIT END *******************************

// **************************** ISR START *******************************
// This method is called every 0.1 seconds, based on the timer 
// intialization in the time_init() function.
ISR(TIMER1_COMPA_vect)
{
    flag = 1;
    // Increment the time.
    // once the tenths and/or ones variables become 9, 
    // reset to 0 and increment the next biggest value.
    tenths++;
    if (tenths > 9)
    {
        tenths = 0;
        ones++;
    }

    if (ones > 9)
    {
        ones = 0;
        tens++;
    }

    if (tens > 5)
    {
        tenths = 0;
        ones = 0; 
        tens = 0;
    }
}
// **************************** ISR END *******************************

// *************************** DEBOUNCE START ******************************
void debounce(uint8_t bit)
{
  // Add code to debounce input "bit" of PINC
  // assuming we have sensed the start of a press.
  if( (PINC & (1<<bit)) == 0 ) // first pressed
  {
      _delay_ms(5);
      while( (PINC & (1<<bit)) == 0 ) { }
      _delay_ms(5);
  }
}
// *************************** DEBOUNCE END ******************************


// void rand_init()
// {
//     int16_t seed = 0;
//     uint8_t i, j, x;

//     // Build a 15-bit number from the LSBs of an ADC
//     // conversion of the channels 1-5, 3 times each
//     for (i = 0; i < 3; i++) {
//   for (j = 1; j < 6; j++) {
//       x = adc_sample(j);
//       x &= 1;	// Get the LSB of the result
//       seed = (seed << 1) + x; // Build up the 15-bit result
//   }
//     }
//     srand(seed);	// Seed the rand function
// }