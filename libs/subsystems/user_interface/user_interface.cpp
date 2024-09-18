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
#include "user_interface.h"
#include "arm_book_lib.h"
#include "delay.h"
#include "lcd.h"
#include "mbed.h"
#include <stddef.h> // para NULL
#include <string.h>
#include "encoder.h"
#include "neopixel.h"

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

static MenuItem_t menu_main_play;
static MenuItem_t menu_main_score;
static MenuItem_t menu_main_dificulty;

static MenuItem_t menu_play_score;
static MenuItem_t manu_play_back;

static MenuItem_t menu_difficulty_easy;
static MenuItem_t menu_difficulty_normal;
static MenuItem_t menu_difficulty_hard;
static MenuItem_t menu_difficulty_back;

static MenuItem_t menu_score_1;
static MenuItem_t menu_score_2;
static MenuItem_t menu_score_3;
static MenuItem_t menu_score_4;
static MenuItem_t menu_score_5;
static MenuItem_t menu_score_back;

static MenuItem_t * ptr_inicio = &menu_main_play;

delay_t screen_refresh;

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);     //!< comunicación serial
char buffer[20] = "";

/*********************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 *********************************************************************************************************/

/*********************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 *********************************************************************************************************/

/*********************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 *********************************************************************************************************/

/**
  \fn           delayInit(delay_t *delay, uint64_t duration)
  \brief        Inicio del temporizador
  \param        delay [in] estructura del temporizador
  \param        duration [in] duración del temporizador
  \author       Nombre
  \date         ${date}
  \return       tipo y descripción de retorno
*/

