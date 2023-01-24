/*
 *   Basic OLED SSD1306 display driver
 */
#include "mcc_generated_files/spi/spi1.h"
#include "oled.h"
#include "logo.h"

unsigned char oled_contrast;

void OLED_Command(uint8_t temp){
    SPI1_Open(SPI1_DEFAULT);
    CS2_SetLow();
    SPI1_ByteExchange(temp);
    CS2_SetHigh();
    SPI1_Close();
}

void OLED_Data(uint8_t temp){
    OLED_DATA_SetHigh();
    OLED_Command(temp);
    OLED_DATA_SetLow();
}

uint8_t OLED_GetContrast(void)
{
    return oled_contrast;
}

void OLED_Initialize( void)
{
    RST2_SetLow();
    __delay_ms(1000);
    RST2_SetHigh();
    __delay_ms(1000);
    OLED_DATA_SetLow();
    OLED_Command(SSD1306_DISPLAYOFF);             //0xAE  Set OLED Display Off
    OLED_Command(SSD1306_SETDISPLAYCLOCKDIV);     //0xD5  Set Display Clock Divide Ratio/Oscillator Frequency
    OLED_Command(0x80);
    OLED_Command(SSD1306_SETMULTIPLEX);           //0xA8  Set Multiplex Ratio
    OLED_Command(39);
    
    OLED_Command(SSD1306_SETSEGMENTREMAP);        //0xA1  Set Segment Remap Inv
    OLED_Command(SSD1306_COMSCANDEC);             //0xC8  Set COM Output Scan Inv
    OLED_Command(SSD1306_SETSTARTLINE);           //0x40  Set Display Start Line
    
    OLED_Command(SSD1306_SETDISPLAYOFFSET);       //0xD3  Set Display Offset
    OLED_Command(0x00);
    OLED_Command(SSD1306_CHARGEPUMP);             //0x8D  Set Charge Pump
    OLED_Command(0x14);                           //0x14  Enable Charge Pump
    OLED_Command(SSD1306_SETCOMPINS);             //0xDA  Set COM Pins Hardware Configuration
    OLED_Command(0x12);
    OLED_Command(SSD1306_SETCONTRAST);            //0x81   Set Contrast Control
    oled_contrast = 0xAF;
    OLED_Command(oled_contrast);
    OLED_Command(SSD1306_SETPRECHARGE);           //0xD9   Set Pre-Charge Period
    OLED_Command(0x25);
    OLED_Command(SSD1306_SETVCOMDETECT);          //0xDB   Set VCOMH Deselect Level
    OLED_Command(0x20);
    OLED_Command(SSD1306_DISPLAYALLON_RESUME);    //0xA4   Set Entire Display On/Off
    OLED_Command(SSD1306_NORMALDISPLAY);          //0xA6   Set Normal/Inverse Display
    OLED_Command(SSD1306_DISPLAYON);              //0xAF   Set OLED Display On
} // OLED_Initialize

void OLED_SetRow(uint8_t add)
{
    add = 0xB0 | add;
    OLED_Command(add);
}

void OLED_SetColumn(uint8_t add)
{
    add += 32;
    OLED_Command((SSD1306_SETHIGHCOLUMN | (add >> 4))); // SET_HIGH_COLUMN
    OLED_Command((0x0f & add));        // SET LOW_COLUMN
}

void OLED_PutPicture(const uint8_t *pic)
{
    unsigned char i,j;
    for(i=0; i<5; i++) // 5*8=40 pixel rows (actually 39)
    {
        OLED_SetRow( i);
        OLED_SetColumn( 0);
        for(j=0; j < 96; j++)  // 96 pixel columns
        {
            OLED_Data(*pic++);
        }
    }
}

void OLED_Clear(void)
{
    unsigned char i,j;
    for(i=0; i<5; i++) // 5*8=40 pixel rows (actually 39)
    {
        OLED_SetRow(i);
        OLED_SetColumn(0);
        for(j=0; j<96; j++) OLED_Data( 0);
    }
}

void OLED_SetContrast(uint8_t temp)
{
    oled_contrast = temp;
    OLED_Command(SSD1306_SETCONTRAST);  
    OLED_Command(temp);                  // contrast step 1 to 256
}

