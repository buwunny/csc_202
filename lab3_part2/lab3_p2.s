// Make the following symbol available outside of this file
.global my_asm_16bitset
.global my_asm_16bitclr
.global my_asm_16bitcheck

my_asm_16bitset:
  ORRS R0, R1 // R0 = R0|R1
  BX LR       // return


my_asm_16bitclr:
  BICS R0, R1 // R0 = R0&(~R1)
  BX LR       // return


my_asm_16bitcheck:
  ANDS R0, R1 // R0 = R0&R1
  BX LR       // return