LIST p=18f4520
#include<p18f4520.inc>
CONFIG OSC = INTIO67
CONFIG WDT = OFF

movlf macro arg1, arg2
    movlw arg1
    movwf arg2,0
    endm
    
addfff macro f1, f2, f_res
    movff f1,WREG
    addwf f2
    movwf f_res, 0
    endm    
    
org 0x00
clrf WREG ;0
    
    movlw D'11' ;02
    rcall Fib	;04
    rcall Finish    ;06
        
Fib:
    movwf LATA	;08
    movlf 0,0x31    ;10
    movlf 1,0x32    ;12
    ;;;這是我要計算的位置 這裡是16
    movff 0x32, 0x31
    movff 0x33, 0x32
    addfff 0x31, 0x32, 0x33
    dcfsnz LATA, 1, 0
    return
    movlf 12, PCL ;會跳回去16
    
    
Finish:
end
