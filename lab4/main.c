//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Rafael Ortiz
//
//       LAB NAME:  Lab 4
//
//      FILE NAME:  main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code displays various outputs to the bar LEDs and 7-segment displays
//    to demonstrate a control over the hardware.
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


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab4_part1();
void run_lab4_part2();
void run_lab4_part3();
void run_lab4_part4();
void run_lab4_part5();
void run_lab4_part6();

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define SEG7_SEG_A_MASK                                    (1 << SEG7_SEG_A_IDX)
#define SEG7_SEG_B_MASK                                    (1 << SEG7_SEG_B_IDX)
#define SEG7_SEG_C_MASK                                    (1 << SEG7_SEG_C_IDX)
#define SEG7_SEG_D_MASK                                    (1 << SEG7_SEG_D_IDX)
#define SEG7_SEG_E_MASK                                    (1 << SEG7_SEG_E_IDX)
#define SEG7_SEG_F_MASK                                    (1 << SEG7_SEG_F_IDX)
#define SEG7_SEG_G_MASK                                    (1 << SEG7_SEG_G_IDX)
#define SEG7_SEG_H_MASK                                    (1 << SEG7_SEG_H_IDX)

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
 
  // lab 4 code
  run_lab4_part1();

  msec_delay(1000);

  run_lab4_part2();

  msec_delay(500);

  run_lab4_part3();

  msec_delay(500);

  run_lab4_part4();

  msec_delay(500);

  run_lab4_part5();

  msec_delay(500);

  run_lab4_part6();

  // Endless loop to prevent program from ending
  while (1);

} /* main */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function initializes the expansion board LEDs, enables them, and turns
//  on LEDs 2-5.
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
void run_lab4_part1()
{
  // initialize LEDs
  leds_init();

  // enable LEDs
  leds_enable();

  // turn on LEDs 2-5
  led_on(LED_BAR_LD2_IDX);
  led_on(LED_BAR_LD3_IDX);
  led_on(LED_BAR_LD4_IDX);
  led_on(LED_BAR_LD5_IDX);
} /* run_lab4_part1 */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function sequences through leds 0-7 then 6-0 lighting up one LED at a
//  time, waits 0.2 seconds, then turns the LED off before moving to the next
//  one. This sequencing repeats 5 times.
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
void run_lab4_part2()
{
  // define list of bar LED indicies
  uint8_t led_indicies[] = {
    LED_BAR_LD0_IDX, LED_BAR_LD1_IDX, LED_BAR_LD2_IDX, LED_BAR_LD3_IDX,
    LED_BAR_LD4_IDX, LED_BAR_LD5_IDX, LED_BAR_LD6_IDX, LED_BAR_LD7_IDX
  };

  // ensure LEDs are off before starting
  leds_off();
  
  uint8_t counter = 0;
  while (counter < 5)
  {
    // sequence left
    for (uint8_t j = 0; j < 8; j++)
    {
      led_on(led_indicies[j]);
      msec_delay(200);
      led_off(led_indicies[j]);
    } /* for */

    // sequence right
    for (uint8_t j = 6; j > 0; j--)
    {
      led_on(led_indicies[j]);
      msec_delay(200);
      led_off(led_indicies[j]);
    } /* for */


    counter++;
  } /* while */

  led_on(LED_BAR_LD0_IDX);
  msec_delay(200);
  // ensure LEDs are off after function
  leds_off();
} /* run_lab4_part2 */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function displays a binary counter with the LEDs going from 0-255 and
//  displays each value for 0.1 seconds. Once 255 is reached it holds the value
//  for 0.5 seconds before clearing all the LEDs.
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
void run_lab4_part3()
{
  // ensure LEDs are off
  leds_off();
  // run twice
  uint8_t run_counter = 0;
  while (run_counter < 2)
  {
    uint16_t counter = 0;
    while (counter <= 255)
    {
      leds_on(counter++);
      msec_delay(100);
    } /* while */
    msec_delay(400);
    run_counter++;
  } /* while */
  
  // ensure LEDs are off after this function
  leds_off();

  // disable LEDs because we are done using them
  leds_disable();
} /* run_lab4_part3 */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function displays the letter "L" on 7-segment DIG1 (furthest left).
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
void run_lab4_part4()
{
  // ensure 7-segment is initialized
  seg7_init();
  
  // make the letter "L"
  seg7_on(
    SEG7_SEG_D_MASK | SEG7_SEG_E_MASK | SEG7_SEG_F_MASK, 
    SEG7_DIG0_ENABLE_IDX
  );

} /* run_lab4_part4 */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function displays the number "4" on DIG2 for 3 seconds then nothing for
//  2 seconds. Repeats 4 times.
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
void run_lab4_part5()
{
  // ensure 7-segment is initialized and off
  seg7_init();
  seg7_off();

  uint8_t loop_counter = 0;
  while (loop_counter < 4)
  {
    // make the number "4" for 3 seconds
    seg7_on(
      SEG7_SEG_B_MASK | SEG7_SEG_C_MASK | SEG7_SEG_F_MASK | SEG7_SEG_G_MASK,
      SEG7_DIG2_ENABLE_IDX
    );
    msec_delay(3000);

    // off for 2 seconds
    seg7_off();
    msec_delay(2000);

    loop_counter++;
  } /* while */
} /* run_lab4_part5 */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function displays the word "CAFE" on DIG0-DIG3.
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
void run_lab4_part6()
{
  // ensure 7-segment is initialized and off
  seg7_init();
  seg7_off();

  uint8_t loop_counter = 0;
  while (loop_counter < 200)
  {
    // letter "C" on DIG0
    seg7_on(
      SEG7_SEG_A_MASK | SEG7_SEG_D_MASK | SEG7_SEG_E_MASK | SEG7_SEG_F_MASK,
      SEG7_DIG0_ENABLE_IDX
    );
    msec_delay(2);

    // letter "A" on DIG1
    seg7_on(
      SEG7_SEG_A_MASK | SEG7_SEG_B_MASK | SEG7_SEG_C_MASK | SEG7_SEG_E_MASK |
      SEG7_SEG_F_MASK | SEG7_SEG_G_MASK,
      SEG7_DIG1_ENABLE_IDX
    );
    msec_delay(2);

    // letter "F" on DIG2
    seg7_on(
      SEG7_SEG_A_MASK | SEG7_SEG_E_MASK | SEG7_SEG_F_MASK | SEG7_SEG_G_MASK,
      SEG7_DIG2_ENABLE_IDX
    );
    msec_delay(2);

    // letter "E" on DIG3
    seg7_on(
      SEG7_SEG_A_MASK | SEG7_SEG_D_MASK | SEG7_SEG_E_MASK | SEG7_SEG_F_MASK |
      SEG7_SEG_G_MASK,
      SEG7_DIG3_ENABLE_IDX
    );
    msec_delay(2);

    loop_counter++;
  } /* while */
  seg7_off();
} /* run_lab4_part6 */
