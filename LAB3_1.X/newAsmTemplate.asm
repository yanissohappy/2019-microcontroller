LIST p=18f4520
#include<p18f4520.inc>
CONFIG OSC = INTIO67
CONFIG WDT = OFF

org 0x00
clrf WREG

;實作NAND NAND即為and的not
;   0	1
;0  1	1
;1  1	0
;
;(題目)11010111 NAND 10011110 == 01101001
;發現兩個1的為0，其他都是1
;xor的性質是:全部跟1xor會toggle，即可做出complement的效果
;NAND -> (AB)'
    
movlw B'11010111'
andlw B'10011110'
xorlw B'11111111' ; 已經把11010111complement了
movwf LATA

clrf WREG

;實作NOR NOR即為or的not
;   0	1
;0  1	0
;1  0	0
;
;(題目)01010011 NOR 11100100 == 00001000
;發現兩個0的為1，其他都是0
;再次利用上面的性質~~~~
;NOR -> (A+B)'    

movlw B'01010011'
iorlw B'11100100'
xorlw B'11111111'
movwf LATB
    
end