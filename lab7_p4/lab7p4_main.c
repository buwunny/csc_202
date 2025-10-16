//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Rafael Ortiz
//
//       LAB NAME:  Lab 7, Part 4
//
//      FILE NAME:  lab7p4_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code counts up from 0 to 99 and upon reaching 99 goes back to 0 until
//    PB1 is pressed. The current count is displayed on the first line of the
//    LCD. Simultaneously, "PB2 PRESSED" is displayed on the second line of the
//    LCD when PB2 is pressed and disappears when pressed again. 
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
#include "ti/devices/msp/peripherals/hw_gpio.h"
#include "ti/devices/msp/peripherals/m0p/hw_cpuss.h"
#include "clock.h"
#include "LaunchPad.h"
#include "lcd1602.h"


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void config_pb1_interrupts(void);
void config_pb2_interrupts(void);
void GROUP1_IRQHandler(void);
void run_lab7_part4(void);


//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define COUNTER_DELAY                                                      (200)
#define DEBOUNCE_DELAY                                                      (10)


//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------
bool PB1_pressed;
bool PB2_pressed;


int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
  leds_init();
  leds_disable();
  seg7_init();
  dipsw_init();
  I2C_init();
  lcd1602_init();
  lcd_clear(); 

  config_pb1_interrupts();
  config_pb2_interrupts();
 
  run_lab7_part4();

  // Endless loop to prevent program from ending
  while (1);

} /* main */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function configures the interrupt for PB2 by setting the polarity to
//  rising edge, clearing Interrupt Clear Register (ICLR) and setting the
//  Interrupt Mask (IMASK). Finally the priority of the interrupt is set and the
//  interrupt is enabled.
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
void config_pb1_interrupts(void)
{
  // Set PB1 to rising edge
  GPIOB->POLARITY31_16 = GPIO_POLARITY31_16_DIO18_RISE;
  
  // Ensure bit is cleared
  GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;
  
  
  // Unmask PB1 to allow interrupt
  GPIOB->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO18_SET;

  NVIC_SetPriority(GPIOB_INT_IRQn, 2);
  NVIC_EnableIRQ(GPIOB_INT_IRQn);
} /* config_pb1_interrupts */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function configures the interrupt for PB1 by setting the polarity to
//  rising edge, clearing Interrupt Clear Register (ICLR) and setting the
//  Interrupt Mask (IMASK). Finally the priority of the interrupt is set and the
//  interrupt is enabled.
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
void config_pb2_interrupts(void)
{
  // Set PB2 to rising edge
  GPIOA->POLARITY15_0 = GPIO_POLARITY15_0_DIO15_RISE;
  
  // Ensure bit is cleared
  GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;
  
  // Unmask PB2 to allow interrupt
  GPIOA->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO15_SET;
  
  NVIC_SetPriority(GPIOA_INT_IRQn, 2);
  NVIC_EnableIRQ(GPIOA_INT_IRQn);
} /* config_pb1_interrupts */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function represents the ISR (Interrupt Service Routine) for the 
//  GROUP1_IRQHandler. It is called when there is an input on GPIOA or GPIOB.
//  This ISR is responsible for updating global flags to true when PB1 or PB2 is
//  pressed.
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
void GROUP1_IRQHandler(void)
{
  bool display;

  uint32_t group_iidx_status;
  uint32_t gpio_mis;
  do 
  {
    group_iidx_status = CPUSS->INT_GROUP[1].IIDX;

    switch (group_iidx_status)
    {
      // Interrupt from GPIOA
      case (CPUSS_INT_GROUP_IIDX_STAT_INT0):
        gpio_mis = GPIOA->CPU_INT.MIS;
        if ((gpio_mis & GPIO_CPU_INT_MIS_DIO15_MASK) == GPIO_CPU_INT_MIS_DIO15_SET)
        {
          PB2_pressed = true;
          // Manually clear bit to acknowledge interrupt
          GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;
        } /* if */
        break;

      // Interrupt from GPIOB        
      case (CPUSS_INT_GROUP_IIDX_STAT_INT1):
        gpio_mis = GPIOB->CPU_INT.MIS;
        if ((gpio_mis & GPIO_CPU_INT_MIS_DIO18_MASK) == GPIO_CPU_INT_MIS_DIO18_SET)
        {
          PB1_pressed = true;
          // Manually clear bit to acknowledge interrupt
          GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;
        } /* if */
        break;

      // unexpected interrupt
      default:
        break;
    } /* switch */
  } while (group_iidx_status != 0);
} /* GROUP1_IRQHandler */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function infinitely counts from 0 to 99 and upon reaching 99 it resets.
//  There is a 0.2 second delay between each count. The count is displayed on
//  LCD line 1 (centered). All LEDs are disabled and unused seven-segment
//  displays are disabled. If PB1 is pressed the infinite loop ends. If PB2 is
//  pressed, "PB2 PRESSED" is displayed to the second line of the LCD until PB2
//  is pressed again.
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
void run_lab7_part4(void)
{
  leds_disable();
  seg7_off();

  bool done = false;
  bool message_displayed = false;
  uint8_t count = 0;
  while (!done)
  {
    // update done flag to true if PB1 is pressed
    done = PB1_pressed;

    // update display_message flag to the opposite value when PB2 is pressed
    if (PB2_pressed)
    {
      // prevent unexpected behavior from multiple button presses
      msec_delay(DEBOUNCE_DELAY);

      // if message is not shown, show it and set the flag to true
      if (!message_displayed)
      {
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        lcd_write_string("PB2 PRESSED");
        message_displayed = true;
        // reset PB2_pressed to false so we can read another press
        PB2_pressed = false;
      } /* if */
      
      // if message is shown, hide it and set the flag to false
      else
      {
        lcd_clear();
        message_displayed = false;
        // reset PB2_pressed to false so we can read another press
        PB2_pressed = false;
      } /* else */
    } /* if */

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

  lcd_clear();
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Program Stopped");
} /* rub_lab7_part4 */