//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Rafael Ortiz
//
//       LAB NAME:  Lab 7, Part 1 and Part 2
//
//      FILE NAME:  lab7_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code scrolls two messages across line 2 of the LCD. When scrolling
//    the first message, "SOS" is displayed to the seven-segment display. The
//    second message is longer so a new function is needed to display without
//    overflow.
//
//*****************************************************************************
//*****************************************************************************

//-----------------------------------------------------------------------------
// Loads standard C include files
//-----------------------------------------------------------------------------
#include <stdio.h>

//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include <ti/devices/msp/msp.h>
#include "clock.h"
#include "LaunchPad.h"
#include "lcd1602.h"


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void SysTick_Handler(void);
void wait_for_pb(int pb_idx);
void debounce_pb(int pb_idx);
void between_parts(char* string);
void run_lab7_part1(void);
void run_lab7_part2(void);
void lcd_write_string_window(char* string, uint8_t start_lcd_addr, uint8_t max_lcd_addr);


//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define MSPM0_CLOCK_FREQUENCY                                             (40E6)
#define SYST_TICK_PERIOD                                              (10.25E-3)
#define SYST_TICK_PERIOD_COUNT        (SYST_TICK_PERIOD * MSPM0_CLOCK_FREQUENCY)

#define NUM_STATES                                                          (18)

#define LCD_LINE2_END_ADDR               (LCD_LINE2_ADDR + LCD_CHAR_POSITION_16)
#define LCD_DELAY                                                          (100)
#define DEBOUNCE_DELAY                                                      (10)
#define ONE_SEC_DELAY                                                     (1000)


//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------
// Define the seven segment display for SOS
const uint8_t seg7_letter_code[] =
{
  0x6D, 0x00, 0x6D, 0x00, 0x6D, 0x00,   // S
  0x3F, 0x00, 0x3F, 0x00, 0x3F, 0x00,   // O
  0x6D, 0x00, 0x6D, 0x00, 0x6D, 0x00    // S
};

// These timings for Morse code dots and dashes can vary slightly based on
// different implementations, but the 1:3 ratio for dots:dashes is a typical
// ratio for Morse code. The delay times in terms of number SysTick interrupts.
const uint8_t delay_count[] = 
{
  0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x18,   // dots
  0x24, 0x24, 0x24, 0x24, 0x24, 0x24,   // dash
  0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x18,   // dots
};


