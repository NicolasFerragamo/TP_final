/*********************************************************************************************************
 *                                               LCD
 *                               Manejo del display LCD
 *
 *                                   <Copyright>
 *
 *                               <Copyright or distribution terms>
 *
 *
 *********************************************************************************************************/

/*********************************************************************************************************
 *                                               <File description>
 *
 * Filename	: PR_LCD
 * Version	: 1.1.0
 * Programmer(s) : NEF
 **********************************************************************************************************
 *  Note(s): Esta libreria solo puede utilizar las cuatro l�neas menos
 *significativas de un puerto en caso de querer utilizar otras l�neas se debe de
 *reprogramar. Es necesaro este habilitada la interrupcion de timer0 en la cual
 *se debe de llamar a la funcion LCD_tic() la misma establece las demoras que de
 *otra manera habra que implementarlas como una funcion aparte. No olvide
 *declarar la variable extern uint8_t LCD_Tout en el archivo FW__Interrupt.c
 *
 *********************************************************************************************************/

/*********************************************************************************************************
 *
 * \file		PR_LCD
 * \brief		Archivo con la funci�n para el manejo del Display LCD
 * \date		14 de octubre del 2020
 * \author		Nicolas Ferragamo nferragamo@est.frba.utn.edu.ar
 * \version     1.1.0
 *********************************************************************************************************/

/*********************************************************************************************************
 *** INCLUDES
 *********************************************************************************************************/

#include "lcd.h"
#include "mbed.h"
#include <stdio.h>

/*********************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 *********************************************************************************************************/

//#define     LCD_ENTRADA     0xF0            //!< Esto es para invertir el bus
// de datos y poder #define     LCD_SALIDA      0x0F            //!<leer cuando
// necesito ver si estoy busy.. #define     LCD_BUSY        LCD_D7_BUS_DIR  //!<
// Con estos defines me abstraigo del hardware #define     LCD_BUS_DIR     DDRB


#define LCD_FALSE 0
#define LCD_TRUE  (!LCD_FALSE)

/*********************************************************************************************************
 *** MACROS PRIVADAS AL MODULO
 *********************************************************************************************************/

/*********************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO
 *********************************************************************************************************/

/*********************************************************************************************************
 *** TABLAS PRIVADAS AL MODULO
 *********************************************************************************************************/
/***
 * \var		LCD_CharactersCGRAM[ ]
 * \brief    Tabla que contine los caracteres de los usuarios
 * \details  Tabla que contine los caracteres de los usuarios, para usarla
 * complete los valores de cada elemento de la tabla con sus caraceres
 */

static const uint8_t LCD_CharactersCGRAM[] = {
    0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x00, //!< caracter 0x00 '|     ' una barra
    0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x00, //!< caracter 0x01 '||    ' dos barras
    0x1C, 0x1C, 0x1C, 0x1C,
    0x1C, 0x1C, 0x1C, 0x00, //!< caracter 0x02 '|||   ' tes barras
    0x1E, 0x1E, 0x1E, 0x1E,
    0x1E, 0x1E, 0x1E, 0x00, //!< caracter 0x03 '||||  ' cuatro barras
    0x1F, 0x1F, 0x1F, 0x1F,
    0x1F, 0x1F, 0x1F, 0x00, //!< caracter 0x04 '||||| ' cinco barras
    0x00, 0x04, 0x02, 0x1F,
    0x02, 0x04, 0x00, 0x00, //!< caracter 0x05 '->    ' flechita
    0x00, 0x04, 0x08, 0x1F,
    0x18, 0x14, 0x10, 0x00, //!< caracter 0x06 '<-    ' flechita
    0x0E, 0x0E, 0x0F, 0x1F,
    0x04, 0x0A, 0x11, 0x00 //!< caracter 0x07 '      ' muniequito

};

/*********************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 *********************************************************************************************************/

// volatile uint8_t LCD_Tout = 0;

/*********************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 *********************************************************************************************************/

uint8_t bits_mode = LCD_8BITMODE; //!< identifica si se trabaja en 8 bits o 4 bits

