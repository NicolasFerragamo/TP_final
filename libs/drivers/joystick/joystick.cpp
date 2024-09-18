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
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "delay.h"
#include "mbed.h"
#include "joystick.h"


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
AnalogIn    joystick_x(A0); //!< Joystick eje x
AnalogIn    joystick_y(A1); //!< Joystick eje y
InterruptIn joystick_sw(D2,PullUp);

static delay_t joystick_sample_period; //!< Periodo de muestreo del ADC
static joystick_t joystick_adc = {512,512};

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
    \fn         void temp_sensor_Init(uint64_t fs, uint64_t screen_update)
    \brief      Inicializa el sensor de temperatura, se establece la frecuencia
                de muestreo del ADC y la frecuencia de actualización de la pantalla,
                se setea la referencia del sensor de temperatura a 3.3V
    \param      fs [in] Frecuencia de muestreo del ADC
    \param      screen_update [in] Frecuencia de actualización de la pantalla
    \author     Nicolas Ferragamo
    \date       ${date}
*/
void joystick_Init(uint64_t fs, void (*joystick_sw_Callback) (void))
{
    delayInit(&joystick_sample_period, fs);
    joystick_x.set_reference_voltage(3.3);
    joystick_y.set_reference_voltage(3.3);
    joystick_sw.fall(joystick_sw_Callback);
}

/**
    \fn         void joystick_update(void)
    \brief      Lee el sensor de temperatura, promedia AVERAGE_SAMPLES muestras.
                Se lee el sensor cada fs segundos. (es un filtrado)
    \author     Nicolas Ferragamo
    \return     Temperatura en volts, con el último valor promediado, no la última muestra.
    \date       ${date}
*/
void joystick_update(void)
{
    if (delayRead(&joystick_sample_period))
    {
        joystick_adc.eje_x = joystick_x.read_u16();	
        joystick_adc.eje_y = joystick_y.read_u16();	
        delayRead(&joystick_sample_period); // Reinicio el delay
    }
}



/**
    \fn         void joystick_read(joystick_t *dato)
    \brief  
    \param      [in] variable donde se sevuelve el último convertido.   
    \author     Nicolas Ferragamo
    \date       ${date}
*/
void joystick_read(joystick_t *dato)
{
    dato->eje_x =  joystick_adc.eje_x;
    dato->eje_y =  joystick_adc.eje_y;
}
