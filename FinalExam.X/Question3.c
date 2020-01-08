/*
 * File:   lab9.c
 * Author: User
 *
 * Created on 2019年12月1日, 下午 10:19
 */

// PIC18F4520 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

int start_count_down = 0;
int three_times = 0;

void main(void) {
    TRISBbits.TRISB0 = 1; //input 因為 RB0要當作input按鈕用
    TRISCbits.TRISC2 = 0; //output 因為要接馬達
    
    //500kHz
    OSCCONbits.IRCF2 = 0;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;

    //;; 用訊號脈波設定!!!因為知道規格中訊號脈波設定：50Hz (即20ms)-> PWM period = [PR2+1]*4*TOSC*TMR2的預除器 = 20ms = 20000 * 10^(-6) sec
    //;; TOSC的單位是1*10^-6!!!!!!!!!!!!!!!!!重要!!!!
    //;; [PR2+1] = 20000/4/2/16 = 156.25 = 156
    //;; PR2 = 155   所以要在PR2裡塞155 ^^
    
    // PR2放進period
    PR2 = 155;
    
    // 設定postscale prescale
    T2CON = 0x07; // bit2是設定timer2開啟  POSTSCALE = 1:1 , PRESCALE = 1:16
 
    // 打開所有interrupt
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;

    // 設mode成PWM
    CCP1CONbits.CCP1M = 0x0C; // PWM mode 1100 P1A,P1C active-high ; P1B, P1D active-high
    
    CCP1CONbits.DC1B = 0; // 就是CCP1CON<5:4>
    //; 再設定 PWM Duty period = (CCPRxL + CCPxCON<bit5:4>)*石英震盪器(週期)*TMR2的預除器
    //; 500 * 10^(-6) => -90度
    //; 2400 * 10^(-6) => 90度
    //; 因為石英震盪器是2(單位us)
    //; 且TMR2的預除器為16
    //; 先想求 -90度的情形好惹 也就是500 * 10^(-6)為duty period -> 500 * 10^(-6) = (CCPRxL + CCPxCON<bit5:4>)*石英震盪器(週期)*TMR2的預除器
    //; (CCPRxL + CCPxCON(bit5 4)) = 500 / 2 / 16 = 15.625 = 16
    //; 所以要在上面那整陀裡面塞16
    //; (_ _ _ _ _ 1 _ _) (_ _) <-這樣塞
    //; 也就是在CCPRxL的bit2塞進1    
    //; 也就是在CCPR1L 塞4的意思~~~~~
    
    //;; 設 DUTY TIME
    //;; (CCPRxL + CCPxCON(bit5 4))*石英震盪器(週期)*TMR2的預除器 = 2400 * 10^(-6) => 90度
    //;; (CCPRxL + CCPxCON(bit5 4)) = 2400 / 2 / 16 = 75
    //;; CCPRxL = 75/4 = 18.25 = 18
    //;; 故4~18 為-90~90的轉法
    
    CCPR1L = 5;

    TMR2 = 0;
    INTCONbits.INT0IE = 1;
    INTCON2bits.INTEDG0 = 0; //;1->0時觸發(也就是按下按鈕時!!!!)    
    
    //for TMR1
    T1CONbits.RD16 = 1; //use 16 bits
    T1CONbits.TMR1ON = 1;
    T1CONbits.TMR1CS = 0; //Clock source = internal clock
    T1CONbits.T1CKPS = 2; //預除器1:4
    
    TMR1 = 34285; //65535 - 500kHz/16
    
    PIE1bits.TMR1IE = 1;
    IPR1bits.TMR1IP = 1; //low priority
    PIR1bits.TMR1IF = 0;
    
    while(1);
}

void __interrupt (high_priority) myIsr (void){
    if(INTCONbits.INT0IF){
        INTCONbits.INT0IF = 0;
        INTCONbits.INT0IE = 0;
        
        start_count_down = 1;
        three_times = 0;
        //CCPR1L = 5;
        ////////////////
        while(CCPR1L > 5){ // -90: 100 00
            
            while(CCP1CONbits.DC1B > 0){
                CCP1CONbits.DC1B--;
            }
            if(CCPR1L == 5 && CCP1CONbits.DC1B == 0){
                break;
            }            
            _delay(10000);
            CCP1CONbits.DC1B = 3;
            CCPR1L--;
        }            
        ////////////////
        INTCONbits.INT0IE = 1;
        _delay(100);
    }
    
    if(start_count_down == 1 && PIR1bits.TMR1IF){
        PIR1bits.TMR1IF = 0;
        three_times++;
        CCPR1L+=2;
        if(three_times == 4) start_count_down = 0;       
    }
}