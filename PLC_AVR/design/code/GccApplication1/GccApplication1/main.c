/*
 * GccApplication1.c
 *
 * Created: 7/15/2018 2:59:36 PM
 * Author : Mohab
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif


 
#include <avr/io.h>
#include <util/delay.h>
#include "Input_Devices/Input_Devices.h"
#include "SevenSegment/SevenSegment.h"
#include "Output_Devices/Output_Devices.h"
#include <avr/interrupt.h>
#include "GlobalErrors.h"
#include "SettingParameters.h"
#include "Eeprom/eeprom.h"


uint16_t blink_freq=600;
uint16_t current_temp=0;
//#include "types/hw_types.h"
#define  button_update_value_rate      100



#define  ERROR_index	     		0
#define  GAS_index					1
#define  P1_index					2
#define  P2_index					3
#define  P3_index					4
#define  ON_index					5
#define  NOMESSAGE_index			6
#define  MODE_index					7
#define  OVERTEMP_index					8 //
#define  TEMPFAIL_index					9
#define  PARAMETER1ERR_index					10
#define  PARAMETER2ERR_index					11
#define  SYS_index								12
#define  NORMAL_index							13
#define  OPERATION_index						14
#define  CONFIGURATION_index					15
#define  CONFIGURATION_index_sh1				16
#define  CONFIGURATION_index_sh2				17
#define  CONFIGURATION_index_sh3				18
#define  CONFIGURATION_index_sh4				19
#define	 CONFIGURATION_index_sh5				20
#define	 CONFIGURATION_index_sh6				21
#define	 CONFIGURATION_index_sh7				22
#define  RUN_index								23
#define  P4_index								24
#define  P5_index								25
#define  RUN_Error_index						26


 char* const System_Messages[]={
	"ERR", //error
	"GAS",
	"P1 ",
	"P2 ",
	"P3 ",
	"ON ",
	"   ",
	"MOD",
	"E1 ",//over temp
	"E2 ",//temperature sensor fail
	"E3 ",//parameter1 value error
	"E4 ",//parameter2 value error
	"SYS",
	"NOR",
	"OP ",
	"CON",
	"ONf",
	"FG ",
	"g  ",
	"   ",
	"  C",
	" CO",
	"CON",
	"RUN",
	"P4 ",
	"P5 ",
	"E5 "
};



// returns 1 if any error occured
uint8_t over_temp_flag=0 , Temp_Sensor_fail_flag=0;
//#include "Shifter/Shifter.h"
void Timer_Init(void);
uint8_t Check_System_failures_Alarm(void);

static uint8_t System_mode=NORMAL_MODE;


struct State{
	uint8_t Parameter_index;	
	uint8_t Current_parameter_data;
	uint32_t Parameter_EEPROM_address;
	uint8_t	 next_parameter[4];
};	typedef struct State STyp;

#define  PARA1	0
#define  PARA2	1
#define  PARA3  2
#define  PARA4  3
#define  PARA5  4

STyp FSM[5]={
	{P1_index,0,Internal_Temperature_Used_EEPROM_ADDRESS,{PARA1,PARA2,PARA5,PARA1}},
	{P2_index,0,Pre_Ignition_On_EEPROM_ADDRESS,{PARA2,PARA3,PARA1,PARA2}},
    {P3_index,0,Post_Ignition_On_EEPROM_ADDRESS,{PARA3,PARA4,PARA2,PARA3}},
    {P4_index,0,Time_Before_Gas_Valve_2_EEPROM_ADDRESS,{PARA4,PARA5,PARA3,PARA4}},
	{P5_index,0,Desired_Tempreature_EEPROM_ADDRESS,{PARA5,PARA1,PARA4,PARA5}}
};


//inital state of parameters
uint8_t Current_state_FSM =0;


static void Debug_Led_Init(void){
	DDRB |=(1<<4);
}
static void Debug_Led_ON(void){
	PORTB |=(1<<4);
}
static void Debug_Led_OFF(void){
	PORTB &=~(1<<4);
}
static void Debug_Led_Toggle(void){
	PORTB ^=(1<<4);
}

static void System_Parameters_Init(void){
	System_Parameters_Get_Values();
	FSM[0].Current_parameter_data=Internal_Temperature_Used;
	FSM[1].Current_parameter_data=Pre_Ignition_On;
	FSM[2].Current_parameter_data=Post_Ignition_On;
	FSM[3].Current_parameter_data=Time_Before_Gas_Valve_2;
	FSM[4].Current_parameter_data=Desired_Tempreature;
}




uint8_t read_up_down_click(void){
	uint8_t res=0;
	if(Up_Button_Clicked()){
		while(Up_Button_Clicked()){}
			res |=(1<<0);
	}
	if(Down_Button_Clicked()){
		while(Down_Button_Clicked()){}
			res |=(1<<1);
		
	}
	return res;
}


/**********************************************************************************************************************/
/********************************************** Configuration_Mode_Run ************************************************/
/**********************************************************************************************************************/

