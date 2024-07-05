/* 
NOTE:

TO-DO:
1. make finished variable 
2. make correct() function
3. make two_pressed() function
4. make function for while loop that returns timer count

*/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcd.h"


// Keeps track of what state the timer is in.
enum states { STALL, SHIFT, END};
// Variables that keep track of the time
volatile int tenths = 0;
volatile int ones = 0;
volatile int tens = 0;
// This variable ensures that the timer display is updated only every 0.1 seconds.
volatile int flag = 0;
// To check if the player should be finished
volatile uint8_t finished = 0;


int main() {
  // Initialize the LCD and TIMER1
  lcd_init();
  ISR(TIMER1_COMPA_vect);
  timer1_init();
  
  // Initialize any variables here:
  uint8_t pressed; // 0 = no button pressed, 1 = button pressed  
  uint8_t state = STALL; 

  // Enable Pull up resistors on D3 and D5 for buttons
  PORTC |= (1 << PC2|1<<PC4);

  // Set up otput for buzzer

  /* Need to initialize relevant DDRD and PORT registers. */

  // Show the splash screen
  lcd_writecommand(1);
  lcd_stringout("Welcome to Twinkle!");
  _delay_ms(2000);
  // Enable interrupts
  sei();

  // Create random blocks:
  // Array that randomizes 0s and 1s to display random blocks
  int blocks[42]; int i = 0;
  for(i = 0; i < 42; i++)
  {
      blocks[i] = (rand()%2);
  }

  lcd_writecommand(1);

  ones = 0;
  tens = 0;
  tenths = 0;
  int start = 0;
  int array_count = 0;
  
  while(1){
    // Check for state
    // if (finished())
    // {
    //   state = END;
    // }
    
    if(state == STALL)
    {
      if (correct(blocks[0])){
        lcd_moveto(0,0);
        lcd_stringout("a");
        state = SHIFT;
      }
      else{
        // Display the blocks.
        // array_count can increment until 42
        // j can increment until 16
        int j = 0;
        for (array_count = start; j < start+16; j++)
        {
          if (array_count < 42)
          {
            lcd_moveto(blocks[array_count], j);
            lcd_stringout(".");
          }
          else
          {
            lcd_moveto(0, j);
            lcd_stringout(" ");
          }
          array_count++;
        }
      }
    }
    else if (state == SHIFT)
    {
      // Move the blocks down.
      start++;
      // Play the note.

      state == STALL;
    }
    else if (state == END)
    {
      
    }
      
  }
  return 0;
}
// **************************** MAIN END *******************************

int correct(int position)
{
    // _delay_ms(3000);
    // return 1;
    char x = PINC;
    if (position == 0)
    {
        // left button should be pressed
        // PC2
        if((x & (1<<PC2)|(1<<PC4)) == (1<<PC4))
        {
            return 1;
        }
        return 0;
    }
    else
    {
        // right button should be pressed
        // PC4
        if((x & (1<<PC2)|(1<<PC4)) == (1<<PC2))
        {
            return 1;
        }
        return 0;
    }

}

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
        finished = 1;
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
