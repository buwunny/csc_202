//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Rafael Ortiz
//
//       LAB NAME:  Lab 9
//
//      FILE NAME:  lab9_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code consists of a main loop with a state machine controlled by PB1
//    and PB2. There are states for each buzzer and both buzzers to be enabled
//    and disabled. Finally there is a state in which a melody is played from a
//    predetermined array of notes.
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
#include "demo_melodies.h"


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void config_pb1_interrupts(void);
void config_pb2_interrupts(void);
void GROUP1_IRQHandler(void);
void run_lab9_part1(void);
void display_frequency_and_duty_cycle(uint16_t frequency, uint8_t duty_cycle, bool display_percent);
void motor0_set_pwm_freq(uint16_t pwm_frequency);
void play_note(uint16_t freq, uint16_t duration_ms);


//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define DEBOUNCE_DELAY                                                      (10)
#define NOTE_DELAY                                                          (30)
#define DEFAULT_KEY                                                       (0x10)
#define DEFAULT_FREQUENCY                                                   (50)
#define enable_active_buzzer                            led_off(LED_BAR_LD1_IDX)
#define disable_active_buzzer                            led_on(LED_BAR_LD1_IDX)
#define enable_passive_buzzer                           led_off(LED_BAR_LD2_IDX)
#define disable_passive_buzzer                           led_on(LED_BAR_LD2_IDX)

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------
bool PB1_pressed = false;
bool PB2_pressed = false;


// Define an enum for the state machine
typedef enum
{
  BUZZER_IDLE = 0,
  ACTIVE_BUZZER,
  PASSIVE_BUZZER,
  BOTH_BUZZERS,
  PASSIVE_MELODY
} fsm_state_t;