const uint8_t font[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5F,0x00,0x00,0x00,0x07,0x00,0x07,0x00, //	'sp,!,"
    0x14,0x7F,0x14,0x7F,0x14, // #
    0x24,0x2A,0x7F,0x2A,0x12,0x23,0x13,0x08,0x64,0x62,0x36,0x49,0x56,0x20,0x50, //	'$,%,&
    0x00,0x08,0x07,0x03,0x00,0x00,0x1C,0x22,0x41,0x00,0x00,0x41,0x22,0x1C,0x00, //	'',(,)
    0x2A,0x1C,0x7F,0x1C,0x2A,0x08,0x08,0x3E,0x08,0x08,0x00,0x00,0x70,0x30,0x00, //	'*,+,,
    0x08,0x08,0x08,0x08,0x08,0x00,0x00,0x60,0x60,0x00,0x20,0x10,0x08,0x04,0x02, //	'-,.,/
    0x3E,0x51,0x49,0x45,0x3E,0x00,0x42,0x7F,0x40,0x00,0x72,0x49,0x49,0x49,0x46, //	'0,1,2
    0x21,0x41,0x49,0x4D,0x33,0x18,0x14,0x12,0x7F,0x10,0x27,0x45,0x45,0x45,0x39, //	'3,4,5
    0x3C,0x4A,0x49,0x49,0x31,0x41,0x21,0x11,0x09,0x07,0x36,0x49,0x49,0x49,0x36, //	'6,7,8
    0x46,0x49,0x49,0x29,0x1E,0x00,0x00,0x14,0x00,0x00,0x00,0x40,0x34,0x00,0x00, //	'9,:,;
    0x00,0x08,0x14,0x22,0x41,0x14,0x14,0x14,0x14,0x14,0x00,0x41,0x22,0x14,0x08, //	'<,=,>
    0x02,0x01,0x59,0x09,0x06,0x3E,0x41,0x5D,0x59,0x4E,                          //  '?,@
    0x7C,0x12,0x11,0x12,0x7C,                                                   //	'A
    0x7F,0x49,0x49,0x49,0x36,0x3E,0x41,0x41,0x41,0x22,0x7F,0x41,0x41,0x41,0x3E, //	'B,C,D
    0x7F,0x49,0x49,0x49,0x41,0x7F,0x09,0x09,0x09,0x01,0x3E,0x41,0x41,0x51,0x73, //	'E,F,G
    0x7F,0x08,0x08,0x08,0x7F,0x00,0x41,0x7F,0x41,0x00,0x20,0x40,0x41,0x3F,0x01, //	'H,I,J
    0x7F,0x08,0x14,0x22,0x41,0x7F,0x40,0x40,0x40,0x40,0x7F,0x02,0x1C,0x02,0x7F, //	'K,L,M
    0x7F,0x04,0x08,0x10,0x7F,0x3E,0x41,0x41,0x41,0x3E,0x7F,0x09,0x09,0x09,0x06, //	'N,O,P
    0x3E,0x41,0x51,0x21,0x5E,0x7F,0x09,0x19,0x29,0x46,0x26,0x49,0x49,0x49,0x32, //	'Q,R,S
    0x03,0x01,0x7F,0x01,0x03,0x3F,0x40,0x40,0x40,0x3F,0x1F,0x20,0x40,0x20,0x1F, //	'T,U,V
    0x3F,0x40,0x38,0x40,0x3F,0x63,0x14,0x08,0x14,0x63,0x03,0x04,0x78,0x04,0x03, //	'W,X,Y
    0x61,0x59,0x49,0x4D,0x43,                                                   //  'Z
    0x00,0x7F,0x41,0x41,0x41,0x02,0x04,0x08,0x10,0x20,                          //	'[,\,   
    0x00,0x41,0x41,0x41,0x7F,0x04,0x02,0x01,0x02,0x04,0x40,0x40,0x40,0x40,0x40, //	'],^,_
    0x00,0x03,0x07,0x08,0x00,0x20,0x54,0x54,0x38,0x40,0x7F,0x28,0x44,0x44,0x38, //	'`,a,b
    0x38,0x44,0x44,0x44,0x28,0x38,0x44,0x44,0x28,0x7F,0x38,0x54,0x54,0x54,0x18, //	'c,d,e
    0x00,0x08,0x7E,0x09,0x02,0x0C,0x52,0x52,0x4A,0x3C,0x7F,0x08,0x04,0x04,0x78, //	'f,g,h
    0x00,0x44,0x7D,0x40,0x00,0x20,0x40,0x40,0x3D,0x00,0x7F,0x10,0x28,0x44,0x00, //	'i,j,k
    0x00,0x41,0x7F,0x40,0x00,0x7C,0x04,0x78,0x04,0x78,0x7C,0x08,0x04,0x04,0x78, //	'l,m,n
    0x38,0x44,0x44,0x44,0x38,0x7C,0x18,0x24,0x24,0x18,0x18,0x24,0x24,0x18,0x7C, //	'o,p,q
    0x7C,0x08,0x04,0x04,0x08,0x48,0x54,0x54,0x54,0x24,0x04,0x04,0x3F,0x44,0x24, //	'r,s,t
    0x3C,0x40,0x40,0x20,0x7C,0x1C,0x20,0x40,0x20,0x1C,0x3C,0x40,0x30,0x40,0x3C, //	'u,v,w
    0x44,0x28,0x10,0x28,0x44,0x4C,0x50,0x50,0x50,0x3C,0x44,0x64,0x54,0x4C,0x44, //	'x,y,z
    0x00,0x08,0x36,0x41,0x00,0x00,0x00,0x77,0x00,0x00,0x00,0x41,0x36,0x08,0x00, //	'{,|,}
    0x02,0x01,0x02,0x04,0x02
    };

void OLED_DrawChar(uint8_t x, uint8_t y, char ch)
{
    OLED_SetRow(y);
    OLED_SetColumn(x);    
    OLED_Putchar(ch);
}

void OLED_Putchar(char ch)
{
    uint8_t i;
    const uint8_t *f = &font[(ch-' ')*5];
    if(ch == '\n' || ch == '\r') return;
    for(i=0; i<5; i++)
        OLED_Data((uint8_t)(*f++ << 1));
    OLED_Data(0x00); // 1 pixel empty column after char
}

void OLED_Puts(uint8_t x, uint8_t y, char *str)
{
    OLED_SetRow(y);
    OLED_SetColumn(x*6);    
    while(*str) {
        OLED_Putchar(*str++);
        x++;
    }
}

void OLED_PrintCuriosityLogo(void)
{
    unsigned char i;
    
    OLED_Clear();
    OLED_PutPicture(curiosity_logo);
    __delay_ms(500);
    
    for(i=0; i<2; i++)
    {
        OLED_Command(SSD1306_INVERTDISPLAY);
        __delay_ms(500);
        OLED_Command(SSD1306_NORMALDISPLAY);
        __delay_ms(500);
    }

   for(i=0xAF; i>0x00; i--)
   {
        OLED_SetContrast(i);
        __delay_ms(10);
   }
   OLED_Clear();
   OLED_SetContrast(0xAF);
}