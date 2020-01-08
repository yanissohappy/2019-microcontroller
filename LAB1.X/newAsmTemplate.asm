LIST p=18f4520
#include<p18f4520.inc>
CONFIG OSC = INTIO67
CONFIG WDT = OFF

org 0x00
Initial:
;******************
start:
    clrf WREG
    clrf TRISD
    movlw 0x08
    movwf TRISD, 0
    movlw 0x00
    
loop:
    addwf TRISD, 0 ,0
    incf TRISD, 1, 0
    btfss TRISD, 4,0
    goto loop
end