//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  Rafael Ortiz
//
//       LAB NAME:  Lab 2 (part 3)
//
//      FILE NAME:  lab2_p3_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code tests a basic struct and a union, displaying the data and sizes 
//    of the values of each.
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


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void msp_printf(char* buffer, unsigned int value);

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


// Define a structure to hold different data types
typedef struct
{
  unsigned  long  int   LA;
  signed    long  int   LB;
  unsigned        int   IA[2];
  signed          int   IB[2];
  unsigned  short int   SA[2];
  signed    short int   SB[2];
  unsigned       char   CA[4];
  signed         char   CB[4];
} test_struct;


// Define a union to hold a value in different formats
typedef union
{
  unsigned  long  int   LA;
  signed    long  int   LB;
  unsigned        int   IA;
  signed          int   IB;
  unsigned  short int   SA[2];
  signed    short int   SB[2];
  unsigned       char   CA[4];
  signed         char   CB[4];
} union_32;


int main(void)
{
  // Define local variables
  test_struct struct_var;
  union_32    union_var;

  // Initialize long integer
  union_var.LA = 0x1234ABCD;

  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
  lp_leds_init();

  UART_init(115200);

  // Lab 2 Code
  msp_printf(" *** PROGRAM RUNNING ***\r\n\r\n", 0);
  msp_printf("CSC-202 MSPM0G3507 (ARM M0+) Data Type Tester\r\n", 0);
  msp_printf("--------------------------------------------------------\r\n", 0);
  msp_printf(" unsigned long int = 0x%x bytes\r\n", sizeof(struct_var.LA));
  msp_printf("   signed long int = 0x%x bytes\r\n", sizeof(struct_var.LB));
  msp_printf("      unsigned int = 0x%x bytes\r\n", sizeof(struct_var.IA[0]));
  msp_printf("        signed int = 0x%x bytes\r\n", sizeof(struct_var.IB[0]));
  msp_printf("unsigned short int = 0x%x bytes\r\n", sizeof(struct_var.SA[0]));
  msp_printf("  signed short int = 0x%x bytes\r\n", sizeof(struct_var.SB[0]));
  msp_printf("     unsigned char = 0x%x bytes\r\n", sizeof(struct_var.CA[0]));
  msp_printf("       signed char = 0x%x bytes\r\n", sizeof(struct_var.CB[0]));

  msp_printf("\r\n", 0);
  msp_printf(" The structure = 0x%x bytes\r\n", sizeof(struct_var));
  msp_printf("     The union = 0x%x bytes\r\n", sizeof(union_var));
  msp_printf("\r\n", 0);

  msp_printf("Dumping data values from Union\r\n", 0);
  msp_printf("--------------------------------------------------------\r\n", 0);
  msp_printf("    union unsigned long int LA = 0x%X\r\n", union_var.LA);
  msp_printf("      union signed long int LB = 0x%X\r\n", union_var.LB);
  msp_printf("\r\n", 0);

  msp_printf("         union unsigned int IA = 0x%X\r\n", union_var.IA);
  msp_printf("           union signed int IA = 0x%X\r\n", union_var.IB);
  msp_printf("\r\n", 0);

  msp_printf("union unsigned short int SA[0] = 0x%X\r\n", union_var.SA[0]);
  msp_printf("  union signed short int SB[0] = 0x%X\r\n", union_var.SB[1]);
  msp_printf("union unsigned short int SA[1] = 0x%X\r\n", union_var.SA[0]);
  msp_printf("  union signed short int SB[1] = 0x%X\r\n", union_var.SB[1]);
  msp_printf("\r\n", 0);

  msp_printf("     union unsigned char CA[0] = 0x%X\r\n", union_var.CA[0]);
  msp_printf("     union unsigned char CA[1] = 0x%X\r\n", union_var.CA[1]);
  msp_printf("     union unsigned char CA[2] = 0x%X\r\n", union_var.CA[2]);
  msp_printf("     union unsigned char CA[3] = 0x%X\r\n", union_var.CA[3]);
  msp_printf("\r\n", 0);

  msp_printf("       union signed char CB[0] = 0x%X\r\n", union_var.CB[0]);
  msp_printf("       union signed char CB[1] = 0x%X\r\n", union_var.CB[1]);
  msp_printf("       union signed char CB[2] = 0x%X\r\n", union_var.CB[2]);
  msp_printf("       union signed char CB[3] = 0x%X\r\n", union_var.CB[3]);
  msp_printf("\r\n", 0);
  msp_printf("\r\n", 0);

  msp_printf(" *** PROGRAM TERMINATED ***\r\n", 0);

  // Endless loop to prevent program from ending
  while (1);

} /* main */


//------------------------------------------------------------------------------
// DESCRIPTION:
//  This function formats an integer value into a string according to a given
//  format and sends the resulting string to a serial port.
//
// INPUT PARAMETERS:
//  buffer: A format string used to format the integer value. It follows the
//          format specifiers used by 'sprintf'.
//  value:  The integer value to be formatted and included in the formatted
//          string.
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  none
//------------------------------------------------------------------------------
void msp_printf(char* buffer, unsigned int value)
{
  unsigned int i = 0;
  unsigned int len =0;
  char  string[80];

  len = sprintf(string, buffer, value);

  // Walk through array to send each character to serial port
  for (i = 0; i < len; i++)
  {
    UART_out_char(string[i]);
  } /* for */

} /* msp_printf */