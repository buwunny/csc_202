//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Rafael Ortiz
//
//       LAB NAME:  Lab 6
//
//      FILE NAME:  main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code interfaces with the Liquid Crystal Display (LCD), displaying
//    various letters and numbers. Display is controlled initially by the push
//    buttons, displaying the alphabet, different numbers, and a counter. Then
//    the display is controlled by the keypad, displaying the keys that are
//    pressed.
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
void disable_leds_and_7seg(void);
void wait_for_pb(int pb_idx);
void debounce_pb(int pb_idx);
void debounce_keypad(void);
void between_parts(char* string);
void run_lab6_part1(void);
void run_lab6_part2(void);
void run_lab6_part3(void);
void run_lab6_part4(void);


//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define DEBOUNCE_DELAY                                                      (10)
#define DEFAULT_KEY                                                       (0x10)


//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
  
  // initialize everything
  leds_init();
  seg7_init();
  dipsw_init();
  keypad_init();
  I2C_init();
  lcd1602_init();

  run_lab6_part1();

  between_parts("Running Part 2");

  run_lab6_part2();

  between_parts("Running Part 3");

  run_lab6_part3();

  between_parts("Running Part 4");

  run_lab6_part4();

  // Endless loop to prevent program from ending
  while (1);

} /* main */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function ensures the LEDs and 7-segment displays are disabled and off.
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
void disable_leds_and_7seg(void)
{
  leds_off();
  leds_disable();
  seg7_off();
} /* disable_leds_and_7seg */


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
//  This function waits short delay, waits for keypad button to be released, and
//  then waits another short delay. This is to debounce the keypad and to
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
void debounce_keypad(void)
{
  msec_delay(DEBOUNCE_DELAY);
  wait_no_key_pressed();
  msec_delay(DEBOUNCE_DELAY);
} /* debounce_keypad */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function waits for PB2 to be pressed then displays a message on line 1
//  of the LCD and waits 1 second.
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
  wait_for_pb(PB2_IDX);
  lcd_clear();
  debounce_pb(PB2_IDX);
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string(string);
  msec_delay(1000);
} /* between_parts */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function displays the alphabet on the LCD. Letters A-P are on the first
//  line and letters Q-Z are on the second line. The following guidelines were
//  used to create this function:
//    • A C for loop must be used.
//    • Letters A through P are displayed on the first line of the LCD
//    • Letters Q through Z are displayed on the second line of the LCD
//      (left-justified)
//    • All LEDs and all seven segment displays are disabled
//    • The design must utilize one for loop to display one letter at a time to
//      the LCD. You may not use the subroutine lcd_write_string.
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
void run_lab6_part1(void)
{
  lcd_clear();
  disable_leds_and_7seg();

  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  for (char letter = 'A'; letter < 'Z'; letter++)
  {
    lcd_write_char(letter);
    if (letter == 'P')
    {
      lcd_set_ddram_addr(LCD_LINE2_ADDR);
    } /* if */
  } /* for */
} /* run_lab6_part1 */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function displays a 32-bit integer, a 16-bit integer and an 8-bit
//  integer to the display. PB1 must be pressed to display the next number.
//  The following guidelines were used to create this function:
//    • Clear the LCD: Ensure the LCD is clear before displaying any numbers.
//    • Display the 32-bit value 1234567890: Center this integer on the first
//      row of the LCD.
//    • Display the 16-bit value 1234: Center this integer on the second row of
//      the LCD.
//    • Display the 8-bit 123: Clear the LCD and center this integer on the
//      first row of the LCD.
//    • User interaction: After the 32-bit value is displayed, wait for the user
//      to press PB1 before displaying the 16-bit value. If the user presses PB1
//      again, clear the LCD and display the 8-bit value. If PB1 is pressed a
//      third time, clear the LCD and display the string “Part 2 Done.”.
//    • Disable all LEDs: Ensure that all LEDs on the board are turned off.
//    • Disable all seven-segment displays: Ensure that all segments of the
//      seven-segment displays are turned off.
//    • Display integers as a single operation: The integers should be displayed
//      on the LCD as a single operation, not as strings or individual characters
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
void run_lab6_part2(void)
{
  lcd_clear();
  disable_leds_and_7seg();

  lcd_set_ddram_addr(LCD_LINE1_ADDR);

  lcd_clear();

  uint32_t first_value = 1234567890;
  uint16_t second_value = 1234;
  uint8_t third_value = 123;

  // display 1234567890 centered on line 1
  lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_4);
  lcd_write_quadbyte(first_value);

  // display 1234 centered on line 2 after PB1 is pressed
  wait_for_pb(PB1_IDX);
  lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_6);
  lcd_write_doublebyte(second_value);
  debounce_pb(PB1_IDX);
  
  // clear display and display 123 centered on line 1 after PB1 is pressed
  wait_for_pb(PB1_IDX);
  lcd_clear();
  debounce_pb(PB1_IDX);
  lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_8);
  lcd_write_byte(third_value);
  
  wait_for_pb(PB1_IDX);
  lcd_clear();
  debounce_pb(PB1_IDX);
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Part 2 Done.");
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("PRESS PB2...");
} /* run_lab6_part2 */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function displays a countdown from 100-0 on line 1 of the LCD and
//  repeats until PB2 is pressed. PB1 also resets the countdown to 100. The
//  following guidelines were used to create this function:
//    • The timer count value starts at 100 and is decremented every 0.2 seconds
//    • When the timer count value reaches 0, the timer automatically restarts
//      at 100
//    • The LCD should be cleared before the timer count value is displayed.
//    • The timer count value is displayed in the center of the first row
//    • If pushbutton PB1 is pressed, the timer automatically restarts at 100
//    • If pushbutton PB2 is pressed, the timer automatically stops, and the
//      message “Part 3 Done” is displayed on line 1 of the LCD.
//    • All LEDs and seven-segment displays are disabled
//    • The design must use a while loop and a done flag to determine if the
//      loop should be terminated.
//    • The design displays integers as a single operation (as an integer) to
//      the LCD
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
void run_lab6_part3(void)
{
  lcd_clear();
  disable_leds_and_7seg();

  uint8_t count = 100;
  bool done = false;

  while (!done)
  {
    lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_8);
    lcd_write_byte(count);
    count--;
    // reset counter if count reaches 0 or if PB1 is pressed
    if (count <= 0 || is_pb_down(PB1_IDX))
    {
      count = 100;
    } /* if */
    if (is_pb_down(PB2_IDX))
    {
      lcd_clear();
      done = true;
    } /* if */
    msec_delay(200);
  } /* while */

  debounce_pb(PB2_IDX);

  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Part 3 Done.");
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("PRESS PB2...");
} /* run_lab6_part3 */



