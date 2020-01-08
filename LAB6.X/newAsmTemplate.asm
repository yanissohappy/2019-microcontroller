LIST p=18f4520
#include<p18f4520.inc>
CONFIG OSC = INTIO67
CONFIG WDT = OFF
CONFIG LVP = OFF

#define SWITCH PORTA, 4 ;switch = RA4
    L1 EQU 0x14
    L2 EQU 0x15

   ORG 0X00

DELAY MACRO num1,num2
    local LOOP1
    local LOOP2
    movlw num2
    movwf L2

    LOOP2:
    movlw num1
    movwf L1
    LOOP1:
    nop
    nop
    nop
    nop
    decfsz L1,1
    goto LOOP1
    decfsz L2,1
    goto LOOP2
ENDM

clrf PORTA
clrf LATA
bsf TRISA, 4	
clrf TRISD
clrf LATD

;movlw b'00001101'
;movwf LATD  

state0:
movlw b'11110000'
movwf LATD
DELAY d'71',d'248'
state0_0:
movlw b'11110000'
movwf LATD    
btfsc SWITCH    
bra state0_0 ;如果是0的話就會一直停在state0_0

state1:
rlncf LATD,1
DELAY d'71',d'248'
state1_0:
movlw b'11110000'
movwf LATD   
btfsc SWITCH    
bra state1_0 ;如果是0的話就會一直停在state1_0

state2:
rlncf LATD,1
rlncf LATD,1    
DELAY d'71',d'248'
state2_0:
movlw b'11110000'
movwf LATD    
btfsc SWITCH    
bra state2_0 ;如果是0的話就會一直停在state2_0    

state3:
rlncf LATD,1
rlncf LATD,1
rlncf LATD,1
DELAY d'71',d'248'
state3_0:
movlw b'11110000'
movwf LATD   
btfsc SWITCH    
bra state3_0 ;如果是0的話就會一直停在state3_0        

state4:
rlncf LATD,1
rlncf LATD,1
rlncf LATD,1
rlncf LATD,1
DELAY d'71',d'248'
state4_0:
movlw b'11110000'
movwf LATD   
btfsc SWITCH    
bra state4_0 ;如果是0的話就會一直停在state4_0
bra state0   

end