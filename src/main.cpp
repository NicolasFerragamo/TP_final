/*********************************************************************************************************
 * <Module name>
 * <Module description
 *
 * <Copyright>
 *
 * <Copyright or distribution terms>
 *
 *
 *********************************************************************************************************/

/*********************************************************************************************************
 * <File description>
 *
 * Filename       : Filename
 * Version        : Module version
 * Programmer(s)  : Programmer initial(s)
 **********************************************************************************************************
 *  Note(s):
 *
 *
 *
 *********************************************************************************************************/

/*********************************************************************************************************
 *
 * \file        ${file_name}
 * \brief       Descripción del modulo
 * \date        ${date}
 * \author      Nicolas Ferragamo nferragamo@est.frba.utn.edu.ar
 * \version
 *********************************************************************************************************/

/*********************************************************************************************************
 *** INCLUDES
 *********************************************************************************************************/
#include "mbed.h"
#include "arm_book_lib.h"
#include "delay.h"
#include <stdio.h>
#include <stdlib.h>
#include "joystick.h"
#include "lcd.h"
#include <string.h>
#include "encoder.h"
#include "neopixel.h"
#include "user_interface.h"

/*********************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 *********************************************************************************************************/


/*********************************************************************************************************
 *** MACROS PRIVADAS AL MODULO
 *********************************************************************************************************/

/*********************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO
 *********************************************************************************************************/


/*********************************************************************************************************
 *** TABLAS PRIVADAS AL MODULO
 *********************************************************************************************************/

/*********************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 *********************************************************************************************************/



/*********************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 *********************************************************************************************************/
static delay_t mostrar; //!< Periodo de muestreo del ADC

/*********************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 *********************************************************************************************************/


/*********************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 *********************************************************************************************************/


/*********************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 *********************************************************************************************************/


 void Joystick_sw_Callback (void)
{
    static bool estado = 0;
    char buffer[4];
    estado = !estado;
    LCD_SetCursor(2, 10);
    sprintf(buffer, "%d", estado);
    LCD_Msg2LCD(buffer);
}

 void encoder_sw_Callback (void)
{
    static bool estado = 0;
    char buffer[4];
    estado = !estado;
    LCD_SetCursor(1, 7);
    sprintf(buffer, "%d", estado);
    LCD_Msg2LCD(buffer);
}

/**
    \fn         int main(void)
    \brief      Función principal
    \author     Nicolas Ferragamo
    \date       ${date}
*/


int main(void)
{
    joystick_t dato;
    char dato_string[6];
    delay_1ms_init();
    LCD_Init(LCD_2LINE, LCD_CURSOROFF, LCD_5x8DOTS, LCD_4BITMODE);
    menu_init();
    joystick_Init(50, Joystick_sw_Callback);  // actualiza la medición cad 50ms
    encoder_Init(encoder_sw_Callback); 
    delayInit(&mostrar, 1000);
    delayRead(&mostrar);
    while (true)
    {
        joystick_update();
        menu_update();
        if (delayRead(&mostrar))
        {
            joystick_read(&dato);
            /*
            LCD_SetCursor(1, 0);
            LCD_Msg2LCD("     ");
            LCD_SetCursor(1, 0);
            sprintf(dato_string, "%d", dato.eje_x);
            LCD_Msg2LCD(dato_string);
            LCD_SetCursor(2, 0);
            LCD_Msg2LCD("     ");
            LCD_SetCursor(2, 0);
            sprintf(dato_string, "%d", dato.eje_y);
            LCD_Msg2LCD(dato_string);*/
            delayRead(&mostrar);
           
        }
    }
    return 1;
}