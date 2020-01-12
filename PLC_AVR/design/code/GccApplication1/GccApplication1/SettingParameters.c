/*
 * SettingParameters.c
 *
 * Created: 7/18/2018 4:44:31 PM
 *  Author: Mohab
 */ 


#include <stdint.h>
#include "Eeprom/eeprom.h"
#include "SettingParameters.h"
uint8_t Internal_Temperature_Used=0;	//p1
uint8_t Pre_Ignition_On=0;				//p2
uint8_t Post_Ignition_On=0;				//p3 
uint16_t Time_Before_Gas_Valve_2=0;		//p4
uint16_t Desired_Tempreature=0;		    //p5							
 
static uint8_t System_mode=0;//p6
 
 void System_Operating_Mode_Change(uint8_t mode){
	 System_mode = mode;
 }
 
 uint8_t Get_System_Operation_Mode(void){
	 return System_mode; //0 normal //1 configuration mode
 }
 
 
 
 void System_Parameters_Get_Values(void){
	Internal_Temperature_Used= eeprom_read(Internal_Temperature_Used_EEPROM_ADDRESS);
	Pre_Ignition_On = eeprom_read(Pre_Ignition_On_EEPROM_ADDRESS);
	Post_Ignition_On = eeprom_read(Post_Ignition_On_EEPROM_ADDRESS);
	Time_Before_Gas_Valve_2= eeprom_read_16b(Time_Before_Gas_Valve_2_EEPROM_ADDRESS);
	Desired_Tempreature= eeprom_read_16b(Desired_Tempreature_EEPROM_ADDRESS);
 }
 
 
 void System_Parameters_Set(g_SystemParameters Sparameter,uint16_t value){
	 if(Sparameter == P1_Internal_Temperature_Used){
		 eeprom_write(Internal_Temperature_Used_EEPROM_ADDRESS,value&0x0F);
	 }
	 else if(Sparameter == P2_Pre_Ignition_On){
		 eeprom_write(Pre_Ignition_On_EEPROM_ADDRESS,value&0x0F);
	 }
	 else if(Sparameter == P3_Post_Ignition_On){
		 eeprom_write(Post_Ignition_On_EEPROM_ADDRESS,value&0x0F);
	 }
	 else if(Sparameter == P4_Time_Before_Gas_Valve_2){
		 eeprom_write_16b(Time_Before_Gas_Valve_2_EEPROM_ADDRESS,value&0x0F);
	 }
	 else if(Sparameter == P5_Desired_Tempreature){
		 eeprom_write_16b(Desired_Tempreature_EEPROM_ADDRESS,value); //16bit
		 
	 }
	 
 }