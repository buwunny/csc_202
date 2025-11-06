//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Rafael Ortiz
//
//       LAB NAME:  Lab 10, Part 2
//
//      FILE NAME:  lab10p2_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code displays a menu on the serial console and allows the user to
//    select an option. Incorrect options are handled and each selection calls a
//    separate function. Upon finishing a message is displayed both on the
//    serial console and on the LCD.
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
#include "uart.h"
#include "lcd1602.h"
#include "adc.h"


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab10_part2(void);
void display_menu(void);
void inc_seg7(bool should_inc);
void show_temp(void);
void flash_leds(void);
void end_program(void);
void UART_write_string(char* string);


//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define BUFFER_LENGTH                                                        (1)
#define NEW_LINE_CHAR                                                     ('\n')
#define NULL_CHAR                                                         ('\0')
#define ZERO_CHAR                                                          ('0')
#define ONE_CHAR                                                           ('1')
#define TWO_CHAR                                                           ('2')
#define THREE_CHAR                                                         ('3')
#define FOUR_CHAR                                                          ('4')
#define DEGREE_SYMBOL                                               (0b11011111)
#define TEMP_SENSOR_CHANNEL                                                  (5)
#define CONVERT_TO_FAHRENHEIT(x)                              ((x) * 9 / 5 + 32)
#define ALL_LEDS_ON_VAL                                                    (255)
#define LEDS_BLINK_AMOUNT                                                    (3)
#define LEDS_DELAY                                                         (250)


//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
 
  leds_init();
  seg7_init();
  I2C_init();
  lcd1602_init();
  UART_init(115200);
  ADC0_init(ADC12_MEMCTL_VRSEL_VDDA_VSSA);

  leds_off();
  lcd_clear();

  run_lab10_part2();

  lcd_clear();
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Program Stopped.");
 
  // Endless loop to prevent program from ending
  while (1);

} /* main */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function handles user input and calls the corresponding functions for 
//  each selection. The correct components are enabled and disabled as needed
//  for correct behaviour. The following guidelines were used to create this
//  function.
//  • Ensure the LCD is cleared and all LEDs are off when the program starts.
//  • All seven-segment displays are disabled except DIG0, which displays 0 when
//    the program starts.
//  • The seven-segment display should not flash when the LEDs are flashing.
//  • The design should read the temperature sensor when selecting the
//    temperature option. The temperature value should remain on the LCD until
//    the option is selected again.
//  • When the program has terminated, “Program Stopped” is displayed on line 1
//    of the LCD.
//  • The design must implement the actions for each menu option using specific
//    functions: flash_leds, inc_seg7, and display_temp.
//  • You must create your version of a uart_write_string function that sends a
//    string to the UART.
//  • You must create a display_menu function to display the menu to the UART.
//  • No global variables are allowed. Each function should maintain the
//    variables it needs to operate. For instance, define the seven-segment
//    count variable as static within its function.
//  • When the program ends, the LEDs and seven-segment display should be
//    disabled.
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
void run_lab10_part2(void)
{
  char user_input = ZERO_CHAR; // default value
  inc_seg7(false);
  do
  {
    display_menu();
    user_input = UART_in_char();
    UART_out_char(user_input);
    UART_out_char(NEW_LINE_CHAR);
    switch (user_input) {
      case ONE_CHAR:
        UART_write_string("You chose to count on 7-segment!\n");
        leds_disable(); // ensure LEDs are disabled before running 7-segment
        inc_seg7(true);
        break;
      case TWO_CHAR:
        UART_write_string("You chose to show current temperature!\n");
        show_temp();
        break;
      case THREE_CHAR:
        UART_write_string("You chose to flash LEDs 3 times!\n");
        seg7_off();
        leds_enable();
        flash_leds();
        leds_disable();
        inc_seg7(false); // bring back the number on 7-segment
        break;
      case FOUR_CHAR:
        UART_write_string("You chose to end the program!\n");
        end_program();
        break;
      default:
        UART_write_string("Please enter a valid option!\n");
        break;
    }
  } while (user_input != FOUR_CHAR);
  seg7_off();
  leds_disable();
} /* run_lab10_part2 */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function displays the menu in the serial console over UART. The options
//  include incrementing the 7-segment display, showing the current temperature,
//  and ending the program.
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
void display_menu(void)
{
  UART_write_string("\n");
  UART_write_string("MENU OPTIONS\n");
  UART_write_string("   1. Increment count on 7-segment display\n");
  UART_write_string("   2. Show current temperature\n");
  UART_write_string("   3. Flash LEDs 3 times\n");
  UART_write_string("   4. End Program\n");
  UART_write_string("Enter your selection: ");
} /* display_menu */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function increments the 7-segment display if the passed in parameter is
//  true. Upon reaching 9, the value rolls back to zero. The current value is
//  stored in a static variable.
//
// INPUT PARAMETERS:
//  should_inc - if the 7-segment should increment
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//------------------------------------------------------------------------------
void inc_seg7(bool should_inc)
{
  static uint8_t current = 0;
  if (should_inc)
  {
    current++;
    if (current > 9) // rollover
    {
      current = 0;
    } /* if */
  }
  seg7_hex(current, SEG7_DIG0_ENABLE_IDX);
} /* count_seg7 */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function displays the current temperature reading from the thermistor
//  on the LCD after with "Temp = " before and a degree and "F" after.
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
void show_temp(void)
{
  uint16_t adc_temp_result = ADC0_in(TEMP_SENSOR_CHANNEL);
  uint8_t temperature_c = thermistor_calc_temperature(adc_temp_result);
  uint8_t temperature_f = CONVERT_TO_FAHRENHEIT(temperature_c);

  lcd_clear();
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Temp = ");
  lcd_write_byte(temperature_f);
  lcd_write_char(DEGREE_SYMBOL);
  lcd_write_char('F');

} /* show_temp */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function flashes all the bar LEDs 3 times. All on, delay, all off,
//  delay, repeat. 
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
void flash_leds(void)
{
  leds_off();
  msec_delay(LEDS_DELAY);
  for (int i = 0; i < LEDS_BLINK_AMOUNT; i++)
  {
    leds_on(ALL_LEDS_ON_VAL);
    msec_delay(LEDS_DELAY);
    leds_off();
    msec_delay(LEDS_DELAY);
  } /* for */
  leds_off();
} /* flash_leds */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function simply displays "Thank you for using the program." over the
//  UART.
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
void end_program(void)
{
  UART_write_string("Thank you for using the program.\n");
} /* end_program */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function sends and displays a string over the UART and console. It goes
//  through the string until it reaches the end (null character) and then stops. 
//
// INPUT PARAMETERS:
//  string - message to be sent and displayed over the UART and console.
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//------------------------------------------------------------------------------
void UART_write_string(char* string)
{
  uint8_t index = 0;
  char current_char = *(string + index);
  while (current_char != NULL_CHAR)
  {
    current_char = *(string + index++);
    UART_out_char(current_char); 
  } /* while */
} /* UART_write_string */