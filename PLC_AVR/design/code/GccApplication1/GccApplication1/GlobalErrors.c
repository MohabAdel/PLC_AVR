/*
 * GlobalErrors.c
 *
 * Created: 7/17/2018 4:20:50 PM
 *  Author: Mohab
 */ 

#include <stdint.h>

uint8_t Gas_leakage_Flag=0; // 0 no problem  1 problem
uint8_t Over_Tempreature_Flag=0; // 0 no problem  1 problem
uint8_t Tempreature_Sensor_Fail=0;	// 0 no problem  1 problem
									//tempreature negative to ~600
uint8_t System_Failure_Flag=0;