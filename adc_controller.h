/**
 * @file        adc_controller.h
 * @brief       Header controller for definitions and functions
 * @author      Jesus G. Palomo
 * @date        15-May-2019
 * @version     Version 1.0
 */

#ifndef ADC_CONTROLLER_H_
#define ADC_CONTROLLER_H_

#include <avr/io.h>
#include <util/delay.h>

#define ON_SIGNAL_0	PORTD |= (1<<PD4);
#define OFF_SIGNAL_0	PORTD &=~(1<<PD4);
#define ON_SIGNAL_1	PORTB |= (1<<PB3);
#define OFF_SIGNAL_1	PORTB &=~ (1<<PB3);
#define TIME_SIGNAL	650
#define ADC_OBJ_0 	(&data_filter)

/*! Class structure for processing voltage from Infrared sensors */
typedef struct{
	float off_set_value; /**< Define the volume to sensing (tuning) */
	float volts_input; /**< Save the voltage value */
	float value_trigger; /**< The voltage when all phototransistor are excited*/
	uint16_t adc_result; /**< Save the ADC conversion result*/
	uint16_t sampling_adc; /**< Number of sampling for reduce noise signal*/
	uint8_t adc_channel; /**< Channel for reader analog to digital convertion*/
	uint16_t not_infrared_light; /**< If the infrared light does not exist in the system*/
	uint8_t new_calibration; /**< Parameter for call neww calibration*/
}adc_d;

/** Prototype function for ADC reading */
void adc_read_channel(adc_d *data);

/** Prototype function for converting */
void reading_volts(adc_d *data);

/** Prototype function */
void set_point_trigger(adc_d *data);

/** Prototype function */
void wait_milli_second(uint16_t delay);

#endif /* ADC_CONTROLLER_H_ */
