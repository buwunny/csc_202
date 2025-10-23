//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Rafael Ortiz
//
//       LAB NAME:  Lab 8, Part 2
//
//      FILE NAME:  lab8p2_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code interfaces with the potentiometer. The bar LEDs light up based
//    on how much the potentiometer is turned. There is a message displayed on 
//    the LCD to show the potentiometer ADC value.
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
#include "adc.h"


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void config_pb1_interrupts(void);
void config_pb2_interrupts(void);
void GROUP1_IRQHandler(void);
void run_lab8_part2(void);


//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define POT_CHANNEL                                                          (7)
#define NUM_STATES                                                           (9)
#define POT_ADC_STEP                                         (4096 / NUM_STATES)
#define DEBOUNCE_DELAY                                                      (10)


//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------
bool PB1_pressed;
bool PB2_pressed;


// Define a structure to hold different data types

int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
  dipsw_init();
  leds_init();
  leds_enable();
  I2C_init();
  lcd1602_init();
  lcd_clear();
  ADC0_init(ADC12_MEMCTL_VRSEL_VDDA_VSSA);

  config_pb1_interrupts();
  config_pb2_interrupts();

  run_lab8_part2();
 
  NVIC_DisableIRQ(GPIOB_INT_IRQn);
  NVIC_DisableIRQ(GPIOA_INT_IRQn);
  leds_disable();

  // Endless loop to prevent program from ending
  while (1);

} /* main */


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
//  This function reads the potentiometer analog value, converts it to digital,
//  and displays the value after "ADC = " on line 1 of the LCD. Once PB1 is
//  pressed, the loop terminates and "Program Stopped" is displayed on the LCD. 
//  The following guidelines were used to create this function:
//  • Clear the LCD.
//  • The first line of the LCD should display the ADC value: “ADC = ” followed 
//    by the ADC value
//  • All seven-segment displays are disabled.
//  • All LEDs are off when the potentiometer is rotated counter-clockwise to 
//    its minimum position.
//  • All LEDs are on when the potentiometer is fully rotated clockwise.
//  • The design must utilize a while loop with a flag to monitor channel 7 of 
//    the ADC continuously.
//  • If pushbutton PB1 is pressed (detected via an interrupt), a flag is set to
//    terminate the while loop, and the message “Program Stopped” is displayed 
//    on line 1 of the LCD.
//  • The program should turn off interrupts and other peripherals before 
//    terminating the program.
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
void run_lab8_part2(void)
{
  bool done = false;
  uint16_t adc_value;
  lcd_clear();

  while (!done)
  {
    done = PB1_pressed;

    adc_value = ADC0_in(POT_CHANNEL);

    uint8_t leds = adc_value / POT_ADC_STEP;
    for (int i = 0; i < NUM_STATES; i++) 
    {
      if (i < leds)
      {
        led_on(i);
      } /* if */
      else
      {
        led_off(i);
      } /* else */
    } /* while */

    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    lcd_write_string("ADC = ");
    lcd_write_doublebyte(adc_value);
  } /* while */

  lcd_clear();
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Program Stopped");
  msec_delay(DEBOUNCE_DELAY);
  PB1_pressed = false;
} /* run_lab8_part2 */