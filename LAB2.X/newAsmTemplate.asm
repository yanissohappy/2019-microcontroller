LIST p=18f4520
#include<p18f4520.inc>
CONFIG OSC = INTIO67
CONFIG WDT = OFF

org 0x00
clrf WREG

putThing macro arg1,arg2
    movlw arg1
    movff WREG, arg2
    endm
    
  
movlf macro arg1, arg2;arg1是要移入的值、arg2是要移入的位址 我只是為了要用TRISD才設這個的=v=
    movlw arg1
    movwf arg2 ,0
    endm
    
    putThing 0x39, 0x0100
    putThing 0x4e, 0x0101
    putThing 0x4f, 0x0102
    putThing 0x1f, 0x0103
    putThing 0x0d, 0x0104
    putThing 0x56, 0x0105
    putThing 0x60, 0x0106
    putThing 0x0d, 0x0107
    putThing 0x52, 0x0108
    putThing 0x4e, 0x0109
    putThing 0x60, 0x010A
    putThing 0x66, 0x010B
    
   
    clrf WREG ;清掉WREG
    clrf BSR ;清掉BSR
    
    movlf D'12',TRISD
    lfsr 0,0x100 ;選定FSR0，然後將0x100(位址)移入裡面
    lfsr 1,0x200
    ;movlw 0x13 ;因為之後全部都要加上13，所以先放好13在WREG裡
loop:  ;此迴圈是把所有的數加上13並同時直接複製到0x200~0x20b的地址
    movlw 0x13 
    addwf INDF0 ;先原地加
    movf POSTINC0,0 ; 把FSR0的值移到WREG裡
    movwf POSTINC1 ; 把WREG的值丟進FSR1，之後FSR1再跳到下一個位址（這行跟上面那行合起來就是把0x100的值丟到0x200...以此類推）
    decfsz TRISD ;計數器--，如果為0，跳出迴圈
    goto loop

end



    
    
    
    
    
    
    
    
    
    
    
    