static void Configuration_Mode_Run(void){
	 uint8_t set_button_click_count=0;
	 uint16_t parameter_value_local=0;
	 while(System_mode==CONFIGURATION_MODE){
		
		SevenSegment_Upper_Display(System_Messages[FSM[Current_state_FSM].Parameter_index],0);
		SevenSegment_Lower_Digit_Display(FSM[Current_state_FSM].Current_parameter_data,0);
		parameter_value_local=FSM[Current_state_FSM].Current_parameter_data;
		if(Set_Button_Clicked()){
			while(Set_Button_Clicked()){}//wait for release
				set_button_click_count=1;
				while (set_button_click_count==1)
				{
					//flash lower seven segment flag =1
					SevenSegment_Lower_Start_Blinking();
					if(Up_Button_Clicked()){

						while (Up_Button_Clicked()){ 
							if(parameter_value_local >=999){
								parameter_value_local =999;
								break;
						     }
							 if(FSM[Current_state_FSM].Parameter_index==P1_index || 
									FSM[Current_state_FSM].Parameter_index==P2_index  ||
										FSM[Current_state_FSM].Parameter_index==P3_index ){
								 parameter_value_local=1;
							 }
							 else{
								 parameter_value_local++;_delay_ms(button_update_value_rate);
							 }
							 SevenSegment_Lower_Digit_Display(parameter_value_local,0);
					    }
					}
					if(Down_Button_Clicked()){
						while (Down_Button_Clicked()){ 
							if(parameter_value_local <= 0){
								parameter_value_local =0;
								break;
							}
							 if(FSM[Current_state_FSM].Parameter_index==P1_index ||
									FSM[Current_state_FSM].Parameter_index==P2_index  ||
										FSM[Current_state_FSM].Parameter_index==P3_index ){
								  parameter_value_local=0;
							}
							else{
								parameter_value_local--;_delay_ms(button_update_value_rate);
							}
							SevenSegment_Lower_Digit_Display(parameter_value_local,0);
						}
						
					}
					if(Set_Button_Clicked()){
						while(Set_Button_Clicked()){}
						set_button_click_count=2;
					}
					if(parameter_value_local <= 0){parameter_value_local=0;}
					if(parameter_value_local >= 999){parameter_value_local=999;}
					SevenSegment_Lower_Digit_Display(parameter_value_local,0);
					
				}//end while set ==1
				SevenSegment_Lower_Stop_Blinking();
			//	if(parameter_value_local < 1){parameter_value_local=1;}
				//if(parameter_value_local >999){parameter_value_local=999;}
				FSM[Current_state_FSM].Current_parameter_data = parameter_value_local;
				//write to the eeprom
				if(FSM[Current_state_FSM].Parameter_index==P5_index || FSM[Current_state_FSM].Parameter_index==P4_index ){
					eeprom_write_16b(FSM[Current_state_FSM].Parameter_EEPROM_address,parameter_value_local);
				}
				else{
					eeprom_write(FSM[Current_state_FSM].Parameter_EEPROM_address,parameter_value_local);
				}
				 
		}
		
		
		set_button_click_count=0;
		Current_state_FSM = FSM[Current_state_FSM].next_parameter[read_up_down_click()];
		
	}
	
}


