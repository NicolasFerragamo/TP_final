/*********************************************************************************************************
 *
 * \file    ${file_name}
 * \brief   Librería para el manejo de matriz de puntos
 * \date    ${date}
 * \author  Nicolás Ferragamo
 * \version 1.0
*********************************************************************************************************/

/*********************************************************************************************************
 *** MODULO
*********************************************************************************************************/

#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

/*#ifdef __cplusplus
extern "C" {
#endif*/

/*********************************************************************************************************
 *** INCLUDES GLOBALES
*********************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mbed.h"

/*********************************************************************************************************
 *** DEFINES GLOBALES
*********************************************************************************************************/
#define MAX_BUFFER 20    
/*********************************************************************************************************
 *** MACROS GLOBALES
*********************************************************************************************************/
#ifndef _RESERVED_BITS
#define _RESERVED_BITS
#define  reserved_bits(x,y,z)   uint8_t reserved_bits##x[z-y+1];
#endif
/*********************************************************************************************************
 *** TIPO DE DATOS GLOBALES
*********************************************************************************************************/
/**
 * @struct  delay_t
 * @brief   Contiene las variables necesarias para el maneo de los delays
 * @date    ${date}
 * @author  Nicolas Ferragamo nferragamo@frba.utn.edu.ar
 */

// Estructura que representa un ítem del menú
typedef struct MenuItem {
    //const char *item; // Nombre del ítem a mostrar en el LCD
    char item[MAX_BUFFER];
    void (*accion)(void); // Acción a ejecutar cuando se selecciona el ítem
    struct MenuItem *subMenu; // Puntero al submenú, si existe
    struct MenuItem *siguiente; // Puntero al siguiente ítem del mismo nivel
    struct MenuItem *anterior; // Puntero al anterior ítem del mismo nivel
} MenuItem_t;

/*********************************************************************************************************
 *** VARIABLES GLOBALES
*********************************************************************************************************/

/*********************************************************************************************************
 *** PROTOTIPOS DE FUNCIONES GLOBALES
**********************************************************************************************************/

void menu_init (void);
void menu_update (void);
//void play (void);
//void score (void);
//void dificulty (void);

/*#ifdef __cplusplus
}
#endif*/

#endif /* __DELAY_H */