void menu_init(void) {

    MenuItem_t menu_main_play =     {.item = "Play",
                                      .accion = NULL,
                                      .subMenu = &menu_play_score,
                                      .siguiente = &menu_main_score,
                                      .anterior = &menu_main_dificulty};
    // sprintf(buffer,"%s", menu_main_play.item);
    // uartUsb.write(buffer, strlen(buffer));
    // uartUsb.write("\r\n",3);
    MenuItem_t menu_main_score =    {.item = "Score",
                                      .accion = NULL,
                                      .subMenu = &menu_difficulty_easy,
                                      .siguiente = &menu_main_dificulty,
                                      .anterior = &menu_main_play};
    // sprintf(buffer,"%s", menu_main_score.item);
    // uartUsb.write(buffer, strlen(buffer));
    // uartUsb.write("\r\n",3);
    MenuItem_t menu_main_dificulty = {.item = "Difficulty",
                                      .accion = NULL,
                                      .subMenu = &menu_score_1,
                                      .siguiente = &menu_main_play,
                                      .anterior = &menu_main_score};
    // sprintf(buffer,"%s", menu_main_dificulty.item);
    // uartUsb.write(buffer, strlen(buffer));
    // uartUsb.write("\r\n",3);
    MenuItem_t menu_play_score =    {.item = "Score",
                                     .accion = NULL,
                                     .subMenu = NULL,
                                     .siguiente = &manu_play_back,
                                     .anterior = &manu_play_back};
    // sprintf(buffer,"%s", menu_play_score.item);
    // uartUsb.write(buffer, strlen(buffer)); 
    // uartUsb.write("\r\n",3);                                
    MenuItem_t manu_play_back   =   {.item = "Back",
                                     .accion = NULL, // terminar luego, ir al menú rincipal
                                     .subMenu = &menu_main_play,
                                     .siguiente = &menu_play_score,
                                     .anterior = &menu_play_score};
    // sprintf(buffer,"%s", manu_play_back.item);
    // uartUsb.write(buffer, strlen(buffer));
    // uartUsb.write("\r\n",3);

    MenuItem_t menu_difficulty_easy =   {.item = "Easy",
                                         .accion = NULL,
                                         .subMenu = NULL,
                                         .siguiente = &menu_difficulty_normal,
                                         .anterior = &menu_difficulty_back};
    // sprintf(buffer,"%s", menu_difficulty_easy.item);
    // uartUsb.write(buffer, strlen(buffer));   
    // uartUsb.write("\r\n",3);                                  
    MenuItem_t menu_difficulty_normal = {.item = "Normal",
                                         .accion = NULL,
                                         .subMenu = NULL,
                                         .siguiente = &menu_difficulty_hard,
                                         .anterior = &menu_difficulty_easy};
    // sprintf(buffer,"%s", menu_difficulty_normal.item);
    // uartUsb.write(buffer, strlen(buffer));  
    // uartUsb.write("\r\n",3);                                   
    MenuItem_t menu_difficulty_hard =   {.item = "Hard",
                                         .accion = NULL,
                                         .subMenu = NULL,
                                         .siguiente = & menu_difficulty_back,
                                         .anterior = &menu_difficulty_normal};
    // sprintf(buffer,"%s", menu_difficulty_hard.item);
    // uartUsb.write(buffer, strlen(buffer)); 
    // uartUsb.write("\r\n",3);                                    
    MenuItem_t menu_difficulty_back =   {.item = "Back",
                                         .accion = NULL,
                                         .subMenu = &menu_main_play,
                                         .siguiente = &menu_difficulty_easy,
                                         .anterior = &menu_difficulty_hard};
    // sprintf(buffer,"%s", menu_difficulty_back.item);
    // uartUsb.write(buffer, strlen(buffer));
    // uartUsb.write("\r\n",3);

    MenuItem_t menu_score_1 =   {.item = "1°:     2°:     ",
                                 .accion = NULL,
                                 .subMenu = NULL,
                                 .siguiente = &menu_score_2,
                                 .anterior = &menu_score_back};
    // sprintf(buffer,"%s", menu_score_1.item);
    // uartUsb.write(buffer, strlen(buffer));  
    // uartUsb.write("\r\n",3);                           
    MenuItem_t menu_score_2 =   {.item = "3°:     4°:     ",
                                 .accion = NULL,
                                 .subMenu = NULL,
                                 .siguiente = &menu_score_3,
                                 .anterior = &menu_score_1};
    // sprintf(buffer,"%s", menu_score_2.item);
    // uartUsb.write(buffer, strlen(buffer));  
    // uartUsb.write("\r\n",3);                           
    MenuItem_t menu_score_3 =   {.item = "5°:     6°:     ",
                                 .accion = NULL,
                                 .subMenu = NULL,
                                 .siguiente = &menu_score_4,
                                 .anterior = &menu_score_2};
    // sprintf(buffer,"%s", menu_score_3.item);
    // uartUsb.write(buffer, strlen(buffer));   
    // uartUsb.write("\r\n",3);                          
    MenuItem_t menu_score_4 =   {.item = "7°:     8°:     ",
                                 .accion = NULL,
                                 .subMenu = NULL,
                                 .siguiente = &menu_score_5,
                                 .anterior = &menu_score_3};
    // sprintf(buffer,"%s", menu_score_4.item);
    // uartUsb.write(buffer, strlen(buffer));  
    // uartUsb.write("\r\n",3);                           
    MenuItem_t menu_score_5 =   {.item = "9°:     10°:    ",
                                 .accion = NULL,
                                 .subMenu = NULL,
                                 .siguiente = &menu_score_back,
                                 .anterior = &menu_score_4};
    // sprintf(buffer,"%s", menu_score_5.item);
    // uartUsb.write(buffer, strlen(buffer)); 
    // uartUsb.write("\r\n",3);                            
    MenuItem_t menu_score_back = {.item = "Back",
                                  .accion = NULL,
                                  .subMenu = &menu_main_play,
                                  .siguiente = &menu_score_1,
                                  .anterior = &menu_score_5};
    // sprintf(buffer,"%s", menu_score_back.item);
    // uartUsb.write(buffer, strlen(buffer));  
    // uartUsb.write("\r\n",3);                            

    ptr_inicio = &menu_main_play;

    // sprintf(buffer,"%s", ptr_inicio->item);
    // uartUsb.write(buffer, strlen(buffer));
    // uartUsb.write("\r\n",3);
    // sprintf(buffer,"%s", ptr_inicio->siguiente->item); // no reconoce lo que esta guardado en siguiente->item
    // uartUsb.write(buffer, strlen(buffer));
    // uartUsb.write("\r\n",3);

    LCD_Clear();
    delayInit(&screen_refresh, 100); 
    delayRead(&screen_refresh);
    LCD_SetCursor(1, 0);
    LCD_Msg2LCD(ptr_inicio->item);
    //LCD_Msg2LCD(menu_main_play.item); 
    LCD_SetCursor(2, 0);
    LCD_Msg2LCD(ptr_inicio->siguiente->item); // no reconoce lo que esta guardado en siguiente->item
    //LCD_Msg2LCD(menu_main_score.item);
}



void menu_update (void)
{
    encoder_sentido_e giro;
    neopixel_color_t color = {64,0,0};
    if(delayRead(&screen_refresh) == true)
    {
        delayRead(&screen_refresh);
         giro = encoder_read();
            //LCD_SetCursor(1, 11);
            if (giro == NEUTRO)
            {
                //LCD_Char2LCD('N');
            } else if (giro == DERECHA)
            {
               // LCD_Char2LCD('D');
                color = {64,0,0};
                neopixel_write(&color);
                ptr_inicio = ptr_inicio->siguiente;
                LCD_Clear();
                LCD_SetCursor(1, 0);
                LCD_Msg2LCD(ptr_inicio->item);
                LCD_SetCursor(2, 0);
                LCD_Msg2LCD(ptr_inicio->siguiente->item);

            } else if (giro == IZQUIERDA)
            {
                //LCD_Char2LCD('I');
                color = {0,0,64};
                neopixel_write(&color);
                ptr_inicio = ptr_inicio->anterior;
                LCD_Clear();
                LCD_SetCursor(1, 0);
                LCD_Msg2LCD(ptr_inicio->item);
                LCD_SetCursor(2, 0);
                LCD_Msg2LCD(ptr_inicio->siguiente->item);
            }

    }
}

