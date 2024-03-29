/**
 * CRC Generated Driver File.
 * 
 * @file crc.c
 * 
 * @ingroup  crc
 * 
 * @brief This file contains the API implementation for the CRC driver.
 *
 * @version CRC Driver Version 1.0.0
*/

/*
� [2024] Microchip Technology Inc. and its subsidiaries.

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

/**
  Section: Included Files
*/

#include <xc.h>
#include "../crc.h"


static void (*CRC_ScannerInterruptHandler)(void);
static void CRC_DefaultScannerInterruptHandler(void);

void CRC_Initialize(void)
{
    CRCCON0 = 0x00;
    //CRC Configurations
    //CRCPLEN 31; 
    CRCCON1 = 0x1F;
    //CRCDLEN 15; 
    CRCCON2 = 0xF;

    // Read/Write access to CRCXOR
    CRCCON0bits.SETUP = 0b10;
    //CRCXORT 4; 
    CRCXORT = 0x04;
    //CRCXORU 193; 
    CRCXORU = 0xC1;
    //CRCXORH 29; 
    CRCXORH = 0x1D;
    //CRCXORL 183; 
    CRCXORL = 0xB7;

    // Read/Write access to CRCOUT
    CRCCON0bits.SETUP = 0b00;
    //CRCOUTT 255; 
    CRCOUTT = 0xFF;
    //CRCOUTU 255; 
    CRCOUTU = 0xFF;
    //CRCOUTH 255; 
    CRCOUTH = 0xFF;
    //CRCOUTL 255; 
    CRCOUTL = 0xFF;

    //CRCDATAT 0; 
    CRCDATAT = 0x0;
    //CRCDATAU 0; 
    CRCDATAU = 0x0;
    //CRCDATAH 0; 
    CRCDATAH = 0x0;
    //CRCDATAL 0; 
    CRCDATAL = 0x0;

    //Scanner Configurations
    //
    SCANHADRH = 0xFF;
    //
    SCANHADRL = 0xFF;
    //
    SCANLADRH = 0x0;
    //
    SCANLADRL = 0x0;
    //SCANTSEL LFINTOSC; 
    SCANTRIG = 0x0;

    // Clearing CRC IF flag
    PIR1bits.CRCIF = 0;
    // Disabled CRCI CRC interrupt
    PIE1bits.CRCIE = 0;

    // Clearing Scanner IF flag before enabling the interrupt.
    PIR1bits.SCANIF = 0;
    // Set Default Interrupt Handler
    CRC_SetScannerInterruptHandler(CRC_DefaultScannerInterruptHandler);
    // Enabled SCANI CRC interrupt
    PIE1bits.SCANIE = 1;

    //CRCEN enabled; CRCGO disabled; CRCACCM data augmented with 0s; CRCSETUP CRC Register Overlay Selection; Read / Write access to CRCOUT; CRCSHIFTM shift right; 
    CRCCON0 = 0x92;
    //SCANEN enabled; SCANGO disabled; SCANINTM not affected; SCANMODE Burst mode; 
    SCANCON0 = 0x91;
}

inline void CRC_StartCrc(void)
{
    // Start the serial shifter
    CRCCON0bits.CRCGO = 1;
}

bool CRC_WriteData(uint32_t data)
{
    if(!CRCCON0bits.FULL)
    {
        CRCDATAT = (uint8_t)((data >> 24) & 0xFF);
        CRCDATAU = (uint8_t)((data >> 16) & 0xFF);
        CRCDATAH = (uint8_t)((data >> 8) & 0xFF);
        CRCDATAL = (uint8_t)(data & 0xFF);
        return true;
    } 
    else 
    {
        return false;
    }
}

static uint32_t CRC_ReverseValue(uint32_t crc)
{
    uint32_t mask;
    uint32_t reverse;

    mask = 1;
    mask <<= CRCCON1bits.PLEN;
    reverse = 0;

    while(crc)
    {
        if(crc & 0x01)
        {
            reverse |= mask;
        }
        mask >>= 1;
        crc >>= 1;
    }
    return reverse;
}

uint32_t CRC_GetCalculatedResult(bool reverse, uint32_t xorValue)
{
    uint32_t result = 0x00;
    // Read/Write access to CRCOUT
    CRCCON0bits.SETUP = 0b00;
    result = (uint32_t)CRCOUTL;
    result = result | ((uint32_t)CRCOUTH << 8);
    result = result | ((uint32_t)CRCOUTU << 16);
    result = result | ((uint32_t)CRCOUTT << 24);
    if(reverse)
    {
        result = CRC_ReverseValue(result);
    }
    result ^= xorValue;
    return (result & 0xFFFFFFFF);
}

inline bool CRC_IsCrcBusy(void)
{
    return(CRCCON0bits.CRCBUSY);
}

inline void CRC_StartScanner(void)
{
    // Start the serial shifter
    CRCCON0bits.CRCGO = 1;
    // Start the scanner
    SCANCON0bits.SGO = 1;
}

inline void CRC_StopScanner(void)
{
    // Stop the serial shifter
    CRCCON0bits.CRCGO = 0;
    // Stop the scanner
    SCANCON0bits.SGO = 0;
}

void CRC_SetScannerAddressLimit(uint16_t startAddr, uint16_t endAddr)
{
    SCANHADRH = (uint8_t)((endAddr >> 8) & 0xFF);
    SCANHADRL = (uint8_t)(endAddr & 0xFF);
    SCANLADRH = (uint8_t)((startAddr >> 8) & 0xFF);
    SCANLADRL = (uint8_t)(startAddr & 0xFF);
}

inline bool CRC_IsScannerBusy(void)
{
    return(SCANCON0bits.BUSY);
}


void CRC_SCANNER_ISR(void)
{
    if(PIR1bits.SCANIF == 1){
        if(CRC_ScannerInterruptHandler){
            CRC_ScannerInterruptHandler();
        }
        PIR1bits.SCANIF= 0;
    }
}

void CRC_SetScannerInterruptHandler(void (* InterruptHandler)(void)){
    CRC_ScannerInterruptHandler = InterruptHandler;
}

static void CRC_DefaultScannerInterruptHandler(void){
    // add your CRC interrupt custom code
}