int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
  leds_init();
  leds_disable();
  seg7_init();
  sys_tick_init(SYST_TICK_PERIOD_COUNT);
  dipsw_init();
  I2C_init();
  lcd1602_init();
  lcd_clear();

  run_lab7_part1();

  between_parts("Running Part 2");

  run_lab7_part2();

  // disable seven-segment after part 2
  sys_tick_disable();
  seg7_off();

  // Endless loop to prevent program from ending
  while (1);

} /* main */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function represents the ISR (Interrupt Service Routine) for the SysTick
//  timer. It is called at regular intervals based on the configured SysTick
//  period. This ISR is responsible for managing the timing and display of the
//  Morse code for "SOS" on the seven-segment display. It controls the blink
//  timing for the letters ,with quick blinks for "S" (dot-dot-dot) and slower
//  blinks for "O" (dash-dash-dash). The sequence of letters is repeated
//  indefinitely.
//
// INPUT PARAMETERS:
//  none
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//------------------------------------------------------------------------------
void SysTick_Handler(void)
{
  static uint16_t delay_time = 1;
  static uint16_t code_index = 0;

  delay_time--;
  if (delay_time == 0)
  {
    // delay time has expired so now move on to next letter to display
    seg7_on(seg7_letter_code[code_index], SEG7_DIG0_ENABLE_IDX);

    // get next delay time
    delay_time = delay_count[code_index];
    code_index++;

    if (code_index == NUM_STATES)
    {
      // we went through SOS message so restart index
      code_index = 0;
    } /* if */
  } /* if */
} /* SysTick_Handler */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function waits for the given pushbutton index to be pressed.
//
// INPUT PARAMETERS:
//  pb_idx - index of the pushbutton to wait for
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//------------------------------------------------------------------------------
void wait_for_pb(int pb_idx)
{
  while (is_pb_up(pb_idx));
} /* wait_for_pb */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function waits short delay, waits for given pushbutton to be released,
//  and then waits another short delay. This is to debounce the button and to
//  prevent unexpected behavior.
//
// INPUT PARAMETERS:
//  none
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//------------------------------------------------------------------------------
void debounce_pb(int pb_idx)
{
  msec_delay(DEBOUNCE_DELAY);
  while (is_pb_down(pb_idx));
  msec_delay(DEBOUNCE_DELAY);
} /* debounce_pb */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function displays "Press PB2" on line 1 then waits for PB2 to be
//  pressed then displays a message on line 1 of the LCD and waits 1 second.
//
// INPUT PARAMETERS:
//  string - The message to be displayed on line 1
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//------------------------------------------------------------------------------
void between_parts(char* string)
{
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("Press PB2");
  wait_for_pb(PB2_IDX);
  lcd_clear();
  debounce_pb(PB2_IDX);
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string(string);
  msec_delay(ONE_SEC_DELAY);
} /* between_parts */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function scrolls the message "Microcontrollers are fun." across the
//  second line of the LCD from right to left. The following guidelines were
//  used to create this function:
//  • The program clears the LCD at the program's start, and the LEDs are
//    disabled.
//  • The message must scroll across the LCD on line 2
//  • When the entire message disappears on the left, it should reappear on the
//    right side of the LCD
//  • The design must utilize one for and a while loop to shift the message one
//    letter at a time across the LCD
//  • The design must utilize the real-time interrupt for the SOS portion of the
//    design
//  • The SOS message should continue to flash while the message is being
//    displayed
//  • If pushbutton PB1 is pressed, a flag is set to terminate the loops. In
//    addition, the LCD is cleared, and the message “Part 1 Done” is displayed
//    on line 1 of the LCD.
//  • The program may not use any break statements.
//  • You may NOT use any library like string to determine the length of the
//    string
//
// INPUT PARAMETERS:
//  none
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//------------------------------------------------------------------------------
void run_lab7_part1(void)
{
  lcd_clear();
  leds_disable();

  bool done = false;
  char* message = "Microcontrollers are fun.";
  while (!done)
  {
    uint8_t offset = 0;
    uint8_t addr = LCD_LINE2_END_ADDR;
    lcd_clear();

    // scroll across second row from right to left until we reach the left side
    for (; addr >= LCD_LINE2_ADDR && !done; addr--)
    {
      done = is_pb_down(PB1_IDX);
      lcd_clear();
      lcd_set_ddram_addr(addr);
      lcd_write_string(message);
      msec_delay(LCD_DELAY);
    } /* for */
    
    // check if the character at the beginning of the adjusted string is a null
    // terminating character ('\n')
    while (*(message + offset)!= '\0' && !done)
    {
      offset++;
      done = is_pb_down(PB1_IDX);
      lcd_clear();
      lcd_set_ddram_addr(addr);
      lcd_write_string(message + offset);      
      msec_delay(LCD_DELAY);
    } /* while */
  } /* while */

  lcd_clear();
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Part 1 Done");
} /* rub_lab7_part1 */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function scrolls the message "Microcontrollers are fun. I love
//  programming in MSPM0+ assembly code!!!" across row 2. The custom function
//  lcd_write_string_window() is used instead of lcd_write_string(). Nearly the
//  same guidelines were followed for creating this function as part 1:
//  • The program clears the LCD at the program's start, and the LEDs are
//    disabled.
//  • The message must scroll across the LCD on line 2
//  • When the entire message disappears on the left, it should reappear on the
//    right side of the LCD
//  • The design must utilize one for and a while loop to shift the message one
//    letter at a time across the LCD
//  • The design must utilize the real-time interrupt for the SOS portion of the
//    design
//  • The SOS message should continue to flash while the message is being
//    displayed
//  • If pushbutton PB1 is pressed, a flag is set to terminate the loops. In
//    addition, the LCD is cleared, and the message “Part 2 Done” is displayed
//    on line 1 of the LCD.
//  • The program may not use any break statements.
//  • You may NOT use any library like string to determine the length of the
//    string  
//
// INPUT PARAMETERS:
//  none
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//------------------------------------------------------------------------------
void run_lab7_part2(void)
{ 
  lcd_clear();
  leds_disable();

  bool done = false;
  char* message = "Microcontrollers are fun. I love programming in MSPM0+ assembly code!!!";

  while (!done)
  {
    uint8_t offset = 0;
    uint8_t addr = LCD_LINE2_END_ADDR;
    lcd_clear();

    // scroll across second row from right to left until we reach the left side
    for (; addr >= LCD_LINE2_ADDR && !done; addr--)
    {
      done = is_pb_down(PB1_IDX);
      lcd_clear();
      lcd_set_ddram_addr(addr);
      lcd_write_string_window(message, LCD_LINE2_ADDR, LCD_LINE2_END_ADDR);
      msec_delay(LCD_DELAY);
      
    } /* for */
    
    // check if the character at the beginning of the adjusted string is a null
    // terminating character ('\n')
    while (*(message + offset) != '\0' && !done)
    {
      offset++;
      done = is_pb_down(PB1_IDX);
      lcd_clear();
      lcd_set_ddram_addr(addr);
      lcd_write_string_window(message + offset, LCD_LINE2_ADDR, LCD_LINE2_END_ADDR);
      msec_delay(LCD_DELAY);
    } /* while */
  } /* while */

  lcd_clear();
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Part 2 Done");
} /* run_lab7_part1 */


//------------------------------------------------------------------------------
// DESCRIPTION
//  This function writes a string to the DDRAM on LCD module. Each character of 
//  the string is written to the LCD one by one. This function only displays in 
//  the given window to ensure there is no overflow to other lines of the LCD.
//
// INPUT PARAMETERS:
//  string - message to be displayed in the given window on the LCD
//  start_lcd_addr - starting address in DDRAM of the window
//  max_lcd_addr - ending address in DDRAM of the window
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//------------------------------------------------------------------------------
void lcd_write_string_window(char* string, uint8_t start_lcd_addr, uint8_t max_lcd_addr)
{
  uint8_t length = max_lcd_addr - start_lcd_addr;
  uint8_t counter = 0;
  // for each spot in the window, write a char of the string to the LCD module
  while (counter <= length && *(string + counter) != '\0')
  {
    lcd_write_char(*(string + counter));
    counter++;
  } /* while */
} /* lcd_write_string */