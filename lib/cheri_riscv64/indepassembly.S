.section .text
.globl assembly_return_site
.globl assembly_helper
.globl exit
.type exit, @function

assembly_helper:
  addi csp, csp, -16
  # save return address
  sc.cap ra, 8(csp)
  # save frame pointer
  sc.cap s0, 0(csp)
  # modify return address
  sc.cap a0, 8(csp)
  # restore frame pointer
  ld s0, 0(csp)
  # restore return address
  ld ra, 8(csp)
  addi csp, csp, 16
  ret

assembly_return_site:
  addi csp, csp, -16
  # save return address
  sc.cap ra, 8(csp)
  # save frame pointer
  sc.cap s0, 0(csp)
  # set return code
  li a0, 0
  # exit
  li a7, 93
  ecall
  # restore frame pointer
  ld s0, 0(csp)
  # restore return address
  ld ra, 8(csp)
  addi csp, csp, 16
  ret