int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
  dipsw_init();
  leds_init();
  leds_off();
  leds_enable();
  I2C_init();
  lcd1602_init();
  lcd_clear();
  keypad_init();
  motor0_init();
  motor0_pwm_init(4000, 0);
  motor0_pwm_enable();

  config_pb1_interrupts();
  config_pb2_interrupts();

  run_lab9_part1();
 
  NVIC_DisableIRQ(GPIOB_INT_IRQn);
  NVIC_DisableIRQ(GPIOA_INT_IRQn);
  leds_disable();
  motor0_pwm_disable();


  lcd_clear();
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Program Stopped");
  
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
//  This function contains the main loop for Lab 9. If PB1 is pressed during the
//  loop, the loop terminates. If PB2 is pressed during the loop, the next state
//  is processed by the state machine and the LCD is cleared. Whenever the state
//  is not PASSIVE_MELODY, the keypad is scanned and if a key is pressed then
//  the duty cycle is updated.
//  The state machine includes the following states:
//  BUZZER_IDLE:
//  • This is the reset or initial state, where both buzzers are disabled.
//  • LCD line 1 displays: "Buzzers Disabled" (centered)
//  • LCD line 2 displays: Current frequency and duty cycle using the format F=xxxxx DC=xxx%
//  • This state monitors key presses from the keypad, updates the LCD, and PWM as required.
//  • If PB2 is pressed, the state machine transitions to ACTIVE_BUZZER.
//  ACTIVE_BUZZER:
//  • In this state, the active buzzer is enabled.
//  • LCD line 1 displays: "Active Buzzer" (centered)
//  • LCD line 2 displays: Current frequency and duty cycle using the format F=xxxxx DC=xxx%
//  • This state monitors key presses from the keypad, updates the LCD, and PWM as required.
//  • If PB2 is pressed, the state machine transitions to PASSIVE_BUZZER.
//  PASSIVE_BUZZER:
//  • In this state, the passive buzzer is enabled.
//  • The user controls the duty cycle (and thus the volume) using the keypad.
//  • LCD line 1 displays: "Passive Buzzer" (centered)
//  • LCD line 2 displays: Current frequency and duty cycle using the format F=xxxxx DC=xxx%
//  • This state monitors key presses from the keypad, updates the LCD, and PWM as required.
//  • If PB2 is pressed, the state machine transitions to BOTH_BUZZERS.
//  BOTH_BUZZERS:
//  • In this state, both buzzers are enabled.
//  • LCD line 1 displays: "Both Buzzers" (centered)
//  • LCD line 2 displays: Current frequency and duty cycle using the format F=xxxxx DC=xxx%
//  • This state monitors key presses from the keypad, updates the LCD, and PWM as required.
//  • If PB2 is pressed, the state machine transitions to PLAY_MELODY.
//  PLAY_MELODY:
//  • In this state, the passive buzzer is enabled using PWM.
//  • LCD line 1 displays: "Play Melody" (centered)
//  • LCD line 2 displays: Current frequency and duration using the format F=xxxxx DC=xxx
//  • This state ignores key presses on the keypad. Instead, the PWM frequency and LCD is updated based on the
//    demo melodies with the support file.
//  • If PB2 is pressed, the state machine transitions to BUZZER_IDLE.
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
void run_lab9_part1(void)
{
  bool done = false;

  fsm_state_t current_state = BUZZER_IDLE;
  fsm_state_t next_state = ACTIVE_BUZZER;

  uint8_t duty_cycle = 0;

  while (!done)
  {
    if (PB1_pressed)
    {
      msec_delay(DEBOUNCE_DELAY);
      done = true;
      PB1_pressed = false;
    } /* if */

    if (PB2_pressed)
    {
      msec_delay(DEBOUNCE_DELAY);
      current_state = next_state;
      lcd_clear();
      PB2_pressed = false;
    } /* if */

    if (current_state != PASSIVE_MELODY)
    {              
      uint8_t key_scanned = keypad_scan();
      if (key_scanned != DEFAULT_KEY)
      {
        duty_cycle = (key_scanned * 100) / 15;
      } /* if */
    } /* if */


    // state machine
    switch (current_state)
    {
      case BUZZER_IDLE:
        next_state = ACTIVE_BUZZER;
        motor0_set_pwm_dc(0);
        motor0_set_pwm_freq(0);
        motor0_pwm_enable();
        lcd_set_ddram_addr(LCD_LINE1_ADDR);
        lcd_write_string("Buzzers Disabled");
        disable_active_buzzer;
        disable_passive_buzzer;
        display_frequency_and_duty_cycle(0, 0, true);
        break;
      
      case ACTIVE_BUZZER:
        next_state = PASSIVE_BUZZER;
        motor0_set_pwm_dc(duty_cycle);
        motor0_set_pwm_freq(DEFAULT_FREQUENCY);
        motor0_pwm_enable();
        lcd_set_ddram_addr(LCD_LINE1_ADDR);
        lcd_write_string("Active Buzzer");        
        enable_active_buzzer;
        disable_passive_buzzer;
        display_frequency_and_duty_cycle(DEFAULT_FREQUENCY, duty_cycle, true);
        break;
      
      case PASSIVE_BUZZER:
        next_state = BOTH_BUZZERS;
        motor0_set_pwm_dc(duty_cycle);
        motor0_set_pwm_freq(DEFAULT_FREQUENCY);
        motor0_pwm_enable();
        lcd_set_ddram_addr(LCD_LINE1_ADDR);
        lcd_write_string("Passive Buzzer");
        enable_passive_buzzer;
        disable_active_buzzer;
        display_frequency_and_duty_cycle(DEFAULT_FREQUENCY, duty_cycle, true);
        break;
      
      case BOTH_BUZZERS:
        next_state = PASSIVE_MELODY;
        motor0_set_pwm_dc(duty_cycle);
        motor0_set_pwm_freq(DEFAULT_FREQUENCY);
        motor0_pwm_enable();
        lcd_set_ddram_addr(LCD_LINE1_ADDR);
        lcd_write_string("Both Buzzers");
        enable_active_buzzer;
        enable_passive_buzzer;
        display_frequency_and_duty_cycle(DEFAULT_FREQUENCY, duty_cycle, true);
        break;
      
      case PASSIVE_MELODY:
        next_state = BUZZER_IDLE;
        lcd_set_ddram_addr(LCD_LINE1_ADDR);
        lcd_write_string("Play Melody");
        enable_passive_buzzer;
        disable_active_buzzer;
        for (uint8_t i = 0; i < IMPERIAL_MARCH_LENGTH; i++)
        {
          song_struct_t note = imperial_march[i];
          if (note.freq != 0)
          {
            uint8_t melody_dc = 10;
            display_frequency_and_duty_cycle(note.freq, note.duration, false);
            play_note(note.freq, note.duration);
            msec_delay(NOTE_DELAY);
          }
          if (PB1_pressed || PB2_pressed)
          {
            // end loop early if either button is pressed
            i = IMPERIAL_MARCH_LENGTH;
          } /* if */
        } /* for */
        break;
      msec_delay(DEBOUNCE_DELAY);
    } /* switch */
  }/* while */

} /* run_lab9_part1 */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function is a helper method to write the second line of the LCD during
//  the state machine. It sets the DDRAM address to line 2, writes "F=" then the 
//  frequency parameter, then " DC=" then the duty cycle parameter. Then if
//  display_percent is true it displays a percent sign at the end.  
//
// INPUT PARAMETERS:
//  frequency - value to display after "F="
//  duty_cycle - value to display after " DC="
//  display_percent - whether or not to display the "%" symbol at the end
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//------------------------------------------------------------------------------
void display_frequency_and_duty_cycle(uint16_t frequency, uint8_t duty_cycle, bool display_percent)
{
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("F=");
  lcd_write_doublebyte(frequency);
  lcd_write_string(" DC=");
  lcd_write_byte(duty_cycle);
  if (display_percent)
  {
    lcd_write_char('%');
  } /* if */
} /* display_frequency_and_duty_cycle */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function is a helper method to set the PWM frequency of motor0. This
//  was copied from the e-book. 
//
// INPUT PARAMETERS:
//  pwm_frequency - the frequency to set motor0 to
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//------------------------------------------------------------------------------
void motor0_set_pwm_freq(uint16_t pwm_frequency)
{
  // Determine what the timer clock divider and prescaler are
  uint32_t tim_clk_divider = TIMA0->CLKDIV + 1;
  uint32_t tim_clk_scaler = (TIMA0->COMMONREGS.CPS & GPTIMER_CPS_PCNT_MASK) + 1;
  // Now determine what the timer clock frequency is
  uint32_t tim_clk = get_bus_clock_freq() / (tim_clk_divider * tim_clk_scaler);
  // Update the load value based on new PWM frequency
  uint32_t load_value = (tim_clk / pwm_frequency);
  // Disable timer and update the load register
  motor0_pwm_disable();
  TIMA0->COUNTERREGS.LOAD = (load_value - 1) & GPTIMER_LOAD_LD_MASK;
} /* motor0_set_pwm_freq */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function is a helper method to play a note from the buzzer. This was
//  copied from the e-book. 
//
// INPUT PARAMETERS:
//  freq - frequency of the note to play
//  duration_ms - duration in milliseconds of the note
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//------------------------------------------------------------------------------
void play_note(uint16_t freq, uint16_t duration_ms)
{
  // Only enable the PWM if we have a frequency
  if (freq != 0)
  {
    motor0_set_pwm_freq(freq);
    motor0_set_pwm_dc(50);
    motor0_pwm_enable();
    msec_delay(duration_ms);
    motor0_pwm_disable();
  } /* if */
} /* play_note */