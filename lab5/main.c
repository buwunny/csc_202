//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Rafael Ortiz
//
//       LAB NAME:  Lab 5
//
//      FILE NAME:  main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code interfaces with the push buttons on the expansion board, keypad,
//    and 7-segment display.
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
void debounce_pb1(void);
void debounce_lpsw2(void);
void debounce_keypad(void);
void run_lab5_part1(void);
void run_lab5_part2(void);
void run_lab5_part3(void);
void run_lab5_part4(void);

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define DEBOUNCE_DELAY                                                      (10)
#define PART_DELAY                                                         (500)

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


// Define an enum for the state machine
typedef enum
{
  GET_LOW,
  GET_HIGH,
  DISPLAY
} fsm_state_t;

typedef unsigned short uint16;


int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();

  // initialize everything  
  leds_init();
  leds_disable();
  seg7_init();
  dipsw_init();
  lpsw_init();
  keypad_init();

  run_lab5_part1();

  msec_delay(PART_DELAY);

  run_lab5_part2();

  msec_delay(PART_DELAY);

  run_lab5_part3();

  msec_delay(PART_DELAY);

  run_lab5_part4();


  // Endless loop to prevent program from ending
  while (1);

} /* main */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function waits short delay, waits for push button 1 to be released, and
//  then waits another short delay. This is to debounce the button and to
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
void debounce_pb1(void)
{
  msec_delay(DEBOUNCE_DELAY);
  while (is_pb_down(PB1_IDX));
  msec_delay(DEBOUNCE_DELAY);
} /* debounce_pb1 */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function waits short delay, waits for LaunchPad switch 1 to be
//  released, and then waits another short delay. This is to debounce the
//  switch and to prevent unexpected behavior.
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
void debounce_lpsw2(void)
{
  msec_delay(DEBOUNCE_DELAY);
  while (is_lpsw_down(LP_SW2_IDX));
  msec_delay(DEBOUNCE_DELAY);
} /* debounce_lpsw2 */


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
//  This function loops until the push button has been pressed 6 times. This
//  allows the "3" to be displayed and hidden on DIG0 three times.
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
void run_lab5_part1(void)
{
  uint8_t loop_counter = 0;
  bool is_on = false;
  while (loop_counter < 6)
  {
    if (is_pb_down(PB1_IDX))
    {
      loop_counter++;
      // if on turn off
      if (is_on)
      {
        seg7_off();
        is_on = false;
      } /* if */
      // if off turn on
      else
      {
        seg7_on(
          SEG7_SEG_A_MASK | SEG7_SEG_B_MASK | SEG7_SEG_C_MASK |
          SEG7_SEG_D_MASK | SEG7_SEG_G_MASK,
          SEG7_DIG0_ENABLE_IDX
        );
        is_on = true;
      } /* else */
      debounce_pb1();
    } /* if */
  } /* while */
} /* run_lab5_part1 */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function reads the the lower four bits then the upper four bits then
//  combines them together and displays them on the 7-segment display. This
//  repeats 3 times.
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
void run_lab5_part2(void)
{
  fsm_state_t current_state = GET_LOW;
  uint8_t loop_counter = 0;
  uint8_t low_nibble = 0;
  uint8_t high_nibble = 0;
  uint8_t full_byte = 0;
  
  while (loop_counter < 3)
  {
    // state machine
    switch (current_state)
    {
      case GET_LOW:
        // read lower half of data on lpsw1 press
        if (is_lpsw_down(LP_SW2_IDX))
        {
          low_nibble = dipsw_read();
          current_state = GET_HIGH;
          debounce_lpsw2();
        } /* if */
        break;

      case GET_HIGH:
        // read upper half of data on lpsw1 press
        if (is_lpsw_down(LP_SW2_IDX))
        {
          high_nibble = dipsw_read() << 4;
          current_state = DISPLAY;
          
          // // combine data
          full_byte = high_nibble | low_nibble;
          
          debounce_lpsw2();
        } /* if */
        break;

      case DISPLAY:
        // display on either DIG2 or DIG0
        if (is_pb_down(PB1_IDX))
        {
          seg7_on(full_byte, SEG7_DIG2_ENABLE_IDX);
          debounce_pb1();
        } /* if */  
        else
        {
          seg7_on(full_byte, SEG7_DIG0_ENABLE_IDX);
        } /* else */

        // start over
        if (is_lpsw_down(LP_SW2_IDX)) {
          loop_counter++;
          seg7_off();
          current_state = GET_LOW;
          debounce_lpsw2();
        } /* if */
        break;
    } /* switch */
  }/* while */
  seg7_off();
} /* run_lab5_part2 */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function displays the value of the keypad button pressed in binary on
//  the bar LEDs. This repeats 8 times.
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
void run_lab5_part3(void)
{
  leds_enable();
  leds_off();

  uint8_t loop_counter = 0;
  
  while (loop_counter < 8)
  {
    // display the number from the keypad in binary with LEDs
    leds_on(getkey_pressed());
    loop_counter++;
    debounce_keypad();
  } /* while */
  
  leds_off();
} /* run_lab5_part3 */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function scans the keypad and flashes the bar LEDs the amount of times
//  read from the keypad (0 to F). The user can enter four key presses.
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
void run_lab5_part4(void)
{
  leds_enable();
  uint8_t loop_counter = 0;
  uint8_t flash_amount = 0;

  while (loop_counter < 4)
  {
    uint8_t flash_counter = 0;
    do
    {
      flash_amount = keypad_scan();
    } while (flash_amount == 0x10);

    while (flash_counter < flash_amount)
    {
      // turn all leds on
      leds_on(0xFFFF);
      msec_delay(PART_DELAY);
      leds_off();
      msec_delay(PART_DELAY);
      flash_counter++;
    } /* while */

    debounce_keypad();
    loop_counter++;
  } /* while */
} /* run_lab5_part4 */