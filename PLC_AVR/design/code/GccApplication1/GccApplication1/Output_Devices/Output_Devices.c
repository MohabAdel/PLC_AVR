/*
 * Output_Devices.c
 *
 * Created: 7/17/2018 12:42:29 PM
 *  Author: Mohab
 */ 


#include "Output_Devices.h"
#include "../Shifter/Shifter.h"
#include "../GlobalErrors.h"
uint8_t Output_Byte=0;
//Main valve bit 0
//Valve 1 bit 1
//valve 2 bit 2
//spark bit 3
//alarm indicator bit 4


void Output_Devices_Init(void){
	Shifter_Init();
	ShiftOut_8bit(0,Output_Byte); //init
	Shifter_Output_Latch();
	
	
}

void OH_ON(void){
	if(System_Failure_Flag==0){
		Output_Byte |=1;
	}
}
void OH_OFF(void){
		Output_Byte &=~1;
	
}
void BV1_ON(void){
	if(System_Failure_Flag==0){
		Output_Byte |= (1<<1);
	}
}
void BV1_OFF(void){
		Output_Byte &=~ (1<<1);
	
}

void BV2_ON(void){
	if(System_Failure_Flag==0){
		Output_Byte |= (1<<2);
	}
}
void BV2_OFF(void){
		Output_Byte &=~ (1<<2);
	
}

void M_ON(void){
	if(System_Failure_Flag==0){
		Output_Byte|=(1<<3);
	}
}
void M_OFF(void){
		Output_Byte&=~(1<<3);
	
}

void Z_ON(void){
	if(System_Failure_Flag==0){
		Output_Byte|=(1<<4);
	}
}
void Z_OFF(void){
		Output_Byte&=~(1<<4);
	
}

void AL_ON(void){
	Output_Byte |=(1<<5);
}
void AL_OFF(void){
	if (System_Failure_Flag==0){
		Output_Byte &=~(1<<5);
	}
	
}

void Output_Off_ALARM_ON(void){
	Output_Byte=0;
	AL_ON();
}


void Output_Devices_Write(void){
	
		ShiftOut_8bit(0,Output_Byte);
		Shifter_Output_Latch();
	
}
void Output_ALL_OFF(void){
	Output_Byte=0;
}