/*********************************************************************************************************
 *** Objetos GLOBALES PRIVADAS AL MODULO
 *********************************************************************************************************/
DigitalOut LCD_RS(D6);
DigitalOut LCD_RW(D7);
DigitalOut LCD_E(D8);

DigitalOut LCD_D0(D14);
DigitalOut LCD_D1(D14);
DigitalOut LCD_D2(D14);
DigitalOut LCD_D3(D14);
DigitalOut LCD_D4(D9);
DigitalOut LCD_D5(D10);
DigitalOut LCD_D6(D11);
DigitalOut LCD_D7(D12);

/*********************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO

*********************************************************************************************************/
void LCD_Init(uint8_t lines, uint8_t cursor, uint8_t dot_format);
void LCD_InitCGRAM(void);
static void LCD_ReadBusy(void);
static void LCD_WriteCMD(uint8_t comando);
static void LCD_Write_8bits(uint8_t dato);
static void LCD_Write_4bits(uint8_t dato);
static void LCD_WriteData(uint8_t dato); // Pone se�ales para escribir dato
static void LCD_TicLCD(void);

/*********************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 *********************************************************************************************************/
/**
 *	\fn         void LCD_Read_Busy(void)
 *	\brief      Se encarga de leer el bit busy
 *  \details    La funci�n se asegura que el LCD haya terminado la operaci�n
 *ctual \author     Nicolás Ferragamo \date 		14 de octubre del 2020
 */

static void LCD_ReadBusy(void) //! FALTA MODIFICAR
{
  wait_us(50);
}

/**
 *	\fn         void LCD_Write_CMD(uint8_t comando)
 *	\brief      Se encarga de escribir un comando en el LCD
 *	\author     Nicolás Ferragamo
 *	\date 		14 de octubre del 2020
 *  \param      [in]  comando recive el comando a enviar al LCD
 */
static void LCD_WriteCMD(uint8_t comando) {
  LCD_RS = LCD_FALSE;
  LCD_RW = LCD_FALSE;
  if (bits_mode == LCD_8BITMODE) {
    LCD_Write_8bits(comando); // Env�a efectivamente el comando
  } else if (bits_mode == LCD_4BITMODE) {
    LCD_Write_4bits(comando); // Env�a efectivamente el comando
  }
}

/**
 *	\fn         void LCD_Write_4bits(unsigned char dato)
 *	\brief      Se encarga de escribir un dato en bus de a un nibble por vez
 *  \details    Se encarga de escribir un dato en bus de a un nibble por vez
 *              para poder trabajar en 4 bits.
 *	\author     Nicolás Ferragamo
 *	\date 		14 de octubre del 2020
 *  \param      [in]  dato recive dato a enviar al LCD
 */
static void LCD_Write_4bits(uint8_t dato) {
  LCD_E = LCD_TRUE;
  LCD_D4 = ((dato >> 4) & 0x01);
  LCD_D5 = ((dato >> 5) & 0x01);
  LCD_D6 = ((dato >> 6) & 0x01);
  LCD_D7 = ((dato >> 7) & 0x01);
  wait_ns(200);
  LCD_E = (LCD_FALSE);
  wait_ns(50);
  LCD_E = (LCD_TRUE);
  LCD_D4 = ((dato)&0x01);
  LCD_D5 = ((dato >> 1) & 0x01);
  LCD_D6 = ((dato >> 2) & 0x01);
  LCD_D7 = ((dato >> 3) & 0x01);
  wait_ns(200);
  LCD_E = (LCD_FALSE);
}

/**
 *	\fn         void LCD_Write_4bits(unsigned char dato)
 *	\brief      Se encarga de escribir un dato en bus de a un nibble por vez
 *  \details    Se encarga de escribir un dato en bus de a un nibble por vez
 *              para poder trabajar en 4 bits.
 *	\author     Nicolás Ferragamo
 *	\date 		14 de octubre del 2020
 *  \param      [in]  dato recive dato a enviar al LCD
 */
