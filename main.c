/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16LF1619
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#define grab_cnt 10
uint8_t getnext(void);

char buf[grab_cnt];
uint8_t grab_flag;
char*  anchor = "GPGGA, ";
char* dollar_sign = "$";
uint8_t flag_tmr0;
uint8_t toggle, rxData,  match_code, prev_rxData;
uint16_t ctr;

void delay(uint8_t ticks){
   
    for (uint8_t i=0;i<ticks;i++){
// Disable TMR0 interrupt
    INTCONbits.TMR0IE = 0;
    // load the timer
    TMR0=0;
    flag_tmr0=1;
// enable interrupt
    INTCONbits.TMR0IE = 1;
    // wait for interrupt to reset flag 
    while(flag_tmr0==1);
    
    }
    // Disable TMR0 interrupt
    INTCONbits.TMR0IE = 0;
}
// parse the input for the time and take action 
void parse(void){
    uint8_t i, c;
    for (i=0; i<3; i++)
    { getnext();}
    // get the fourth one
    c=getnext();
    RB6 = c & 1;
    RC6 = c&1;
}
// Get next character
uint8_t getnext(void)
{// return next character
    while(!(EUSART_is_rx_ready()))
    ;
    rxData=EUSART_Read();
    return rxData;
}
// Match a string of length N
// returns 1 for no match, 0 for match
uint8_t match(char *strg, uint8_t length){
    uint8_t flag, index,retcode;
    flag=1;
    index=0;
    retcode=0; // 0 is good match, 1 is fail
    
    while ((length > 0) & (flag==1)){
        uint8_t next;
        next=getnext();
        if( (next) != (*strg++))  {flag=0; retcode=1;} /* exit on no match */
        length--;
    }
   return retcode; }

void grab(void){
   // get next X chars one time 
    uint8_t i;
    if (grab_flag==1){
    grab_flag=0;
    for (i=0; i<grab_cnt; i++)
    {buf[i] = getnext();}
}
}
/* 
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    grab_flag=1;
    toggle=1;
    EUSART_Initialize();
    RB7 = 0;
    while (1)
    {
        
     
       // RC7 = (toggle & 0x1); RC3 = !(toggle & 0x1); toggle++;
        // search for $ first, then anchor string
        match_code = match(dollar_sign, 1);
        if(match_code==0)
        {    match_code=match(anchor,6);
            if(match_code==0) // found the anchor string
                {grab(); parse();
                RC7 = (toggle & 0x1);
                RC3 = !(toggle & 0x1);
                toggle++;
                // ctr++; if (ctr==100){RB6 = 1; ctr=0;} else RB6 = 0; 
            }
        }
        
        
       
        }
    }

    /**
 End of File
*/