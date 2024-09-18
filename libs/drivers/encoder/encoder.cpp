/*********************************************************************************************************
 *
 * \file		${file_name}
 * \brief		
 * \date		${date}
 * \author		
 * \version 
*********************************************************************************************************/

/*********************************************************************************************************
 *** INCLUDES
*********************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include "mbed.h"
#include "encoder.h"
#include "lcd.h"


/*********************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
*********************************************************************************************************/


/*********************************************************************************************************
 *** MACROS PRIVADAS AL MODULO
*********************************************************************************************************/

/*********************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO
*********************************************************************************************************/
typedef struct
{
    bool A;
    bool B;
} encoder_t;

/*********************************************************************************************************
 *** TABLAS PRIVADAS AL MODULO
*********************************************************************************************************/

/*********************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
*********************************************************************************************************/

/*********************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
*********************************b************************************************************************/
InterruptIn     encoder_A(D4); //!< 
DigitalIn       encoder_B(D3); //!< 
InterruptIn     encoder_sw(D5,PullUp);


static encoder_t encoder = {false,false};
static encoder_sentido_e sentido = NEUTRO;

/*********************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
*********************************************************************************************************/
void encoder_a_Callback (void);

/*********************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
*********************************************************************************************************/
void encoder_a_Callback (void)
{
    if(encoder_B == 1)
    {
        sentido = DERECHA;
    }
    else
    {
        sentido = IZQUIERDA;
    }
}


/*********************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
*********************************************************************************************************/
/**
    \fn         void temp_sensor_Init(uint64_t fs, uint64_t screen_update)
    \brief      Inicializa el sensor de temperatura, se establece la frecuencia
                de muestreo del ADC y la frecuencia de actualización de la pantalla,
                se setea la referencia del sensor de temperatura a 3.3V
    \param      fs [in] Frecuencia de muestreo del ADC
    \param      screen_update [in] Frecuencia de actualización de la pantalla
    \author     Nicolas Ferragamo
    \date       ${date}
*/
void encoder_Init(void (*encoder_sw_Callback) (void))
{
    encoder_A.rise(encoder_a_Callback);
    encoder_sw.fall(encoder_sw_Callback);
}


encoder_sentido_e encoder_read(void)
{
    encoder_sentido_e aux = sentido;
    sentido = NEUTRO;
    return aux;
}
