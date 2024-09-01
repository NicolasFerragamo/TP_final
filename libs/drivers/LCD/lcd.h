/**********************************************************************************************************
*                                               LCD
*						Incluye las definiciones necesarias para el manejo
*                       del display LCD
*						
*                                      <Copyright>
*
*						<Copyright or distribution terms>
*
*
*********************************************************************************************************/

/*********************************************************************************************************
*                                               <File description>
*
* Filename	: LCD
* Version	: 1.1.0				
* Programmer(s) : NEF
**********************************************************************************************************
*  Note(s):
*
*
*
*********************************************************************************************************/

/*********************************************************************************************************
 *
 * \file		LCD
 * \brief		Incluye las definiciones necesarias para el manejo
*               del display LCD
 * \date		14 de octubre del 2020
 * \author		Nicolas Ferragamo nferragamo@est.frba.utn.edu.ar
 * \version     1.1.0
*********************************************************************************************************/

/*********************************************************************************************************
 *** MODULO
*********************************************************************************************************/

#ifndef PR_LCD_H
#define PR_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************
 *** INCLUDES GLOBALES
*********************************************************************************************************/

#include "Tdatos.h"
#include <Arduino.h>


/*********************************************************************************************************
 *** DEFINES GLOBALES
*********************************************************************************************************/
/*!
 @defined 
 @abstract   All these definitions shouldn't be used unless you are writing 
 a driver.
 @discussion All these definitions are for driver implementation only and
 shouldn't be used by applications.
 */
// LCD Commands
// ---------------------------------------------------------------------------
#define LCD_CLEARDISPLAY        0x01    
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04 
#define LCD_DISPLAYCONTROL      0x08 
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80

// flags for display entry mode
// ---------------------------------------------------------------------------
#define LCD_ENTRYDECREMENT      0x00
#define LCD_ENTRYINCREMENT      0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off and cursor control
// ---------------------------------------------------------------------------
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00

// flags for display/cursor shift
// ---------------------------------------------------------------------------
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00

// flags for function set
// ---------------------------------------------------------------------------
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00


// Define COMMAND and DATA LCD Rs (used by send method).
// ---------------------------------------------------------------------------
#define COMMAND                 0
#define DATA                    1
#define FOUR_BITS               2


#define     LCD_RS_BUS_DIR      8       //!< inicialización del pin
#define     LCD_RW_BUS_DIR      7
#define     LCD_E_BUS_DIR       6
#define     LCD_D4_BUS_DIR      9 
#define     LCD_D5_BUS_DIR      10
#define     LCD_D6_BUS_DIR      11
#define     LCD_D7_BUS_DIR      12
#define     LCD_DISPLAY         PORTB	//!< define el puerto dnde esta conectado el bus

#define     LCD_RS(x) 			digitalWrite(LCD_RS_BUS_DIR, x)	
#define     LCD_RW(x) 			digitalWrite(LCD_RW_BUS_DIR, x)	
#define     LCD_E(x) 			digitalWrite(LCD_E_BUS_DIR,  x)	
#define     LCD_D4(x)           digitalWrite(LCD_D4_BUS_DIR, x)
#define     LCD_D5(x)           digitalWrite(LCD_D5_BUS_DIR, x)
#define     LCD_D6(x)           digitalWrite(LCD_D6_BUS_DIR, x)
#define     LCD_D7(x)           digitalWrite(LCD_D7_BUS_DIR, x)



#define     LCD_SIMULADOR_ON     1
#define     LCD_SIMULADOR_OFF    0

#define     LCD_SIMULADOR       LCD_SIMULADOR_ON

/*********************************************************************************************************
 *** MACROS GLOBALES
*********************************************************************************************************/

/*********************************************************************************************************
 *** TIPO DE DATOS GLOBALES

*********************************************************************************************************/

/*********************************************************************************************************
 *** VARIABLES GLOBALES
*********************************************************************************************************/

/*********************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
**********************************************************************************************************/


void LCD_Char2LCD (uint8_t caracter);
void LCD_Msg2LCD (const char* msg);  
void LCD_Clear (void);	
void LCD_RetHome (void);	
void LCD_SetCursor (uint8_t line, uint8_t pos);                     
void LCD_Desp2Izq (uint8_t cant);
void LCD_Desp2Der (uint8_t cant);
void LCD_DisplayOff (void);
void LCD_DisplayOn (void);
void LCD_BlinkOfOn (void);
void LCD_BlinkOff (void);
void LCD_CursorOn (void);
void LCD_CursorOff (void);
void LCD_MoveCursorRight(void);
void LCD_MoveCursorLeft(void);
void LCD_InitCGRAM (void);
void LCD_Init(uint8_t lines, uint8_t cursor, uint8_t dot_format);


#ifdef __cplusplus
}
#endif


#endif /* PR_LCD_H */
