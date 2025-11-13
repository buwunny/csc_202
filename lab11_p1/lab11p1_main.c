//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Rafael
//
//       LAB NAME:  Lab 11, Part 1
//
//      FILE NAME:  lab11p1_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code uses SPI and a shift register (74HC595) to set data, send data,
//    and update output. The output of the shift register is connected to 8
//    leds.
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
#include "uart.h"
#include "spi.h"


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void spi_init(void);
void run_lab11_part1(void);
void display_menu(void);
uint8_t get_spi_data(void);
void send_data(uint8_t data);
void update_leds(void);
void end_program(void);
uint16_t string_to_uint16(char* string);
void UART_write_string(char* string);


//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define BUFFER_LENGTH                                                        (3)
#define CARRIAGE_RETURN_CHAR                                              ('\r')
#define BACKSPACE_CHAR                                                    ('\b')
#define ZERO_CHAR                                                          ('0')
#define ONE_CHAR                                                           ('1')
#define TWO_CHAR                                                           ('2')
#define THREE_CHAR                                                         ('3')
#define FOUR_CHAR                                                          ('4')
#define SPIC_CSC0_DELAY                                                      (1)

//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
 
  // enter your code here
  I2C_init();
  lcd1602_init();
  UART_init(115200);
  spi_init();
 
  run_lab11_part1();

  // Endless loop to prevent program from ending
  while (1);

} /* main */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function initialized SPI specifically for this lab. After spi1_init the
//  CS0 pin is set to be a GPIO pin instead of an SPI pin so we can manually 
//  control it. Then, data out is enabled and cleared.
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
void spi_init(void)
{
  spi1_init();
  // configure CS0 as GPIO pin instead of SPI
  IOMUX->SECCFG.PINCM[LP_SPI_CS0_IOMUX] = (IOMUX_PINCM_PC_CONNECTED |
    IOMUX_PINCM23_PF_GPIOB_DIO06);
  // enable output for this pin
  GPIOB->DOE31_0 |= LP_SPI_CS0_MASK;
  // clear DOUT
  GPIOB->DOUT31_0 &= ~LP_SPI_CS0_MASK;
} /* spi_init */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function contains the main loop of Lab 11, displaying the menu,
//  handling menu selection and calling the corresponding functions for each 
//  option.
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
void run_lab11_part1(void)
{
  lcd_clear();
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Program running");
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("See serial port");
  UART_write_string("\r\n");

  uint16_t set_data = 0;
  char user_input = ZERO_CHAR; // default value
  while (user_input != FOUR_CHAR)
  {
    display_menu();
    user_input = UART_in_char();
    UART_out_char(user_input);
    UART_write_string("\r\n\r\n");
    switch (user_input) {
      case ONE_CHAR:
        UART_write_string("Set SPI xmit data menu selected!\r\n");
        set_data = get_spi_data();
        break;
      case TWO_CHAR:
        UART_write_string("Send data selected!\r\n");
        send_data(set_data);
        break;
      case THREE_CHAR:
        UART_write_string("Update LEDs selected!\r\n");
        update_leds();
        break;
      case FOUR_CHAR:
        UART_write_string("Thank you for using the program!\r\n");
        end_program();
        break;
      default:
        UART_write_string("Please enter a valid option!\r\n");
        break;
    }
  }
} /* run_lab11_part1 */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function displays the menu in the serial console over UART. The options
//  include setting the data to send, sending the set data, updating the LEDs
//  with the sent data, and ending the program.
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
  UART_write_string("\r\n");
  UART_write_string("MENU OPTIONS\r\n");
  UART_write_string("   1. Set Data to Send\r\n");
  UART_write_string("   2. Send Data\r\n");
  UART_write_string("   3. Update LEDs\r\n");
  UART_write_string("   4. End Program\r\n");
  UART_write_string("Enter your selection: ");
} /* display_menu */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function gets the data from the user and returns it. It uses the UART
//  and a buffer to enter the data, limits it to 3 characters, and once return 
//  is pressed, the data is validated. If the data is invalid (<0 or >255 or 
//  non-number) a 0 is returned.
//
// INPUT PARAMETERS:
//  none
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  input - the validated data entered by the user
//------------------------------------------------------------------------------
uint8_t get_spi_data(void)
{
  UART_write_string("\r\nEnter a valid decimal number (0 to 255): ");
  char buffer[BUFFER_LENGTH];
  uint8_t index = 0;
  char key;
  do
  {
    key = UART_in_char();
    if (key == CARRIAGE_RETURN_CHAR)
    {
      UART_out_char(key);
      buffer[index] = NULL;
    } /* if */
    else if (key == BACKSPACE_CHAR) {
      if (index > 0)
      {
        UART_out_char(key);
        index--;
      } /* if */
    } /* else if */
    else {
      if (index < BUFFER_LENGTH)
      {
        UART_out_char(key);
        buffer[index] = key;
        index++;
      }
    } /* else */
  } while (key != CARRIAGE_RETURN_CHAR);
  UART_write_string("\r\n");

  // convert input to uint
  uint16_t input = string_to_uint16(buffer);
  
  // validate data
  if (input < 0 || input > UINT8_MAX || input == NULL)
  {
    input = 0;
    UART_write_string("ERROR: Invalid data entered.\r\n");
    UART_write_string("SPI data set to 0");
  } /* if */
  else 
  {
    UART_write_string("SPI data set to ");
    UART_write_string(buffer);  
  } /* else */
  UART_write_string("\r\n");

  return (uint8_t) input;
} /* get_spi_data */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function actually transmits the data over SPI to the shift register.
//  The value sent to the shift register is displayed after "SEND: " on the
//  first line and the value read from the shift register is displayed after
//  "RECV: " on the second line.
//
// INPUT PARAMETERS:
//  data - data to send over SPI
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//------------------------------------------------------------------------------
void send_data(uint8_t data)
{
  spi1_write_data(data);
  uint8_t read_data = spi1_read_data();
  lcd_clear();
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("SEND: ");
  lcd_write_byte(data);
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("RECV: ");
  lcd_write_byte(read_data);
} /* send_data */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function updates the LEDs with the value in the shift register by
//  toggling the CS0 pin which has been changed to a GPIO pin (see spi_init()).
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
void update_leds(void)
{
  GPIOB->DOUTSET31_0 = LP_SPI_CS0_MASK;
  msec_delay(SPIC_CSC0_DELAY);
  GPIOB->DOUTCLR31_0 = LP_SPI_CS0_MASK;;
} /* update_leds */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function ends the program, clearing the lcd and writing "Program 
//  stopped" to the first line of the LCD.
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
  lcd_clear();
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  lcd_write_string("Program stopped");
} /* end_program */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function converts a given string to a uint16_t by getting each char and
//  calculating the value.
//
// INPUT PARAMETERS:
//  string - the string to convert to a uint16_t
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//------------------------------------------------------------------------------
uint16_t string_to_uint16(char* string)
{
  uint16_t result = 0;

  // Convert the string to an integer
  while (*string != NULL)
  {
    result = result * 10 + (*string - '0');
    string++;
  } /* while */

  return result;
} /* string_to_uint16 */


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
  while (current_char != NULL)
  {
    current_char = *(string + index++);
    UART_out_char(current_char); 
  } /* while */
} /* UART_write_string */