static void LCD_Write_8bits(uint8_t dato) {
  LCD_E = LCD_TRUE;
  LCD_D0 = ((dato)&0x01);
  LCD_D1 = ((dato >> 1) & 0x01);
  LCD_D2 = ((dato >> 2) & 0x01);
  LCD_D3 = ((dato >> 3) & 0x01);
  LCD_D4 = ((dato >> 4) & 0x01);
  LCD_D5 = ((dato >> 5) & 0x01);
  LCD_D6 = ((dato >> 6) & 0x01);
  LCD_D7 = ((dato >> 7) & 0x01);
  wait_ns(200);
  LCD_E = (LCD_FALSE);
  wait_ns(50);
}

/**
 *	\fn         void LCD_Write_Data (uint8_t dato)
 *	\brief      Configura las se�ales necesarias para escribir el dato
 *	\author     Nicolás Ferragamo
 *	\date 		14 de octubre del 2020
 *  \param      [in]  dato recive el dato a enviar al LCD
 */
static void LCD_WriteData(uint8_t dato) {
  LCD_RS = LCD_TRUE;
  LCD_RW = LCD_FALSE;
  if (bits_mode == LCD_8BITMODE) {
    LCD_Write_8bits(dato); // Env�a efectivamente el comando
  } else if (bits_mode == LCD_4BITMODE) {
    LCD_Write_4bits(dato); // Env�a efectivamente el comando
  }
}

/**
 *	\fn         void LCD_TicLCD(void)
 *	\brief      Rutina necesaria para el fncionamiento del m�dulo
 *  \details    Esta rutina se debe llama desde la interrupci�n de timer cada
 *1mS \author     Nicolás Ferragamo \date 		14 de octubre del 2020
 */
static void LCD_TicLCD(void) {
  // if (LCD_Tout)
  //   LCD_Tout--;
}

/*********************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 *********************************************************************************************************/

/**
 *	\fn         void LCD_Char2LCD(uint8_t caracter)
 *	\brief      Envia un carater al LCD
 *	\author     Esteban Lemos
 *	\date
 *  \param      [in]  caracter
 */
void LCD_Char2LCD(uint8_t caracter) {
  LCD_WriteData(caracter);
  LCD_ReadBusy();
}

/**
 *	\fn         void LCD_Msg2LCD const uint8_t* msg)
 *	\brief      Envia un string al LCD
 *	\author     Esteban Lemos
 *	\date
 *  \param      [in]  msg
 */
void LCD_Msg2LCD(const char *msg) {
  while (*msg != 0) {
    LCD_WriteData(*msg);
    LCD_ReadBusy();
    msg++;
  }
}

/**
 *	\fn         void LCD_Clear (void)
 *	\brief      Borra el LCD
 *	\author     Nicolás Ferragamo
 *	\date 		14 de octubre del 2020
 *  \param      [in]  mensaje a enviar al LCD
 */
void LCD_Clear(void) {
  LCD_WriteCMD(LCD_CLEARDISPLAY);
  // LCD_ReadBusy();
  delay(2);
}

/**
 *	\fn         void LCD_RetHome (void)
 *	\brief      Regresa el curor al inicio
 *	\author     Nicolás Ferragamo
 *	\date 		14 de octubre del 2020
 */
void LCD_RetHome(void) {
  LCD_WriteCMD(LCD_RETURNHOME);
  // LCD_ReadBusy();
  delay(2);
}

/**
 *	\fn         void LCD_SetCursor (uint8_t)
 *	\brief      Ubica el cursor en una posici�n determinada de la 0 a la 39
 de cada linea *	\author     Nicolás Ferragamo
 *	\date 		14 de octubre del 2020
 *  \param      [in] line 	linea 1 o 2
 *  \param      [in] pos 	posicion delcursor

*/
void LCD_SetCursor(uint8_t line, uint8_t pos) {
  if (pos > 39) {
    pos = 0;
  }
  if (line == 2) {
    pos += 0x40;
  }

  pos |= LCD_SETDDRAMADDR;
  LCD_WriteCMD(pos);
  LCD_ReadBusy();
}

