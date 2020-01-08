/*
 * File:   breathing_light.c
 * Author: yan
 *
 * Created on 2019年12月22日, 上午 11:24
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

//CCP1要接燈泡(呼吸燈))
//RD0要接燈泡
//上面兩個東西要接在同一個孔，然後那個孔要用電阻接到燈泡的一端，燈泡的另一端要接負極

//要接正極的電阻到按鈕的一端，INT0也要接到正極的按鈕那端，按鈕的另一端要接到負極

void LED_Init();
int mode = 0; //0=increment, 1=decrement (for LED)
unsigned int result;
int temp_for_timer = 0;


void main(void) {
    LED_Init();
    while(1);
}

void LED_Init() //set as low priority
{
    TRISCbits.TRISC2 = 0; //use RC2 port as output (CCP1)
    //////////////////////////////////////
    OSCCONbits.IRCF2 = 0; //use 500kHz oscillator
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    //////////////////////////////////////
    T2CON = 0x07; //TMR2ON = 1, T2CKPS<1:0> = 11 (1:16 prescaler)
    TMR2 = 0;
    
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    
    PR2 = 16; //timer 2 is compared to PR2=16 at each cycle //period = 2ms
    CCPR1L = 0; //duty cycle = 0~2 ms
    CCP1CONbits.DC1B1 = 0;
    CCP1CONbits.DC1B0 = 0;
    
    CCP1CONbits.CCP1M3 = 1; //set CCP to PWM mode
    CCP1CONbits.CCP1M2 = 1;
    CCP1CONbits.CCP1M1 = 0;
    CCP1CONbits.CCP1M0 = 0;
    
    //for TMR1
    T1CONbits.RD16 = 1; //use 16 bits
    T1CONbits.TMR1ON = 1;
    T1CONbits.TMR1CS = 0; //Clock source = internal clock
    T1CONbits.T1CKPS = 3; //設定timer1預除器為1:8 
    
    TMR1 = 34285; //65535 - 500kHz/16
    PIE1bits.TMR1IE = 1;
    IPR1bits.TMR1IP = 1; 
    PIR1bits.TMR1IF = 0;
    
    // 設定ADC
    //ADC
    TRISAbits.TRISA0 = 1;
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    
    ADCON0 = 0b00000001;
    ADCON1 = 0b00001110;
    ADCON2 = 0b10010100;
    
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1; 
    IPR1bits.ADIP = 1;
    
    ADCON0bits.GO_DONE = 1;    
    
}

void __interrupt(high_priority) Hi_ISR(void)
{    
    if(PIR1bits.ADIF == 1){    
        ADCON0bits.GO_DONE = 0;
        PIR1bits.ADIF = 0;
        result = ADRESL + (ADRESH << 8);
        if(result >= 999){
            temp_for_timer = 61369;
        }
        else if(result >= 666){
            temp_for_timer = 63453;
        }
        else{
            temp_for_timer = 64493;
        }
        _delay(10);
        ADCON0bits.GO_DONE = 1;        
    } 
       
    if(TMR1IF)
    {
        TMR1IF = 0;
        if(mode == 0 && CCPR1L < 16)
        {
            CCPR1L = CCPR1L + 1;
            if(CCPR1L == 16) mode = 1;
        }
        else if(mode == 1 && CCPR1L > 0)
        {
            CCPR1L = CCPR1L - 1;
            if( CCPR1L == 0) mode = 0;
        }
        TMR1 = temp_for_timer;
    }
    
    return ;
}