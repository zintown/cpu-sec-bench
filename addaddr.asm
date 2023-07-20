PUBLIC AdderAsm
.code _text

AdderAsm PROC PUBLIC

add     rcx, rdx;
add     rcx, r8;
mov     rax, rcx;

ret

AdderAsm ENDP

AdderAsm2 PROC PUBLIC
int 3
ret
AdderAsm2 ENDP 

END