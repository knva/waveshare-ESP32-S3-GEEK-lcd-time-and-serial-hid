/*****************************************************************************
* | File        :   LCD_Driver.c
* | Author      :   Waveshare team
* | Function    :   Electronic paper driver
* | Info        :
*----------------
* | This version:   V1.0
* | Date        :   2018-11-18
* | Info        :   
#
# Permission is hereby granted, free of UBYTEge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "LCD_Driver.h"

/*******************************************************************************
function:
  Hardware reset
*******************************************************************************/
static void LCD_Reset(void)
{
  DEV_Digital_Write(DEV_CS_PIN,0);
  DEV_Delay_ms(20);
  DEV_Digital_Write(DEV_RST_PIN,0);
  DEV_Delay_ms(20);
  DEV_Digital_Write(DEV_RST_PIN,1);
  DEV_Delay_ms(20);
}

/*******************************************************************************
function:
  Setting backlight
parameter :
    value : Range 0~255   Duty cycle is value/255
*******************************************************************************/
void LCD_SetBacklight(UWORD Value)
{
  DEV_Set_BL(DEV_BL_PIN, Value);
}

/*******************************************************************************
function:
    Write register address and data
*******************************************************************************/
void LCD_WriteData_Byte(UBYTE da) 
{ 
  DEV_Digital_Write(DEV_CS_PIN,0);
  DEV_Digital_Write(DEV_DC_PIN,1);
  DEV_SPI_WRITE(da);  
  DEV_Digital_Write(DEV_CS_PIN,1);
}  

 void LCD_WriteData_Word(UWORD da)
{
  UBYTE i=(da>>8)&0xff;
  DEV_Digital_Write(DEV_CS_PIN,0);
  DEV_Digital_Write(DEV_DC_PIN,1);
  DEV_SPI_WRITE(i);
  DEV_SPI_WRITE(da);
  DEV_Digital_Write(DEV_CS_PIN,1);
}   

void LCD_WriteReg(UBYTE da)  
{ 
  DEV_Digital_Write(DEV_CS_PIN,0);
  DEV_Digital_Write(DEV_DC_PIN,0);
  DEV_SPI_WRITE(da);
  //DEV_Digital_Write(DEV_CS_PIN,1);
}

/******************************************************************************
function: 
    Common register initialization
******************************************************************************/
void LCD_Init(void)
{
  LCD_Reset();

   LCD_WriteReg(0x36); 
  LCD_WriteData_Byte(0x70);

  LCD_WriteReg(0x3A); 
  LCD_WriteData_Byte(0x05);

  LCD_WriteReg(0xB2);
  LCD_WriteData_Byte(0x0C);
  LCD_WriteData_Byte(0x0C);
  LCD_WriteData_Byte(0x00);
  LCD_WriteData_Byte(0x33);
  LCD_WriteData_Byte(0x33);

  LCD_WriteReg(0xB7); 
  LCD_WriteData_Byte(0x35);  

  LCD_WriteReg(0xBB);
  LCD_WriteData_Byte(0x19);

  LCD_WriteReg(0xC0);
  LCD_WriteData_Byte(0x2C);

  LCD_WriteReg(0xC2);
  LCD_WriteData_Byte(0x01);

  LCD_WriteReg(0xC3);
  LCD_WriteData_Byte(0x12);   

  LCD_WriteReg(0xC4);
  LCD_WriteData_Byte(0x20);  

  LCD_WriteReg(0xC6); 
  LCD_WriteData_Byte(0x0F);    

  LCD_WriteReg(0xD0); 
  LCD_WriteData_Byte(0xA4);
  LCD_WriteData_Byte(0xA1);

  LCD_WriteReg(0xE0);
  LCD_WriteData_Byte(0xD0);
  LCD_WriteData_Byte(0x04);
  LCD_WriteData_Byte(0x0D);
  LCD_WriteData_Byte(0x11);
  LCD_WriteData_Byte(0x13);
  LCD_WriteData_Byte(0x2B);
  LCD_WriteData_Byte(0x3F);
  LCD_WriteData_Byte(0x54);
  LCD_WriteData_Byte(0x4C);
  LCD_WriteData_Byte(0x18);
  LCD_WriteData_Byte(0x0D);
  LCD_WriteData_Byte(0x0B);
  LCD_WriteData_Byte(0x1F);
  LCD_WriteData_Byte(0x23);

  LCD_WriteReg(0xE1);
  LCD_WriteData_Byte(0xD0);
  LCD_WriteData_Byte(0x04);
  LCD_WriteData_Byte(0x0C);
  LCD_WriteData_Byte(0x11);
  LCD_WriteData_Byte(0x13);
  LCD_WriteData_Byte(0x2C);
  LCD_WriteData_Byte(0x3F);
  LCD_WriteData_Byte(0x44);
  LCD_WriteData_Byte(0x51);
  LCD_WriteData_Byte(0x2F);
  LCD_WriteData_Byte(0x1F);
  LCD_WriteData_Byte(0x1F);
  LCD_WriteData_Byte(0x20);
  LCD_WriteData_Byte(0x23);

  LCD_WriteReg(0x21); 

  LCD_WriteReg(0x11); 

  LCD_WriteReg(0x29);
} 

