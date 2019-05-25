/**
 * @file        main.c
 * @brief       In this code I try to show you good defensive programming skills. I hope you find helpfully.
 * @author      Jesus G. Palomo
 * @date        15-May-2019
 * @version     Version 1.0
 * @details
 		Device: atmega8
		Program:    1386 bytes (16.9% Full)
		(.text + .data + .bootloader)

		Data:          0 bytes (0.0% Full)
		(.data + .bss + .noinit)

		Crystal: 8 Mhz Internal
 */

#include "adc_controller.h"

/** Analog Digital Converter function prototype  **/
static void init_adc(void);

/** Digital outputs function prototype  **/
static void  io_settings(void);

/**
 * @brief           main
 * @details         main function, set data and keep the while process
 */
int main(void)
{

	io_settings();
	init_adc();

	adc_d data_filter;

	/*Setting the initial condition*/
	data_filter.adc_channel = 0;
	data_filter.sampling_adc = 150;
	data_filter.new_calibration = 0;
	data_filter.not_infrared_light = 450;
	data_filter.adc_result = 0;
	data_filter.value_trigger = 0;
	data_filter.volts_input = 0;
	data_filter.off_set_value = 0.8999;

	set_point_trigger(ADC_OBJ_0);

	while(1)
	{

		reading_volts(ADC_OBJ_0);

		if ( (data_filter.volts_input - data_filter.off_set_value) > data_filter.value_trigger ) {
			/* The sensor detected an item, then the microcontroller turn on a PIN */
			ON_SIGNAL_0;
			wait_milli_second(TIME_SIGNAL);
			data_filter.new_calibration+=1;
		}
		else if (data_filter.volts_input  < data_filter.value_trigger)
		{
			/* if something block the sensor recalibrated */
			data_filter.new_calibration = 0;
			set_point_trigger(ADC_OBJ_0);
		}
		else{
			OFF_SIGNAL_0;
		}

		/* time out for detecting item, the sensors had to be recalibrated*/
		if (data_filter.new_calibration > 2)
		{
			data_filter.new_calibration = 0;
			set_point_trigger(ADC_OBJ_0);
		}

		/* The infrared light does not provide by emitter*/
		if (data_filter.adc_result > data_filter.not_infrared_light)
		{
			OFF_SIGNAL_1;
		}
		else
		{
			ON_SIGNAL_1;
		}

	}

	return 0;
}


/**
 * @brief	init_adc
 * @details	Setting the ADC with the following parameters
 * AREF = VCC whit external capacitor at AREF pin, PRESCALER = 128
 */
static void init_adc(void)
{
	ADMUX = (1<<REFS0);
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

/**
 * @brief	init_adc
 * @details	Setting the ADC with the following parameters
 */
static void  io_settings(void)
{
	DDRD|=(1<<PD7)|(1<<PD6)|(1<<PD5)|(1<<PD4);
	DDRB |= (1<<PB3);
}
