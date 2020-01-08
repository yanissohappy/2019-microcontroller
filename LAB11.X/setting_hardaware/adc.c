#include <xc.h>

void ADC_Initialize(void) {
//    TRISA = 0xff;		// Set as input port
//    ADCON1 = 0x0e;  	// Ref vtg is VDD & Configure pin as analog pin 
//    // ADCON2 = 0x92;  	
//    ADFM = 1 ;          // Right Justifie
//    ADCON2bits.ADCS = 7; // 
//    ADRESH=0;  			// Flush ADC output Register
//    ADRESL=0;  
    
    //////////////////////////////////////////////////////////////////////////////////////////
    ADCON1bits.VCFG1 = 0; //setting vref- = Vss
    ADCON1bits.VCFG0 = 0; //setting vref+ = Vdd
    ADCON1bits.PCFG = 0; //setting A/D port configuration control(AN0~AN12 = analog)
    ADCON0bits.CHS = 7; //setting input channel = AN7
    TRISEbits.TRISE2 = 1; //setting RE2/AN7 port as input
    //TRISE =  0x02   ;
    ADCON2bits.ADFM = 1; //setting RIGHT justified

    //ADCON2bits.ADCS = 3; //conversion time Frc (Tad = 1.2 us) //USE THIS IF FOSC = 1MHz
    ADCON2bits.ADCS = 4; //conversion time 4Tosc (Tad = 0.25x4 = 1 us) //USE THIS IF FOSC = 4MHz
    //ADCON2bits.ACQT = 1; //setting acquisition time ACQT<2:0> 2Tad = 2.4 us (Tacqt should be >=2.4us) //USE THIS IF FOSC = 1MHz
    ADCON2bits.ACQT = 5; //setting acquisition time ACQT<2:0> 12Tad = 3 us (Tacqt should be >=2.4us) //USE THIS IF FOSC = 4MHz
    
    ADCON0bits.ADON = 1; // enable AD module

    PIR1bits.ADIF = 0;//clear ADIF
    PIE1bits.ADIE = 1; //set ADIE
    INTCONbits.GIE = 1;//set GIE
    IPEN = 1;
    ADIP = 1;
}

int ADC_Read(int channel)
{
    int digital;
    
    ADCON0bits.CHS = channel ; // Select Channel
    ADCON0bits.GO = 1;
    ADCON0bits.ADON = 1;
    
    while(ADCON0bits.GO_nDONE==1);

    digital = (ADRESH*256) | (ADRESL);
    return(digital);
}