/******************************************************************************
function: Set the cursor position
parameter :
    Xstart:   Start UWORD x coordinate
    Ystart:   Start UWORD y coordinate
    Xend  :   End UWORD coordinates
    Yend  :   End UWORD coordinatesen
******************************************************************************/
void LCD_SetCursor(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD  Yend)
{ 
  LCD_WriteReg(0x2a);
  LCD_WriteData_Word(Xstart	+52);
  LCD_WriteData_Word(Xend	+52);
  LCD_WriteReg(0x2b);
  LCD_WriteData_Word(Ystart	+40);
  LCD_WriteData_Word(Yend	+40);

  LCD_WriteReg(0x2C);
}

/******************************************************************************
function: Clear screen function, refresh the screen to a certain color
parameter :
    Color :   The color you want to clear all the screen
******************************************************************************/
void LCD_Clear(UWORD Color)
{
  UWORD i,j;    
  LCD_SetCursor(0,0,LCD_WIDTH-1,LCD_HEIGHT-1);

  DEV_Digital_Write(DEV_DC_PIN,1);
  for(i = 0; i < LCD_WIDTH; i++){
    for(j = 0; j < LCD_HEIGHT; j++){
      LCD_WriteData_Word((Color>>8)&0xff);
      LCD_WriteData_Word(Color);
    }
   }
}

/******************************************************************************
function: Refresh a certain area to the same color
parameter :
    Xstart:   Start UWORD x coordinate
    Ystart:   Start UWORD y coordinate
    Xend  :   End UWORD coordinates
    Yend  :   End UWORD coordinates
    color :   Set the color
******************************************************************************/
void LCD_ClearWindow(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend,UWORD color)
{          
  UWORD i,j; 
  LCD_SetCursor(Xstart, Ystart, Xend-1,Yend-1);
  for(i = Ystart; i <= Yend-1; i++){                                
    for(j = Xstart; j <= Xend-1; j++){
      LCD_WriteData_Word(color);
    }
  }                   
}

/******************************************************************************
function: Set the color of an area
parameter :
    Xstart:   Start UWORD x coordinate
    Ystart:   Start UWORD y coordinate
    Xend  :   End UWORD coordinates
    Yend  :   End UWORD coordinates
    Color :   Set the color
******************************************************************************/
void LCD_SetWindowColor(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend,UWORD  Color)
{
  LCD_SetCursor( Xstart,Ystart,Xend,Yend);
  LCD_WriteData_Word(Color);      
}

/******************************************************************************
function: Draw a UWORD
parameter :
    X     :   Set the X coordinate
    Y     :   Set the Y coordinate
    Color :   Set the color
******************************************************************************/
void LCD_SetUWORD(UWORD x, UWORD y, UWORD Color)
{
  LCD_SetCursor(x,y,x,y);
  LCD_WriteData_Word(Color);      
} 
