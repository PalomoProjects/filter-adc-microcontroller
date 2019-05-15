/**
 * @file		adc_controller.c
 * @brief       Functions files for controller header
 * @author      Jesus G. Palomo
 * @date        15-May-2019
 * @version     Version 1.0
 */

#include "adc_controller.h"


/**
 * @brief	adc_read_channel
 * @details	This function implement a filter for reduce the noise from the signal.
 */
void adc_read_channel(adc_d *data)
{
	data->adc_result = 0;
	data->adc_channel = ( data->adc_channel & (0b00000111) );
	ADMUX = ( (ADMUX & 0xF8) | (data->adc_channel) );
	/*Applying digital filter for noise cancellation*/
	for (uint16_t i = 0; i < data->sampling_adc; ++i)
	{
		ADCSRA|=(1<<ADSC);
		while(!(ADCSRA & (1<<ADIF)));
		ADCSRA|=(1<<ADIF);
		data->adc_result += ADC;
	}
	/*calculate the average*/
	data->adc_result /= data->sampling_adc;
}


/**
 * @brief   converting_to_volts
 * @details
				The equation for ADC Conversion is ADC = (VIN / vVREF) * 1023 applying algebra:
  				1. ADC = (VIN * 1024) / VREF
 				2. VIN = (5 * ADC) / 1024
 				3. ADC * ( 5 / 1024 )
 				4. ADC * 0.004882812 (Final equation)
				volts_input had a voltage divisor with resistors between 1.2k and 1.2k

 */
void reading_volts(adc_d *data){
	data->volts_input = 0;
	adc_read_channel(data);
	data->volts_input = data->adc_result;
	data->volts_input *= 0.004882812;
	data->volts_input *= 100;
}


/**
 * @brief	set_point_trigger
 * @details	This function need to be call when the sensor need a recalibrate
 */
void set_point_trigger(adc_d *data)
{
	data->value_trigger = 0;
	adc_read_channel(data);
	data->value_trigger = data->adc_result;
	data->value_trigger *= 0.004882812;
	data->value_trigger *= 100;

}

/**
 * @brief	wait_milli_second
 * @details	function for delayed process
 */
void wait_milli_second(uint16_t delay)
{
	for (uint16_t i=0;i < delay;i++){
		_delay_ms(1);
	}
}
