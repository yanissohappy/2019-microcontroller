#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
#include <xc.h>
// using namespace std;

char str[20];
void mode1() ;
void mode2() ;

void main(void) 
{
    SYSTEM_Initialize() ;
    
    while(1) {
        if (strcmp("mode1", GetString()) == 0) 
            mode1();
        else if (strcmp("mode2", GetString()) == 0) 
            mode2();
    }
    return;
    
}

void mode1() // print "Hello world"
{
    ClearBuffer();
    UART_Write_Text("Hello "); // ????
    UART_Write_Text("world"); // ????
}

void mode2() { // Output Voltage 
    ClearBuffer();
    int digital = 0;
    float voltage = 0.0;
    while(strcmp(GetString(), "e") != 0) 
    {
        ClearBuffer();
        digital=ADC_Read(7);
        voltage = digital* ((float)5/1023); // 5v / 2^10-1  (10bits)
        sprintf(str, "%3.2f", voltage); 
        UART_Write_Text(str); 
        _delay(10000);
        
    }    
    ClearBuffer();
}

// old version: 
// void interrupt high_priority Hi_ISR(void)
void __interrupt(high_priority) Hi_ISR(void)
{
    if(PIR1bits.CCP1IF == 1) {
        RC2 ^= 1;
        PIR1bits.CCP1IF = 0;
        TMR3 = 0;
    }
    return ;
}

