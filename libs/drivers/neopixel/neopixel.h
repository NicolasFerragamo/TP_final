/*********************************************************************************************************
 *
 * \file		${file_name}
 * \brief		
 * \date		${date}
 * \author		
 * \version
*********************************************************************************************************/

/*********************************************************************************************************
 *** MODULO
*********************************************************************************************************/

#ifndef _NEOPIXEL_H
#define _NEOPIXEL_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************
 *** INCLUDES GLOBALES
*********************************************************************************************************/
#include <stddef.h>
#include <stdint.h>

/*********************************************************************************************************
 *** DEFINES GLOBALES
*********************************************************************************************************/


/*********************************************************************************************************
 *** MACROS GLOBALES
*********************************************************************************************************/


/*********************************************************************************************************
 *** TIPO DE DATOS GLOBALES
*********************************************************************************************************/
typedef struct
{
    uint8_t G;
    uint8_t R;
    uint8_t B;
} neopixel_color_t;

/*********************************************************************************************************
 *** VARIABLES GLOBALES
*********************************************************************************************************/

/*********************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
**********************************************************************************************************/
void neopixel_write(const neopixel_color_t * color);


#ifdef __cplusplus
}
#endif

#endif /*  */
