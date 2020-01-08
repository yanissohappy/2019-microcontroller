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
int light_incr = 0;
int light_decr = 0;
int temp_for_DC1B = 0;
int switch_A = 1; 
int temp_for_DC2B = 0;
int big_price = 0;
int light_twenty_times = 0;


void main(void) {
    LED_Init();
    while(1);
}

void LED_Init() 
{
    TRISCbits.TRISC2 = 0; //RC2 port = output (CCP1)
    TRISCbits.TRISC1 = 0; //RC1 port = output (CCP2)
    
    T2CON = 0x07; //TMR2ON = 1, T2CKPS<1:0> = 11 (1:16 prescaler)
    TMR2 = 0;
    
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    
    PR2 = 30; 
    CCPR1L = 0; 
    CCP1CONbits.DC1B = 0;
    
    CCP1CONbits.CCP1M3 = 1; //set PWM mode
    CCP1CONbits.CCP1M2 = 1;
    CCP1CONbits.CCP1M1 = 0;
    CCP1CONbits.CCP1M0 = 0;

    
    T1CONbits.RD16 = 1; 
    T1CONbits.TMR1ON = 1;
    T1CONbits.TMR1CS = 0; //Clock source = internal clock
    
    TMR1 = 50000;
    PIE1bits.TMR1IE = 1;
    IPR1bits.TMR1IP = 1; 
    PIR1bits.TMR1IF = 0;
    
    light_incr = 1;
    light_decr = 0;
    
    temp_for_DC1B = 0;
    CCPR2L = 5;
    temp_for_DC2B = CCPR2L << 2;
    
    CCP2CONbits.DC2B = 0;
    CCP2CONbits.CCP2M3 = 1; //set PWM mode
    CCP2CONbits.CCP2M2 = 1;
    CCP2CONbits.CCP2M1 = 0;
    CCP2CONbits.CCP2M0 = 0;  

    //按鈕INT0 這個是真正大獎掉下去壓到的按鈕 //左邊那顆按鈕
    INTCONbits.INT0IE = 1;
    INTCON2bits.INTEDG0 = 0; //;1->0時觸發(也就是按下按鈕時!!!!)        
    
    //開啟int 1 這個是給手動按按鈕開啟掉錢的按鈕 //右邊那顆按鈕
     INTCON3bits.INT1IE = 1;
     INTCON2bits.INTEDG1 = 0;  //;1->0時觸發(也就是按下按鈕時!!!!)  
    
    while(1);
}

void __interrupt(high_priority) Hi_ISR(void)
{
    if(INTCON3bits.INT1IF){
        INTCON3bits.INT1IE = 0;
        INTCONbits.INT0IF = 0;
        INTCON3bits.INT1IF = 0;
 
        //轉馬達
        while(1){
            _delay(10000);
            temp_for_DC2B++;
            CCPR2L =  temp_for_DC2B >> 2;
            CCP2CONbits.DC2B =  temp_for_DC2B & 0b00000011;

            if (CCPR2L >= 17 ){
                _delay(100000);
                break;
            }                        
        }

        //+90 -> -90
        while(1){ 
            _delay(1000);
            temp_for_DC2B--;
            CCPR2L =  temp_for_DC2B >> 2;
            CCP2CONbits.DC2B =  temp_for_DC2B & 0b00000011;

            if(CCPR2L <= 5){
                _delay(100000);
                break;
            }            
        }           
        INTCON3bits.INT1IE = 1;       
    }
    
    if(INTCONbits.INT0IF){
        INTCONbits.INT0IF = 0;
        INTCONbits.INT0IE = 0;
//        CCP1CONbits.CCP1M = 0; //關掉CCP1模組
//        TRISDbits.TRISD0 = 0; //RD0要當輸出亮燈
//        PORTDbits.RD0 = 1; 
     
        // CCP2
        CCP2CONbits.DC2B = 0;

        CCP2CONbits.CCP2M3 = 1; //set PWM mode
        CCP2CONbits.CCP2M2 = 1;
        CCP2CONbits.CCP2M1 = 0;
        CCP2CONbits.CCP2M0 = 0;   
        CCPR2L = 11;       
        
        big_price = 1;

        if(CCP2CONbits.CCP2M3 > 0){ //轉馬達
            while(1){
                _delay(10000);
                temp_for_DC2B++;
                CCPR2L =  temp_for_DC2B >> 2;
                CCP2CONbits.DC2B =  temp_for_DC2B & 0b00000011;
                
                if (CCPR2L >= 16 ){
                    _delay(100000);
                    break;
                }                        
            }

            while(1){ 
                _delay(1000);
                temp_for_DC2B--;
                CCPR2L =  temp_for_DC2B >> 2;
                CCP2CONbits.DC2B =  temp_for_DC2B & 0b00000011;

                if(CCPR2L <= 5){
                    _delay(100000);
                    break;
                }            
            }           
        }     
        INTCONbits.INT0IE = 1;
    }
    
    if(TMR1IF && (CCP1CONbits.CCP1M > 0) && (!INTCONbits.INT0IF) && !big_price)
    {
        TMR1IF = 0;
        INTCONbits.INT0IF = 0;
        if(light_incr == 1 && light_decr ==0)
        {            
            if(temp_for_DC1B >= 30) {
                light_incr = 0;
                light_decr = 1;
                return;
            }
            temp_for_DC1B += 2;
            CCPR1L = temp_for_DC1B >> 2;
//            temp_for_DC1B = temp_for_DC1B & 0x11;
            CCP1CONbits.DC1B = temp_for_DC1B  & 0b00000011;
            //_delay(100);
        }
        else if(light_incr == 0 && light_decr ==1)
        {            
            if(temp_for_DC1B <= 1) {
                light_incr = 1;
                light_decr = 0;
                return;
            }        
            temp_for_DC1B -= 2;
            CCPR1L = temp_for_DC1B >> 2;
//            temp_for_DC1B = temp_for_DC1B & 0x11;
            CCP1CONbits.DC1B = temp_for_DC1B & 0b00000011;            
            //_delay(100);
        }
    }else{
        TMR1IF = 0;
        INTCONbits.INT0IF = 0;
        if(switch_A == 0){
            CCPR1L = 10;
            _delay(20000);
            switch_A = 1;  
        }
        if(switch_A == 1){
            CCPR1L = 0;
            _delay(20000);
            switch_A = 0;      
        }
        light_twenty_times++;
        if(light_twenty_times == 20) {
            big_price = 0;
        }
    }
    


    
//    //////熱烈歡迎大獎來臨!!!!//////
//    if(PIR1bits.TMR1IF == 1 && (CCP1CONbits.CCP1M <= 1)){
//        //CCP1CONbits.CCP1M = 0; //關掉CCP模組
//        INTCONbits.INT0IE = 0;
//        INTCONbits.INT0IF = 0;
//        PIR1bits.TMR1IF = 0;
//        PORTDbits.RD0 = 1; 
//        if(switch_A == 0){
//        //if(TMR1IF && switch_A == 0){
////            TRISDbits.TRISD0 = 0;
//            PORTDbits.RD0 = 0; 
//            _delay(20000);
//            switch_A = 1;  
//        }
//        if(switch_A == 1){
//        //if(TMR1IF && switch_A == 1){
////            TRISDbits.TRISD0 = 0;
//            PORTDbits.RD0 = 1; 
//            _delay(20000);
//            switch_A = 0;      
//        }
//        INTCONbits.INT0IE = 1;
//    }

    return ;
}