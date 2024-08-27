.section .note.gnu.property, "a", @note
        .balign 8
        .long 4                 /* n_namsz: Length of n_name */
        .long 16                /* n_descz: note descriptor size */
        .long 5                 /* n_type: NT_GNU_PROPERTY_TYPE_0 */
        .asciz "GNU"            /* n_name */
        .balign 8
        .long 3221225474        /* pr_type: GNU_PROPERTY_X86_FEATURE_1_AND */
        .long 4                 /* pr_datasz: Length of pr_data = 4 */
        .long 0x3               /* pr_data: property descriptor */
        .balign 8

.section .text
.globl assembly_return_site
.globl assembly_helper
.globl exit
.type exit, %function

assembly_helper:
  # Save address
  movq %rdi, %rax
  # Pop the return address from the stack
  popq %rbx
  # Save the fake return address to the stack
  pushq %rax
  ret


assembly_return_site:
  # Save the stack info
  push %rbp
  movq %rsp, %rbp
  # Set exit code
  xor %rdi, %rdi
  call exit
  # Restore stack info
  movq %rbp, %rsp
  pop %rbp
  ret