/***********************************************************************************************************************/
/************************************************ Normal Operation *****************************************************/
/***********************************************************************************************************************/
#define  T1      5
#define  TW		 5
uint8_t run_error=0;
uint8_t Normal_Operate(void){
	Output_ALL_OFF();
	uint8_t flag_fail=0;
	uint16_t init_t2=0;  // Safety Time From 85% Of A B
	uint16_t remain_t2=0; // Safety Time From 15% Of A B
	uint16_t local_t1 = T1;
	uint16_t local_t4 = Time_Before_Gas_Valve_2;
	OH_ON();
	for(uint16_t i=0;i<TW;i++){
		  _delay_ms(1000);
		  if(System_mode==CONFIGURATION_MODE)break;
		  SevenSegment_Upper_Digit_Display(Desired_Tempreature,0);
		  SevenSegment_Lower_Digit_Display(current_temp,0);
	}
	//C //start of loop Burner Motor;
	while (local_t1 > 0) {
		SevenSegment_Upper_Digit_Display(Desired_Tempreature,0);
		SevenSegment_Lower_Digit_Display(current_temp,0);
		if(System_mode==CONFIGURATION_MODE)break;
	    _delay_ms(1000);
	    local_t1 --;
	    //A
	    if(OW_Read() == 1){
		    //B
		    M_ON();
			
		    if(FS_Read() == 0){        //set flag and z=1
			    //if(Pre_Ignition_On == 1)
				  //Z_ON();
		    }
		    else{       //flame sensor
			    flag_fail = 1 ;// Fail
			    break;
		    }
	    }
	    else{      //ow==0
		    flag_fail = 1; //Fail
		    break;
	    }
   }//end of the loop
   if(flag_fail==1){
	   
   }
   else{
	   init_t2 = 15 / 100 * T1;
	   remain_t2 = 10 - init_t2 ;
	   BV1_ON();
	   Z_ON();
	   if(System_mode==CONFIGURATION_MODE){}
	   else{
	   _delay_ms(remain_t2);
	   }
	   while (local_t4 > 0 ){
		   SevenSegment_Upper_Digit_Display(Desired_Tempreature,0);
		   SevenSegment_Lower_Digit_Display(current_temp,0);
		   if(System_mode==CONFIGURATION_MODE)break;
		   _delay_ms(1000);
		   local_t4 --;
		   if(flag_fail){
			   break;
		   }
		   else{
			   if (Post_Ignition_On == 1){
				  // Z_ON();
			   }
			   else {
				   Z_OFF();
			   }
			   //E
			   if (remain_t2 > 0){
				   
				   remain_t2--;
				   //F
				   _delay_ms(500); //debug
				   if (FS_Read() == 1){
					   continue;
				   }
				   else{ //fs==0
					   Z_ON();
				   }
			   }
			   //G
			   else{
				   _delay_ms(500); //debug
				   if(FS_Read() == 1){
					   continue;
				   }
				   else{
					   flag_fail = 1;
					   break;
				   }
			   }
			   
		   }

		   

	   }
	   //End of loop
	   if(flag_fail){
	    }
   		//R
		   run_error=1;
	   	else{
		   BV2_ON();
		   while(FS_Read() == 1){
		   		   SevenSegment_Upper_Digit_Display(Desired_Tempreature,0);
		   		   SevenSegment_Lower_Digit_Display(current_temp,0);
		   }
		   flag_fail = 1;    //Fail();
	    }
	   
   }
    return flag_fail;
   
}
void Fail (void){
	Output_Off_ALARM_ON();
	SevenSegment_Upper_Display(System_Messages[RUN_Error_index],0);
	SevenSegment_Lower_Display(System_Messages[NOMESSAGE_index],0);
	
	for (uint8_t i=50 ; i>0;i--)
	{
		SevenSegment_Upper_Display(System_Messages[RUN_Error_index],0);
		SevenSegment_Lower_Digit_Display(i,0);
		_delay_ms(1000);
	}
	while (Up_Button_Clicked() == 0);
	
}

/***********************************************************************************************************************/
/************************************************** Main Function ******************************************************/
/***********************************************************************************************************************/
uint8_t System_ON_Run=0;

int main(void)
{
    /* Replace with your application code */
	//System_Parameters_Init();
	Debug_Led_Init();
	Input_Devices_Init();
	SevenSegment_Init();
	System_Parameters_Init();
 	Output_Devices_Init();
	 _delay_ms(1000);
	 for (uint8_t i=0;i<5;i++)
	 {
		 current_temp=Read_Tempreature_Sensor();
	 }
	 
	Timer_Init();
	SevenSegment_Upper_Display(System_Messages[SYS_index],0);
	SevenSegment_Lower_Display(System_Messages[ON_index],0);
 	_delay_ms(2500);    
	uint8_t res=0;
    while (1) 
    { 		
		if(System_mode==NORMAL_MODE){
			blink_freq =600;
			if(!over_temp_flag){
				SevenSegment_Upper_Display(System_Messages[RUN_index],0);
				SevenSegment_Lower_Display(System_Messages[NOMESSAGE_index],0);
				_delay_ms(2000);
				System_ON_Run=1;
				res=Normal_Operate();
				if(res)Fail();
// 			while (System_mode==NORMAL_MODE)
// 			{
// 				//current_temp = Read_Tempreature_Sensor();
// 				SevenSegment_Upper_Digit_Display(Desired_Tempreature,0);
// 				SevenSegment_Lower_Digit_Display(current_temp,0);
// 				_delay_ms(500);
// 			}

		}

		}
		else if(System_mode==CONFIGURATION_MODE){
			System_ON_Run=0;
			SevenSegment_Lower_Stop_Blinking();
			Output_ALL_OFF();
			blink_freq =350;
			for (uint8_t j=0;j<2;j++)
			{
					for(uint8_t i=0;i<8;i++){
						SevenSegment_Upper_Display(System_Messages[CONFIGURATION_index+i],0);
						SevenSegment_Lower_Display(System_Messages[OPERATION_index],0);
						_delay_ms(300);
					}
			}
			SevenSegment_Upper_Display(System_Messages[CONFIGURATION_index],0);
			_delay_ms(2000);
			Configuration_Mode_Run();
			Internal_Temperature_Used = FSM[PARA1].Current_parameter_data;//p1
			Pre_Ignition_On = FSM[PARA2].Current_parameter_data;//p2
			
			Post_Ignition_On = FSM[PARA3].Current_parameter_data;//p3
			Time_Before_Gas_Valve_2 = FSM[PARA4].Current_parameter_data;//p4
			Desired_Tempreature = FSM[PARA5].Current_parameter_data;//p5
		}
    }
}










