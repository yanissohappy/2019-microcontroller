LIST p=18f4520
#include<p18f4520.inc>
CONFIG OSC = INTIO67
CONFIG WDT = OFF

org 0x00
clrf WREG
clrf TRISA  
clrf TRISB  
clrf TRISC  
clrf 0x30  

movlw 0x03  ;這邊助教可以改 0x07
movwf TRISB 
rlncf TRISB
rlncf TRISB
rlncf TRISB
rlncf TRISB ;為不會動的被乘數
movlw 0x05   ;這邊助教可以改 0x0A
;movwf TRISC 
movwf TRISA
    
;;;;TRISB不要動

movlw 4
movwf 0x30 ;把計數值4隨便亂存~~~~~~~~~~~~~~
    
movff TRISB,WREG

loop:
    btfsc TRISA,0
    addwf TRISA,1
    rrcf TRISA
    bcf STATUS,C ;清掉Carry!!!記得不是用clrf!!!因為我下面要使用bnc 所以我要先把status的carry清空才能用bnc無限制地跳
    decfsz 0x30
    bnc loop 
    nop
    
    
end


