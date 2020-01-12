/*
 * Input_Devices.c
 *
 * Created: 7/17/2018 4:29:11 PM
 *  Author: Mohab
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>
#include "Input_Devices.h"
#include "../types/hw_types.h"
#include <stdbool.h>
#include "../GlobalErrors.h"
#include "../SettingParameters.h"
#include <avr/io.h>
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>

static void Mode_Button_Input_Interrupt_Enable(void);
static void Tempreature_Sensor_Analog_Channel_Enable(void);


#ifdef GAS_INTERRUPT
static void Gas_Sensor_Input_Interrupt_Enable(void);
#endif
void Input_Devices_Init(void){
	//all inputs are pulled up
	
	
//Mode input
	MODE_BUTTON_DIRECTION_REG &=~ (1<<MODE_BUTTON_PIN_NUM);
	Mode_Button_Input_Interrupt_Enable();


//Gas_input 
	GAS_SENSOR_DIRECTION_REG &=~ (1<<GAS_SENSOR_PIN_NUM); //input
#ifdef GAS_INTERRUPT
	Gas_Sensor_Input_Interrupt_Enable();
#endif	
	

	
	
	//up input
	UP_BUTTON_DIRECTION_REG &=~(1<<UP_BUTTON_PIN_NUM);
	
	//down input
	DOWN_BUTTON_DIRECTION_REG &=~(1<<DOWN_BUTTON_PIN_NUM);
	
	//set input
	SET_BUTTON_DIRECTION_REG &=~(1<<SET_BUTTON_PIN_NUM);
	
	//Flame1 Input
	FLAME1_SENSOR_DIRECTION_REG &=~(1<<FLAME1_SENSOR_PIN_NUM);
	
	//Flame2 Input  with flag checking
	OW_DIRECTION_REG &=~(1<<OW_PIN_NUM);
	
	//temp input //analog input
	//TEMPREATURE_SENSOR_DIRECTION_REG &=~(1<<TEMPREATURE_SENSOR_PIN_NUM); //input
	Tempreature_Sensor_Analog_Channel_Enable();
	
	


	
	

	
}


//EICRA &=~ (1 << ISC11);    // set INT1 to trigger on the falling edge

static void Mode_Button_Input_Interrupt_Enable(void){
	
	EICRA |= (1 << ISC01);    // set INT0 to trigger on the falling edge
	//EICRA &=~ (1 << ISC00);    // set INT0 to trigger on the falling edge
	EIMSK |= (1 << INT0);     // Turns on INT0

	sei();                    // turn on interrupts
}

ISR (INT0_vect) //mode button
{
	//PORTD^=(1<<7);
	static uint8_t count=0;
	count++;
	if(count ==2)count=0;
	if(count==1){
		System_Operating_Mode_Change(CONFIGURATION_MODE);
		_delay_ms(2);
	}
	else if(count==0){
		System_Operating_Mode_Change(NORMAL_MODE);
		_delay_ms(2);
	}
	
	
}




#ifdef GAS_INTERRUPT
static void Gas_Sensor_Input_Interrupt_Enable(void){
		EICRA |= (1 << ISC11);    // set INT1 to trigger on the falling edge
		EIMSK |= (1 << INT1);     // Turns on INT1
		sei();                    // turn on interrupts
		
	
}

ISR (INT1_vect) //Gas input
{
	//PORTD^=(1<<7);
	Gas_leakage_Flag=1;
}


#endif



static void Tempreature_Sensor_Analog_Channel_Enable(void){
	DDRC &=~(1<<4);
	ADMUX = 0x04;
	ADCSRA = 0xDF; 
	sei();
	
}
uint8_t converted=0;
double ADC_Data_value=0.0;
ISR(ADC_vect)
{
	//static  uint16_t count=0;
	uint32_t an_data=0;
	an_data = (ADCL) | (ADCH << 8);
	ADC_Data_value =  3.29 * an_data/1024;
	converted=1;
	
	//count++;
}
	
static void Tempreature_Signal_To_read(void){
	ADCSRA |= (1<<4)|(1<<6);
}

uint16_t Read_Tempreature_Sensor(void){
	static double Old_Temp_Read=0.0;
	double return_value=0;
	double Temp , rt, a = 0.0039083 , r0 =100, y, b = -0.000000577,x ; 
	Tempreature_Signal_To_read();
	uint16_t time_out=8000;//500 ms delay on 16MHz clock
	while(converted==0 && time_out){time_out--;} //wait the new data from ADC interrupt
	if(time_out==0){
		return_value=Old_Temp_Read;
	}
	else{
		converted=0;
		rt = ADC_Data_value /0.01513  ;

		//Serial.println("R  = " + (String)rt);
		// Getting The temperature
		Temp = (-1 * a) ;
		x =  pow(a,2);
		y = 4  * b * (1 - (rt/r0));
		x = x - y;
		x = pow(x,0.5);
		Temp = Temp + x ;
		Temp /= (2 * b);


		Old_Temp_Read=round(Temp);
		return_value=Old_Temp_Read;
	}
	return return_value;
	
}

//returns 1 if gas leakage
uint8_t Is_Gas_Leakage(void){
	uint8_t res=0;
	if(((GAS_SENSOR_DATA_REG & (1<<GAS_SENSOR_PIN_NUM))==0)){
		res=1;
	}
	return res; 
}


uint8_t Up_Button_Clicked(void){
		uint8_t res=0;
		if(((UP_BUTTON_DATA_REG & (1<<UP_BUTTON_PIN_NUM))==0)){
			res=1;
		}
		return res;
}

uint8_t Down_Button_Clicked(void){
	uint8_t res=0;
	if(((DOWN_BUTTON_DATA_REG & (1<<DOWN_BUTTON_PIN_NUM))==0)){
		res=1;
	}
	return res;
}

uint8_t Set_Button_Clicked(void){
	uint8_t res=0;
	if(((SET_BUTTON_DATA_REG & (1<<SET_BUTTON_PIN_NUM))==0)){
		res=1;
	}
	return res;
}

uint8_t FS_Read(void){
	uint8_t res=0;
	if(((FLAME1_SENSOR_DATA_REG & (1<<FLAME1_SENSOR_PIN_NUM))==0)){
		res=1;
	}
	return res;
}

uint8_t OW_Read(void){
	uint8_t res=0;
	if(((OW_DATA_REG & (1<<OW_PIN_NUM))==0)){
		res=1;
	}
	return res;
}


uint8_t RESET_Read(void){
	return 1;
}





// void adc_init(void)
// {
// 	ADMUX = (1<<REFS0);     //select AVCC as reference
// 	ADSRA = (1<<ADEN) | 7;  //enable and prescale = 128 (16MHz/128 = 125kHz)
// }
// 
// int readAdc(char chan)
// {
// 	ADMUX = (1<<REFS0) | (chan & 0x0f);  //select input and ref
// 	ADSRA |= (1<<ADSC);                 //start the conversion
// 	while (ADSRA & (1<<ADSC));          //wait for end of conversion
// 	return ADCW;
//}