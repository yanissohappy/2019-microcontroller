#include "xc.inc"
GLOBAL _divide

PSECT mytext, local, class=CODE, reloc=2

_divide:
 MOVFF 0x001, LATB ;被除數 低位(即remainder 0~8 :quotient)
 ;MOVFF 0x002, LATC ;被除數 高位(即remainder 9~15 :remainder)
 CLRF LATC
 MOVFF 0x003, TRISA ;除數
 
 MOVLW 0x08
 MOVWF LATA ;LATA放8
 BCF STATUS,0  
 RLCF LATB
 RLCF LATC

 step_2:
 MOVF TRISA, W
 SUBWF LATC, F
 BTFSS LATC, 7 ;看有沒有負數
 goto if_remainder_larger_than_0
 goto if_remainder_smaller_than_0 ;如果有負數的話

 if_remainder_larger_than_0:
 BCF STATUS,0   
 RLCF LATB
 RLCF LATC
 BSF LATB,0 ;最右邊設成1
 DCFSNZ LATA
 goto done ;如果lata ==0 就到done
 goto step_2

 if_remainder_smaller_than_0:
 MOVF TRISA, W
 ADDWF LATC, F
 BCF STATUS,0 ;一定要這樣打!!!否則會沒辦法編譯
 RLCF LATB
 RLCF LATC
 BCF LATB,0 ;最右邊設成0
 DCFSNZ LATA
 goto done ;如果lata ==0 就到done
 goto step_2

 done: 
 BCF STATUS,0   
 RRCF LATC 
 ;MOVFF LATB, 0x003
 ;CLRF 0x04
 ;MOVFF LATC, 0x004
 MOVFF LATB, 0x001
 MOVFF LATC, 0x002
 
    RETURN


