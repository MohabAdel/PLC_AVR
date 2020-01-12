/*
 * SevenSegment.h
 *
 * Created: 7/15/2018 3:54:40 PM
 *  Author: Mohab
 */ 


#ifndef SEVENSEGMENT_H_
#define SEVENSEGMENT_H_
#include <stdint.h>

//#define 
#include "../Shifter/shifter.h"

#define FRAMEPERIOD 2000
//Total amount of time (in microseconds) for the display frame. 1,000us is roughly 1000Hz update rate
//A framePeriod of:
//5000 is flickery
//3000 has good low brightness vs full brightness
//2000 works well
//500 seems like the low brightness is pretty bright, not great

#define BLANK 16 //Special character that turns off all segments (we chose 16 as it is the first spot that has this)
 
 //This is the combined array that contains all the segment configurations for many different characters and symbols
#define SEVEN_SEGMENT_S1_DIRECTION_REG  DDRB
#define SEVEN_SEGMENT_S1_DATA_REG		PORTB
#define SEVEN_SEGMENT_S1_PIN_NUM		1

#define SEVEN_SEGMENT_S2_DIRECTION_REG	DDRB
#define SEVEN_SEGMENT_S2_DATA_REG		PORTB
#define SEVEN_SEGMENT_S2_PIN_NUM		2

#define SEVEN_SEGMENT_S3_DIRECTION_REG	DDRB
#define SEVEN_SEGMENT_S3_DATA_REG		PORTB
#define SEVEN_SEGMENT_S3_PIN_NUM		3
	
void SevenSegment_Init(void);
void SevenSegment_Upper_Digit_Display(uint16_t x,uint8_t Auth);
void SevenSegment_Lower_Digit_Display(uint16_t x,uint8_t Auth);
void SevenSegment_Upper_Display(char* ptoDisplay,uint8_t Auth);
void SevenSegment_Lower_Display(char* ptoDisplay,uint8_t Auth);
void SevenSegment_Lower_Start_Blinking(void);
void SevenSegment_Lower_Stop_Blinking(void);
void To_Display(void);
/*void Hold_Display(void);*/
#endif /* SEVENSEGMENT_H_ */