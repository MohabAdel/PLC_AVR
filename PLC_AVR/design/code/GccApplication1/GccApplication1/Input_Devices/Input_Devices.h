/*
 * Input_Devices.h
 *
 * Created: 7/17/2018 4:29:27 PM
 *  Author: Mohab
 */ 


#ifndef INPUT_DEVICES_H_
#define INPUT_DEVICES_H_

#include "../System_Configuration.h"
 



#define GAS_SENSOR_DIRECTION_REG   DDRD
#define GAS_SENSOR_DATA_REG		   PIND
#define GAS_SENSOR_PIN_NUM		   3

#define FLAME1_SENSOR_DIRECTION_REG   DDRD
#define FLAME1_SENSOR_DATA_REG		  PIND
#define FLAME1_SENSOR_PIN_NUM		  4

#define OW_DIRECTION_REG  DDRB
#define OW_DATA_REG       PINB
#define OW_PIN_NUM        0


#define MODE_BUTTON_DIRECTION_REG    DDRD
#define MODE_BUTTON_DATA_REG         PIND
#define MODE_BUTTON_PIN_NUM          2

#define UP_BUTTON_DIRECTION_REG      DDRD
#define UP_BUTTON_DATA_REG           PIND
#define UP_BUTTON_PIN_NUM            5

#define SET_BUTTON_DIRECTION_REG    DDRD
#define SET_BUTTON_DATA_REG         PIND
#define SET_BUTTON_PIN_NUM          6

#define DOWN_BUTTON_DIRECTION_REG    DDRD
#define DOWN_BUTTON_DATA_REG         PIND
#define DOWN_BUTTON_PIN_NUM          7

#define TEMPREATURE_SENSOR_DIRECTION_REG   DDRC
#define TEMPREATURE_SENSOR_PIN_NUM         4
#define TEMPREATURE_SENSOR_ANALOG_CHANNEL  ADC4
void Input_Devices_Init(void);
uint8_t Is_Gas_Leakage(void);
uint16_t Read_Tempreature_Sensor(void);

uint8_t Up_Button_Clicked(void);
uint8_t Down_Button_Clicked(void);
uint8_t Set_Button_Clicked(void);
uint8_t FS_Read(void);
uint8_t OW_Read(void);
uint8_t RESET_Read(void);
#endif /* INPUT_DEVICES_H_ */