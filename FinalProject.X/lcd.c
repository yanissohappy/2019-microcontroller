/*
 * File:   lcd.c
 * Author: yan
 *
 * Created on 2019年12月23日, 下午 9:28
 */

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
#include <pic18f4520.h>             /* Include PIC18F4520 header file */
#define RS LATD0                    /* PORT 0 pin is used for Register Select */
#define EN LATD1                    /* PORT 1 pin is used for Enable */
#define LCD_Data LATB               /* PORT is used for transmitting data to LCD */
#define LCD_Port TRISB          /* LCD data PORT direction */
#define LCD_Control TRISD       /* LCD command PORT direction */
//#define F_CPU 8000000/64            /* Define ferquency */
//#define BAUDRATE (((float)(F_CPU)/(float)baud_rate)-1)/* Define Baud value */

//void USART_Init(long);              /* USART Initialization function */
//void USART_TxChar(char);            /* USART character transmit function */
//char USART_RxChar();                /* USART character receive function */
//void USART_SendString(const char *);/* USART String transmit function */
void LCD_Init();                    /* LCD initialization function */
void LCD_Command(char );            /* LCD command function */
void LCD_Clear();
void LCD_Char(char x);              /* LCD character data function */
void LCD_String(const char*);       /* LCD string function */
void MSdelay(unsigned int );        /* millisecond delay function */
void LCD_String_xy(char, char, const char*);/* LCD string function with Row and position option */

void main(void) {
    OSCCON=0x72;                                /* set internal clock to 8MHz */
//	USART_Init(9600);							/* Initiate USART with 9600 baud rate */
    MSdelay(40);   
	LCD_Init();		

	LCD_String_xy(1, 0, "NCKU");  	/* Print initial test message */
	LCD_String_xy(2, 0, "group 29!!");                   
//	LCD_Clear();
//    INTCONbits.GIE=1;                           /* enable Global Interrupt */
//    INTCONbits.PEIE=1;                          /* enable Peripheral Interrupt */
//    PIE1bits.RCIE=1;                            /* enable Receive Interrupt */	    
    while(1);
}

void LCD_Init()
{
   	LCD_Port = 0;       /* Set LCD data PORT direction as output PORT */
    LCD_Control = 0;    /* Set LCD command PORT direction as output PORT */
    MSdelay(20);            /* LCD power on initialization delay must >15mSec */
	LCD_Command(0x38);      /* Initialize with 2 line and 5*7 matrix of LCD */
    LCD_Command(0x01);      /* Clear display screen */
    MSdelay(2);
    LCD_Command(0x0c);      /* Display on cursor off */
    LCD_Command(0x06);      /* Increment cursor (shift cursor to right) */
    LCD_Command(0x80);      /* Set cursor to first row 0th position */
}

void LCD_Clear()
{
    LCD_Command(0x01);      /* Clear display screen */
    MSdelay(2);
    LCD_Command(0x80);      /* Set cursor to first row 0th position */
    MSdelay(1);
}

void LCD_Command(char Command )
{
	LCD_Data = Command;     /* Send data to PORT as a command for LCD */   
	RS = 0;                 /* Command Register is selected */
	EN = 1;                 /* High-to-Low pulse on Enable pin to latch data */ 
//	NOP();
    MSdelay(40);
	EN = 0;
    MSdelay(1);
}

void LCD_Char(char Data)
{
	LCD_Data = Data;        /* Send data to LCD */  
	RS = 1;                 /* Data Register is selected */
	EN=1;                   /* High-to-Low pulse on Enable pin to latch data */   
//	NOP();
    MSdelay(40);
	EN=0;
    MSdelay(1);
}

void LCD_String(const char *str)
{
	while((*str)!=0)
	{		
	  LCD_Char(*str);
	  str++;	
    }
}

void LCD_String_xy(char row,char pos,const char *msg)
{
    char location=0;
    if(row<=1)
    {
        location=(0x80) | ((pos) & 0x0f); /*Print message on 1st row and desired location*/
        LCD_Command(location);
    }
    else
    {
        location=(0xC0) | ((pos) & 0x0f); /*Print message on 2nd row and desired location*/
        LCD_Command(location);    
    }  
    LCD_String(msg);

}
/*********************************Delay Function********************************/
void MSdelay(unsigned int val)
{
     unsigned int i,j;
        for(i=0;i<=val;i++)
            for(j=0;j<165;j++);      /*This count Provide delay of 1 ms for 8MHz Frequency */
}
