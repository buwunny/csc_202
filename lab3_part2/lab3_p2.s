// Make the following symbol available outside of this file
.global my_asm_16bitset
.global my_asm_16bitclr
.global my_asm_16bitcheck


//-----------------------------------------------------------------------------
// DESCRIPTION:
//  This function sets the specified bit(s) in a 16-bit register value using
//  the provided bit mask. It performs a bitwise OR operation to set the bit(s).
//
// INPUT PARAMETERS:
//  R0 - The original 16-bit register value.
//  R1  - The bit mask indicating which bit(s) to clear.
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  R0 - The modified register value with the specified bit(s) set.
// -----------------------------------------------------------------------------
my_asm_16bitset:
  ORRS R0, R1 // R0 = R0|R1
  BX LR       // return


//-----------------------------------------------------------------------------
// DESCRIPTION:
//  This function clears the specified bit(s) in a 16-bit register value using
//  the provided bit mask. It performs a bitwise AND operation with the
//  complement of the bit mask to clear the bit(s).
//
// INPUT PARAMETERS:
//  R0 - The original 16-bit register value.
//  R1  - The bit mask indicating which bit(s) to clear.
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  R0 - The modified register value with the specified bit(s) cleared.
// -----------------------------------------------------------------------------
my_asm_16bitclr:
  BICS R0, R1 // R0 = R0&(~R1)
  BX LR       // return


//-----------------------------------------------------------------------------
// DESCRIPTION:
//  This function checks if the specified bit(s) in a 16-bit register value are
//  set using the provided bit mask. It performs a bitwise AND operation to 
//  verify if the bit(s) are set.
//
// INPUT PARAMETERS:
//  R0 - a 16-bit register value to check.
//  R1  - a 16-bit mask indicating which bit(s) to check.
//
// OUTPUT PARAMETERS:
//  none
//
// RETURN:
//  R0 - true if the specified bit(s) are set, false otherwise.
// -----------------------------------------------------------------------------
my_asm_16bitcheck:
  ANDS R0, R1 // R0 = R0&R1
  CMP R0, R1  // R0 == R1
  BEQ same    // branch if z == 1 (equal)
  MOVS R0, #0 // R0 = 0
  BX LR       // return

// label for when R0 and R1 are the same
same:
  MOVS R0, #1 // R0 = 1
  BX LR       // return