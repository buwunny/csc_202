//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Rafael Ortiz
//
//       LAB NAME:  Lab 7, Part 3
//
//      FILE NAME:  lab7p3_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code infinitely counts up from 0 to 99 and upon reaching 99 goes back
//    to 0. Simultaneously a count of how many DIP switches are in the on
//    position is displayed on DIG0 of the seven-segment display.
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
void run_lab7_part3(void);


//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define MSPM0_CLOCK_FREQUENCY                                             (40E6)
#define SYST_TICK_PERIOD                                              (10.25E-3)
#define SYST_TICK_PERIOD_COUNT        (SYST_TICK_PERIOD * MSPM0_CLOCK_FREQUENCY)

#define COUNTER_DELAY                                                      (200)


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
  leds_disable();
  seg7_init();
  sys_tick_init(SYST_TICK_PERIOD_COUNT);
  dipsw_init();
  I2C_init();
  lcd1602_init();
  lcd_clear();

  run_lab7_part3();
 
  // Endless loop to prevent program from ending
  while (1);

} /* main */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function represents the ISR (Interrupt Service Routine) for the SysTick
//  timer. It is called at regular intervals based on the configured SysTick
//  period. This ISR is responsible for monitoring the number of DIP switches in
//  in the ON position. Then, it displays that number (0-4) on the DIG0
//  seven-segment display.
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
  uint8_t dipsw_reading = dipsw_read();
  uint8_t count = 0;
  // check each switch independently to see if it is on
  for (int i = 0; i < 4; i++)
  {
    if ((dipsw_reading & (1 << i)) == (1 << i))
    {
      count++;
    } /* if */
  } /* for */
  seg7_hex(count, SEG7_DIG0_ENABLE_IDX);
} /* SysTick_Handler */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function infinitely counts from 0 to 99 and upon reaching 99 it resets.
//  There is a 0.2 second delay between each count. The count is displayed on
//  LCD line 1 (centered). All LEDs are disabled and unused seven-segment
//  displays are disabled.
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
void run_lab7_part3(void)
{
  leds_disable();
  seg7_off();

  bool done = false;
  uint8_t count = 0;
  while (!done)
  {
    lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_8);
    lcd_write_byte(count);
    count++;
    // reset counter if count reaches 0
    if (count >= 99)
    {
      count = 0;
    } /* if */
    msec_delay(COUNTER_DELAY);
  } /* while */

} /* rub_lab7_part3 */