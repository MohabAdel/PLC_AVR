/*
 * shifter.h
 *
 * Created: 7/15/2018 4:21:41 PM
 *  Author: Mohab
 */ 


#ifndef SHIFTER_H_
#define SHIFTER_H_
#define  LSBFIRST  0
#include <stdint.h>
#include <avr/io.h>
#include "../types/hw_types.h"
//data pin
#define SHIFTER_DATA_DIRECTION_REG	 DDRC		
#define SHIFTER_DATA_DATA_REG		 PORTC   
#define SHIFTER_DATA_PIN_NUM		 0

//clk pin
#define SHIFTER_CLK_DIRECTION_REG	 DDRC
#define SHIFTER_CLK_DATA_REG		 PORTC	
#define SHIFTER_CLK_PIN_NUM			 2

//latch pin to seven segment
#define SHIFTER_LATCH_DISPLAY_DIRECTION_REG	  DDRC	
#define SHIFTER_LATCH_DISPLAY_DATA_REG		  PORTC
#define SHIFTER_LATCH_DISPLAY_PIN_NUM		  1

//latch pin to output
#define SHIFTER_LATCH_OUTPUT_DIRECTION_REG	 DDRB
#define SHIFTER_LATCH_OUTPUT_DATA_REG		 PORTB
#define SHIFTER_LATCH_OUTPUT_PIN_NUM		 5

#define SHIFTER_RESET_DISPLAY_DIRECTION_REG   DDRC
#define SHIFTER_RESET_DISPLAY_DATA_REG		  PORTC
#define SHIFTER_RESET_DISPLAY_PIN_NUM		  3

#define SHIFTER_RESET_OUTPUT_DIRECTION_REG   DDRC
#define SHIFTER_RESET_OUTPUT_DATA_REG		 PORTC
#define SHIFTER_RESET_OUTPUT_PIN_NUM		 5




void Shifter_Init(void);
void ShiftOut_8bit(uint8_t bitOrder, uint8_t val);
void ShiftOut_16bit(uint8_t bitOrder, uint16_t val);
void Shifter_Display_Latch(void);
void Shifter_Output_Latch(void);

#endif /* SHIFTER_H_ */