//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function displays what is pressed on the keypad to the LCD. After both
//  lines of the LCD are filled, the LCD clears and fills again. PB2 ends this
//  function and PB1 clears the screen and resets to the first position in the
//  first line. The following guidelines were used to create this function:
//    • All LEDs and seven-segment displays are disabled
//    • The LCD starts cleared
//    • The first digit pressed on the keypad is displayed in the first position
//      of the first row
//    • Additional digits are displayed on the LCD as pressed on the keypad
//    • Once the first row is filled, the digits are displayed on the second row
//    • When the second line is filled, and another key is pressed, the LCD is
//      cleared, and the next digit is displayed in the first position of the
//      first row
//    • If pushbutton PB1 is pressed, the LCD is cleared, and the next digit is
//      displayed in the first position of the first row
//    • If pushbutton PB2 is pressed, the loop is exited, and the message 
//      “Program Stopped” is displayed on line 1 of the LCD.
//    • The design must use a flag (i.e., Bool done) to exit the main counter
//      loop
//    • The design displays keypad values as a single operation (as a
//      hexadecimal number) to the LCD
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
void run_lab6_part4(void)
{
  lcd_clear();
  disable_leds_and_7seg();

  bool done = false;
  uint8_t char_count = 0;
  uint8_t key = DEFAULT_KEY;
  
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  while (!done)
  {
    // reset screen and move cursor to beginning when PB1 is pressed
    if (is_pb_down(PB1_IDX))
    {
      lcd_clear();
      lcd_set_ddram_addr(LCD_LINE1_ADDR);
      char_count = 0;
    } /* if */
    
    if (is_pb_down(PB2_IDX))
    {
      lcd_clear();
      done = true;
    } /* if */
    
    // move to next line
    if (char_count == CHARACTERS_PER_LCD_LINE)
    {
      lcd_set_ddram_addr(LCD_LINE2_ADDR);
    } /* if */

    // loop around
    if (char_count > TOTAL_CHARACTERS_PER_LCD)
    {
      lcd_clear();
      lcd_set_ddram_addr(LCD_LINE1_ADDR);
      char_count = 0;
    } /* if */
    // if not looping scan for key
    // (this lets the 33rd key press be displayed in line 1, position 1)
    else 
    {
      key = keypad_scan();
    } /* else */
    
    if (key != DEFAULT_KEY) 
    {
      debounce_keypad();
      
      char char_to_display = hex_to_ascii(key);
      // '*' and '#' display 'E' and 'F' so we change them to be correct char
      if (char_to_display == 'E')
      {
        char_to_display = '*';
      } /* if */
      if (char_to_display == 'F')
      {
        char_to_display = '#';
      } /* if */
      
      lcd_write_char(char_to_display);
      char_count++;
    } /* if */
  } /* while */

  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Part 4 Done.");
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("PROGRAM STOPPED.");  
} /* run_lab6_part4 */