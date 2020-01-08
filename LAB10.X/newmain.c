/*
 * File:   lab10.c
 * Author: User
 *
 * Created on 2019年12月7日, 下午 16:19
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

void adc_init(void);
void ccp2_init(void);
void timer3_init(void);
void map(int);

//int arr[8];
int i=0;
int is_five_times = 0;

void __interrupt(high_priority) Hi_ISR(void)
{
    //deal ccp2 interrupt and adc interrupt
    if(is_five_times < 5){
        is_five_times++;
    }else{
        is_five_times = 0;
        if(PIR1bits.ADIF){//AD conversion finish
            map(ADRES);            

            PIR1bits.ADIF = 0;
            ADCON0bits.ADON = 1;
            PIR2bits.CCP2IF = 0; //以防萬一(但應沒有這種可能先關掉special event trigger)
        }
        else if(PIR2bits.CCP2IF){ //special event triger
            PIR2bits.CCP2IF = 0;
        }
    }
    return ;
}

void main(void) {
    adc_init();
    ccp2_init();
    timer3_init();
    while(1);
    return;
}

void map(int ad){
    if(ad <= 127) LATD = 0;
    else if(ad <= 255) LATD = 1;
    else if(ad <= 383) LATD = 2;
    else if(ad <= 511) LATD = 3;
    else if(ad <= 639) LATD = 4;
    else if(ad <= 767) LATD = 5;
    else if(ad <= 895) LATD = 6;
    else if(ad <= 1023) LATD = 7;
}

void adc_init(void){
    //datasheet p232 TABLE 19-2
    //Configure the A/D module
    
    OSCCONbits.IRCF = 7; // 設定oscillator 8MHz
    
    //ADCON0
    //select analog channel
    ADCON0bits.CHS3 = 0;
    ADCON0bits.CHS2 = 1;
    ADCON0bits.CHS1 = 1;
    ADCON0bits.CHS0 = 1; //設AN7為AD converter 的 input port
    TRISEbits.TRISE2 = 1; //把 RE2/AN7 port 設為 input
    
    ADCON0bits.ADON = 1; //把module打開
    //ADCON0bits.GO = 1; //若GO設起，代表conversion 在進行中，但我們用CCP2用special event trigger，它會自動幫我們控管^^
    
    //設參考電壓囉~~~~~
    ADCON1bits.VCFG1 = 0; // 用Vss
    ADCON1bits.VCFG0 = 0; // 用Vdd

    //要決定一個Tad 代表的時間是多少!!!! 
    ADCON2bits.ADCS2 = 0;
    ADCON2bits.ADCS1 = 0;
    ADCON2bits.ADCS0 = 1;
    
    //Acquisition Time採樣時間是由4個Tad組成的
    ADCON2bits.ACQT2 = 0;
    ADCON2bits.ACQT1 = 1;
    ADCON2bits.ACQT0 = 0;

    ADCON2bits.ADFM = 1; // 設成right justified
    //Configure A/D interrupt(optional)
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    IPR1bits.ADIP = 1; // A/D 設成high priority
    
    //一般的priority 設定
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1; 
    
    // 設定燈泡
    
    TRISD = 0;
    LATD = 0;
    /*
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD3 = 0;
    LATDbits.LATD0 = 0;
    LATDbits.LATD1 = 0;
    LATDbits.LATD2 = 0;
    LATDbits.LATD3 = 0;    
    */
}

void ccp2_init(void){
    //Configure CCP2 mode,ref datasheet p.141
    // 以下的1011表示用 Compare mode, trigger special event; reset timer; CCP2 match starts A/D conversion (CCPxIF bit is set)
    CCP2CONbits.CCP2M3 = 1;
    CCP2CONbits.CCP2M2 = 0;
    CCP2CONbits.CCP2M1 = 1;
    CCP2CONbits.CCP2M0 = 1; 
    //configure CCP2 interrupt
    PIE2bits.CCP2IE = 1; 
    IPR2bits.CCP2IP = 1;  
    //configure CCP2 comparator value
    // 石英震盪器 = 8MHz -> 8000000/4 = 2000000
    // 2000000 / T3預除器 = 2000000/8 = 250000 但是我們的值最高只能設65535
    // 所以我就把CCPR2先設50000，然後進去ISR裡面五次後才可以真正去跑ADC的東東
    CCPR2 = 50000;
}

void timer3_init(void){
    //set up timer3, ref datasheet p135
    //no need to turn up timer3 interrupt
    T3CONbits.RD16 = 1; //開啟16bit
    T3CONbits.T3CCP2 = 1; // 1x = Timer3 is the capture/compare clock source for the CCP modules
    T3CONbits.T3CCP1 = 1;
    T3CONbits.TMR3ON =1; //打開Timer3
    T3CONbits.T3CKPS = 3; //預除器 = 1:8 
}