.section .text
.globl assembly_return_site
.globl assembly_helper
.globl exit
.type exit, %function

assembly_helper:
  sub sp, sp, #0x20
  stp x29, x30, [csp]
  str x0,[sp,#0x10]
  ldp x29,x30,[csp]
  add sp,sp,#0x20
  ret

assembly_return_site:
  sub sp, sp, #0x20
  stp x29, x30, [csp]
  mov x0, #0x0
  bl exit
  ldp x29, x30, [csp]
  add sp, sp, #0x20
  ret