/**
 *	\fn         void LCD_Desp2Izq (void)
 *	\brief      Desplaza al LCD a la izq
 *	\param 		[in] cant cantidad de veces que quiere despplazar
 *	\author     Nicolás Ferragamo
 *	\date 		14 de octubre del 2020
 */
void LCD_Desp2Izq(uint8_t cant) {
  while (cant) {
    LCD_WriteCMD(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
    LCD_ReadBusy();
    cant--;
  }
}

/**
 *	\fn         void LCD_Desp2Der (void)
 *	\brief      Desplaza al LCD a la izq
 *	\param 		[in] cant cantidad de veces que quiere despplazar
 *	\author     Nicolás Ferragamo
 *	\date 		14 de octubre del 2020
 */
void LCD_Desp2Der(uint8_t cant) {
  while (cant) {
    LCD_WriteCMD(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
    LCD_ReadBusy();
    cant--;
  }
}

/**
 * @fn 			LCD_DisplayOff (void)
 * @brief		Apaga el display
 * \author      Nicolás Ferragamo
 * \date 		14 de octubre del 2020
 */
void LCD_DisplayOff(void) {
  LCD_WriteCMD(LCD_DISPLAYCONTROL | LCD_DISPLAYOFF);
  LCD_ReadBusy();
}

/**
 * @fn 			LCD_DisplayOn (void)
 * @brief		Enciende el display
 *  \author      Nicolás Ferragamo
 * \date 		14 de octubre del 2020
 */
void LCD_DisplayOn(void) {
  LCD_WriteCMD(LCD_DISPLAYCONTROL | LCD_DISPLAYON);
  LCD_ReadBusy();
}

/**
 * @fn 			LCD_CursorOff (void)
 * @brief		Apaga el cursor
 *  \author      Nicolás Ferragamo
 * \date 		14 de octubre del 2020
 */
void LCD_CursorOff(void) {
  LCD_WriteCMD(LCD_DISPLAYCONTROL | LCD_CURSOROFF);
  LCD_ReadBusy();
}

/**
 * @fn 			LCD_CursorOn (void)
 * @brief		Enciende el cursor
 *  \author      Nicolás Ferragamo
 * \date 		14 de octubre del 2020
 */
void LCD_CursorOn(void) {
  LCD_WriteCMD(LCD_DISPLAYCONTROL | LCD_CURSORON);
  LCD_ReadBusy();
}

/**
 * @fn 			LCD_BlinkOff (void)
 * @brief		Apaga el blinky
 *  \author      Nicolás Ferragamo
 * \date 		14 de octubre del 2020
 */
void LCD_BlinkOff(void) {
  LCD_WriteCMD(LCD_DISPLAYCONTROL | LCD_BLINKOFF);
  LCD_ReadBusy();
}

/**
 * @fn 			LCD_BlinkOn (void)
 * @brief		Enciende  el blinky
 *  \author      Nicolás Ferragamo
 * \date 		14 de octubre del 2020
 */
void LCD_BlinkOfOn(void) {
  LCD_WriteCMD(LCD_DISPLAYCONTROL | LCD_BLINKON);
  LCD_ReadBusy();
}

/**
 * @fn 			LCD_MoveCursorRight (void)
 * @brief		Mueve el cursor a la derecha
 *  \author      Nicolás Ferragamo
 * \date 		14 de octubre del 2020
 */
void LCD_MoveCursorRight(void) {
  LCD_WriteCMD(LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVERIGHT);
  LCD_ReadBusy();
}

/**
 * @fn 			LCD_MoveCursorLeft (void)
 * @brief		Mueve el cursor a la izquierda
 *  \author      Nicolás Ferragamo
 * \date 		14 de octubre del 2020
 */
void LCD_MoveCursorLeft(void) {
  LCD_WriteCMD(LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVELEFT);
  LCD_ReadBusy();
}

/**
 *	\fn         void LCD_Init(void)
 *	\brief      Inicializa el LCD para el shield 2
 *  \details    Inicializa los puertos del entrenador necesarios para usar
 *              el display LCD e inicializa el LCD propiamente dicho.
 *	\author     Nicolás Ferragamo
 *	\date       14 de octubre del 2020
 */
void LCD_Init(uint8_t lines, uint8_t cursor, uint8_t dot_format, uint8_t mode) {

  LCD_D4 = LCD_FALSE;
  LCD_D5 = LCD_FALSE;
  LCD_D6 = LCD_FALSE;
  LCD_D7 = LCD_FALSE;
  LCD_E = LCD_FALSE;
  LCD_RS = LCD_FALSE;
  LCD_RW = LCD_FALSE;

  // LCD_Tout = 30;          // Espera 30 interrup de timer
  delay(50);
  LCD_WriteCMD(LCD_FUNCTIONSET | LCD_8BITMODE); // la primera instruccipon solo deben ser lo 4 bits altos
                      // de la Funion set
  delay(5);
  LCD_WriteCMD(LCD_FUNCTIONSET | LCD_8BITMODE); // la primera instruccipon solo deben ser lo 4 bits altos
                      // de la Funion set
  delay(1);
 LCD_WriteCMD(LCD_FUNCTIONSET | LCD_8BITMODE); // la primera instruccipon solo deben ser lo 4 bits altos
                      // de la Funion set
  delay(1);

  

  
    LCD_WriteCMD(LCD_FUNCTIONSET | LCD_4BITMODE); // la primera instruccipon solo deben ser lo 4 bits altos
                      // de la Funion set
    delay(1);
  
bits_mode = mode;
  LCD_WriteCMD(LCD_FUNCTIONSET | bits_mode | lines |
               dot_format); // trabajar en 4bits 0010****
                            // LCD_WriteCMD (0X28);
  // LCD_Tout = 1;		// Al menos 50uS
  delay(1);
   LCD_WriteCMD(LCD_DISPLAYCONTROL | LCD_DISPLAYOFF |
               cursor); // disp ON Cursor OFF Blink OFF
  // LCD_WriteCMD (0X0E);
  // LCD_Tout = 1;
  // while (LCD_Tout);
  delay(1);

    LCD_WriteCMD(LCD_CLEARDISPLAY); // borra el display
                                  // LCD_Tout = 1;
  // while (LCD_Tout);
  delay(1);

  LCD_WriteCMD(LCD_ENTRYMODESET |
               LCD_ENTRYINCREMENT); // Incrementar la pos auto SCROLL OFF
                                    // LCD_WriteCMD (0X06);
  // LCD_Tout = 1;
  // while (LCD_Tout);
  delay(1);

  LCD_WriteCMD(LCD_DISPLAYCONTROL | LCD_DISPLAYON |
               cursor); // disp ON Cursor OFF Blink OFF
  // LCD_WriteCMD (0X0E);
  // LCD_Tout = 1;
  // while (LCD_Tout);
  delay(1);
}

/**
 *	\fn         LCD_InitCGRAM
 *	\brief      Inicializa los caracteres de la CGRAM para el shield 2
 *  \details    Iicializa los caracteres creados por el usuario, solo debe
 *llamar a esta función en el main y guardar sus caracteres en la tabla
 *              LCD_CharactersCGRAM[ ]
 *	\author     Nicolás Ferragamo
 *	\date       14 de octubre del 2020
 *//**
 *  \brief      Initializes the custom characters for the LCD shield
 *  \details    Initializes the custom characters created by the user. This function should
 *              only be called once in the main function, and the custom characters should be
 *              stored in the LCD_CharactersCGRAM[] table.
 *	\author     Nicolás Ferragamo
 *	\date       14 de octubre del 2020
*/
void LCD_InitCGRAM(void) {
  uint8_t i = 0;

  LCD_WriteCMD(LCD_SETCGRAMADDR); // Set CGRAM address to 0

  for (i = 0; i < 64; i++) // Fill the CGRAM with the custom characters
  {
    LCD_Char2LCD(LCD_CharactersCGRAM[i]);
  }
  LCD_WriteCMD(LCD_SETDDRAMADDR); // Set DDRAM address to 0
  LCD_RetHome();                  // Move cursor to first position of first line
}