uint8_t Check_System_failures(void){
	uint8_t res=0;
	//static uint16_t Gas_tolerance=0;
	if(Gas_leakage_Flag || Is_Gas_Leakage()){
		//SevenSegment_Upper_Display(System_Messages[GAS_index],1);
		//SevenSegment_Lower_Display(System_Messages[ERROR_index],1);
		res=1;
		Gas_leakage_Flag=1;
	}
    
	if(current_temp> Desired_Tempreature+10 || current_temp<5){
		if(current_temp>Desired_Tempreature+10){ //over temp
			//SevenSegment_Upper_Display(System_Messages[OVERTEMP_index],1);
			//SevenSegment_Lower_Digit_Display(current_temp,1);
			over_temp_flag=1;
			res=1;
			
		}
		else{
			over_temp_flag=0;
		}
		if(current_temp>500 || current_temp<5){  //sensor fail
			
		//	SevenSegment_Upper_Display(System_Messages[TEMPFAIL_index],0);
		//	SevenSegment_Lower_Display(System_Messages[ERROR_index],0);
			Temp_Sensor_fail_flag=1;
			res=1;
		}
		else{
			Temp_Sensor_fail_flag=0;
		}
		//res=1;
		
	}
	return res;

}














#define TWO_MILLIS  131
#define THREE_MILLIS 69
#define MAX_MILLIS   1



void Timer_Init(void){
	TCCR0A = 0x00;
	TCCR0B = 0x04;
	TCNT0 =MAX_MILLIS;
	TIMSK0 |=0x01;
	sei();
}




ISR(TIMER0_OVF_vect){ //2ms
	static int count_1=0;
	count_1++;
	
// 	if(System_Failure_Flag==0){
// 		//check return output 0 to actuators but 1 to alarm
// 		if (Check_System_failures()){
// 			//Output_Off_ALARM_ON();
// 			System_Failure_Flag=1; //lock all outputs and display no update data
// 		}
// 	}
// 	if(System_Failure_Flag==1){
// 		Output_Off_ALARM_ON();
// 	}
	if (count_1 %30==0)
	{
		current_temp=Read_Tempreature_Sensor();
	}
// 	if(System_mode ==NORMAL_MODE && System_ON_Run){

// 	}
	Check_System_failures();
	if(Gas_leakage_Flag){
		System_Failure_Flag=1;
		SevenSegment_Upper_Display(System_Messages[GAS_index],1);
		SevenSegment_Lower_Display(System_Messages[ERROR_index],1);
		Output_Off_ALARM_ON();
		
	}
	else if(run_error){
		Output_Off_ALARM_ON();
	}
	else if(over_temp_flag && System_mode==NORMAL_MODE){
		//System_Failure_Flag=0;
		SevenSegment_Upper_Display(System_Messages[OVERTEMP_index],0);
		SevenSegment_Lower_Digit_Display(current_temp,0);
		Output_ALL_OFF();
		over_temp_flag=0;		
	}
	else{
		
		System_Failure_Flag=0;
	}
// 	if(Temp_Sensor_fail_flag){
// 		SevenSegment_Upper_Display(System_Messages[TEMPFAIL_index],1);
// 		SevenSegment_Lower_Display(System_Messages[ERROR_index],1);
// 		Output_Off_ALARM_ON();
// 		Temp_Sensor_fail_flag=0;
// 	}
	System_mode=Get_System_Operation_Mode();
	Output_Devices_Write();
	To_Display();

	if(count_1 == blink_freq){
		Debug_Led_Toggle();
		
		count_1 =0;
	}
	TIFR0 |=0x01;
	TCNT0 = MAX_MILLIS;
	
}
