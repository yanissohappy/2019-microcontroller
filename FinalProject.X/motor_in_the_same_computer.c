/*
 * File:   motor_in_the_same_computer.c
 * Author: yan
 *
 * Created on 2019年12月28日, 上午 11:24
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

//我的馬達要接CCP1，幽靈的馬達要接CCP2

int temp1 = 0;
int temp2 = 0;

void main(void) {
    TRISBbits.TRISB0 = 1; //input
    TRISCbits.TRISC2 = 0; //RC2 port = output (CCP1)
    TRISCbits.TRISC1 = 0; //RC1 port = output (CCP2)
    
    //500kHz
    OSCCONbits.IRCF2 = 0;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    
    PR2 = 155;
    
    T2CON = 0x07; // bit2是設定timer2開啟  POSTSCALE = 1:1 , PRESCALE = 1:16
 
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;

    CCP1CONbits.CCP1M = 0x0C; // PWM mode 1100 P1A,P1C active-high ; P1B, P1D active-high
    
    CCP2CONbits.DC2B = 0;
    CCP2CONbits.CCP2M3 = 1; //set PWM mode
    CCP2CONbits.CCP2M2 = 1;
    CCP2CONbits.CCP2M1 = 0;
    CCP2CONbits.CCP2M0 = 0;    
    CCPR2L = 8;         
     
    CCP1CONbits.DC1B = 1; // 就是CCP1CON<5:4>
    CCPR1L = 10;

    temp1 = (CCPR1L << 2) + CCP1CONbits.DC1B;
    temp2 = (CCPR2L << 2) + CCP2CONbits.DC2B;
    
    TMR2 = 0;
    INTCONbits.INT0IE = 1;
    INTCON2bits.INTEDG0 = 0; //;1->0時觸發(也就是按下按鈕時!!!!)    
    INTCONbits.INT0IF = 0;
     while(1);
}

void __interrupt () myIsr (void){
    if(INTCONbits.INT0IF){
        INTCONbits.INT0IE = 0;
        
        while(1){ // -90: 100 00
            _delay(10000);
            temp1--;
            temp2++;
            CCPR1L = temp1 >> 2;
            CCPR2L = temp2 >> 2;
            CCP1CONbits.DC1B = temp1 & 0b00000011;
            CCP2CONbits.DC2B = temp2 & 0b00000011;
            if(CCPR1L < 6){
                break;
            }            

        }    
        
        while(1){
            _delay(10000);
            temp1++;
            temp2--;
            CCPR1L = temp1 >> 2;
            CCPR2L = temp2 >> 2;
            CCP1CONbits.DC1B = temp1 & 0b00000011;
            CCP2CONbits.DC2B = temp2 & 0b00000011;
            if (CCPR1L >= 13){
                break;
            }                        

        }
        
        INTCONbits.INT0IE = 1;
    }
}