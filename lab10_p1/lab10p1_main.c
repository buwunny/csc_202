//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Rafael Ortiz
//
//       LAB NAME:  Lab 10, Part 1
//
//      FILE NAME:  lab10p1_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code communicates with the microcontroller over UART, emulating a
//    terminal. Whatever is typed is displayed on line one of the LCD.
//    Backspaces are handled and output is truncated to fit on one line.
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


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab10_part1(void);


//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define BUFFER_LENGTH                                                       (16)
#define CARRIAGE_RETURN_CHAR                                              ('\r')
#define NULL_CHAR                                                         ('\0')
#define BACKSPACE_CHAR                                                    ('\b')


//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


// Define a structure to hold different data types

int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
 
  I2C_init();
  lcd1602_init();
  UART_init(115200);
 
  run_lab10_part1();
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("Program Stopped.");

  // Endless loop to prevent program from ending
  while (1);

} /* main */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function communicates with the MSPM0+ over UART, handling backspaces
//  and long messages. The following guidelines were used to create this
//  function.
//   • All LEDs and seven-segment displays are disabled
//   • Immediately echoed back characters to the terminal emulator
//   • Your program should detect the backspace key, allowing users to correct 
//     their input.
//   • The LCD should only display characters after the ENTER key is pressed.
//   • The first line of the LCD should display the status: “Name: ” followed by
//     the characters entered.
//   • The second line of the LCD should display the message “Program Stopped”.
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
void run_lab10_part1(void)
{
  char buffer[BUFFER_LENGTH];
  uint8_t index = 0;
  char key;
  do
  {
    key = UART_in_char();
    UART_out_char(key);
    if (key == CARRIAGE_RETURN_CHAR)
    {
      buffer[index] = NULL;
    } /* if */
    else if (key == BACKSPACE_CHAR) {
      if (index > 0)
      {
        index--;
      } /* if */
    } /* else if */
    else {
      if (index < BUFFER_LENGTH)
      {
        buffer[index] = key;
        index++;
      }
    } /* else */
  } while (key != CARRIAGE_RETURN_CHAR);
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Name:");
  lcd_write_string(buffer);
} /* run_lab10_part1 */