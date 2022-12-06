 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
*/

/*
� [2022] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/
#include "mcc_generated_files/system/system.h" 

volatile bool SEND_FRAME = false;
//Include delay header if needed. 

void DS_Frame_Update(void){
    DataStreamer.count +=5; /*Update variable values */
    DataStreamer.count16 += 1000;
    DataStreamer.count32 += 50000000;
    DataStreamer.count_f += 1.5;
    SEND_FRAME = true;   /* Variable values updated, a new Data Streamer frame can be sent. */
    LED_SetLow();
}
/*
    Main application
*/

int main(void)
{
    SYSTEM_Initialize();
    
    DataStreamer.count = 0;
    DataStreamer.count16 = 0;
    DataStreamer.count32 = 0;
    DataStreamer.count_f = 1.5;
    
    while(1)
    {
        DS_Frame_Update();
        if(SEND_FRAME){
            WriteFrame();
            SEND_FRAME = false;
            LED_SetHigh();
        }
        __delay_ms(100);
    }
}