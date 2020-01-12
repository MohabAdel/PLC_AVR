/*
 * SettingParameters.h
 *
 * Created: 7/19/2018 11:24:43 AM
 *  Author: Mohab
 */ 


#ifndef SETTINGPARAMETERS_H_
#define SETTINGPARAMETERS_H_

#define Internal_Temperature_Used_EEPROM_ADDRESS			  0 //p1
#define Pre_Ignition_On_EEPROM_ADDRESS						  1 //p2
#define Post_Ignition_On_EEPROM_ADDRESS						  2 //p3
#define Time_Before_Gas_Valve_2_EEPROM_ADDRESS				  3 //p4 //4 also 16 bit
#define Desired_Tempreature_EEPROM_ADDRESS					  10 //p5 //6 also 16 bit 

#define  CONFIGURATION_MODE 1
#define	 NORMAL_MODE 0
typedef enum{
	P1_Internal_Temperature_Used=0,
	P2_Pre_Ignition_On,
	P3_Post_Ignition_On,
	P4_Time_Before_Gas_Valve_2,
	P5_Desired_Tempreature

}g_SystemParameters;

extern uint8_t Internal_Temperature_Used;
extern uint8_t Pre_Ignition_On;
extern uint8_t Post_Ignition_On;//T1 //open main gas
extern uint16_t Time_Before_Gas_Valve_2; //TW
extern uint16_t Desired_Tempreature;
void System_Operating_Mode_Change(uint8_t mode);
uint8_t Get_System_Operation_Mode(void); 
void System_Parameters_Get_Values(void);
#endif /* SETTINGPARAMETERS_H_ */