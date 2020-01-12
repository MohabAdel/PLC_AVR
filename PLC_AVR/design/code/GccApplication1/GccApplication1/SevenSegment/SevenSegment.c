/*
 * SevenSegment.c
 *
 * Created: 7/15/2018 3:53:32 PM
 *  Author: Mohab
 */ 



#include "SevenSegment.h"
#include "../Shifter/Shifter.h"
#include "../GlobalErrors.h"
static uint8_t Lower_Flash=0;
static uint8_t UpdateShifter(uint8_t nShifted);
 const uint8_t characterArray[] = {
	 //  ABCDEFG  Segments      7-segment map:
	 0b11111100, // 0   "0"          AAA 11101110
	 0b01100000, // 1   "1"         F   B
	 0b11011010, // 2   "2"         F   B
	 0b11110010, // 3   "3"          GGG
	 0b01100110, // 4   "4"         E   C
	 0b10110110, // 5   "5"         E   C
	 0b10111110, // 6   "6"          DDD
	 0b11100000, // 7   "7"
	 0b11111110, // 8   "8"
	 0b11110110, // 9   "9"
	 0b11101110, // 10  "A" 11101110
	 0b00111110, // 11  "b"
	 0b10011100, // 12  "C"
	 0b01111010, // 13  "d"
	 0b10011110, // 14  "E"
	 0b10001110, // 15  "F"
	 0b00000000, // 16  NO DISPLAY
	 0b00000000, // 17  NO DISPLAY
	 0b00000000, // 18  NO DISPLAY
	 0b00000000, // 19  NO DISPLAY
	 0b00000000, // 20  NO DISPLAY
	 0b00000000, // 21  NO DISPLAY
	 0b00000000, // 22  NO DISPLAY
	 0b00000000, // 23  NO DISPLAY
	 0b00000000, // 24  NO DISPLAY
	 0b00000000, // 25  NO DISPLAY
	 0b00000000, // 26  NO DISPLAY
	 0b00000000, // 27  NO DISPLAY
	 0b00000000, // 28  NO DISPLAY
	 0b00000000, // 29  NO DISPLAY
	 0b00000000, // 30  NO DISPLAY
	 0b00000000, // 31  NO DISPLAY
	 0b00000000, // 32  ' '
	 0b00000000, // 33  '!'  NO DISPLAY
	 0b01000100, // 34  '"'
	 0b00000000, // 35  '#'  NO DISPLAY
	 0b00000000, // 36  '$'  NO DISPLAY
	 0b00000000, // 37  '%'  NO DISPLAY
	 0b00000000, // 38  '&'  NO DISPLAY
	 0b01000000, // 39  '''
	 0b10011100, // 40  '('
	 0b11110000, // 41  ')'
	 0b00000000, // 42  '*'  NO DISPLAY
	 0b00000000, // 43  '+'  NO DISPLAY
	 0b00001000, // 44  ','
	 0b00000010, // 45  '-'
	 0b00000000, // 46  '.'  NO DISPLAY
	 0b00000000, // 47  '/'  NO DISPLAY
	 0b11111100, // 48  '0'
	 0b01100000, // 49  '1'
	 0b11011010, // 50  '2'
	 0b11110010, // 51  '3'
	 0b01100110, // 52  '4'
	 0b10110110, // 53  '5'
	 0b10111110, // 54  '6'
	 0b11100000, // 55  '7'
	 0b11111110, // 56  '8'
	 0b11110110, // 57  '9'
	 0b00000000, // 58  ':'  NO DISPLAY
	 0b00000000, // 59  ';'  NO DISPLAY
	 0b00000000, // 60  '<'  NO DISPLAY
	 0b00000000, // 61  '='  NO DISPLAY
	 0b00000000, // 62  '>'  NO DISPLAY
	 0b00000000, // 63  '?'  NO DISPLAY
	 0b00000000, // 64  '@'  NO DISPLAY
	 0b11101110, // 65  'A'
	 0b00111110, // 66  'b'
	 0b10011100, // 67  'C'
	 0b01111010, // 68  'd'
	 0b10011110, // 69  'E'
	 0b10001110, // 70  'F'
	 0b10111100, // 71  'G'
	 0b01101110, // 72  'H'
	 0b01100000, // 73  'I'
	 0b01110000, // 74  'J'
	 0b00000000, // 75  'K'  NO DISPLAY
	 0b00011100, // 76  'L'
	 0b00000000, // 77  'M'  NO DISPLAY
	 0b11101100, // 78  'n'
	 0b11111100, // 79  'O'
	 0b11001110, // 80  'P'
	 0b11100110, // 81  'q'
	 0b10001100, // 82  'r'
	 0b10110110, // 83  'S'
	 0b00011110, // 84  't'
	 0b01111100, // 85  'U'
	 0b00000000, // 86  'V'  NO DISPLAY
	 0b00000000, // 87  'W'  NO DISPLAY
	 0b00000000, // 88  'X'  NO DISPLAY
	 0b01110110, // 89  'y'
	 0b00000000, // 90  'Z'  NO DISPLAY
	 0b10011100, // 91  '['
	 0b00000000, // 92  '\'  NO DISPLAY
	 0b11110000, // 93  ']'
	 0b00000000, // 94  '^'  NO DISPLAY
	 0b00010000, // 95  '_'
	 0b00000100, // 96  '`'
	 0b11101110, // 97  'a' SAME AS CAP
	 0b00111110, // 98  'b' SAME AS CAP
	 0b00011010, // 99  'c'
	 0b01111010, // 100 'd' SAME AS CAP
	 0b11011110, // 101 'e'
	 0b10001110, // 102 'F' SAME AS CAP
	 0b10111100, // 103 'G' SAME AS CAP
	 0b00101110, // 104 'h'
	 0b00100000, // 105 'i'
	 0b01110000, // 106 'j' SAME AS CAP
	 0b00000000, // 107 'k'  NO DISPLAY
	 0b01100000, // 108 'l'
	 0b00000000, // 109 'm'  NO DISPLAY
	 0b11101100, // 110 'n' SAME AS CAP
	 0b00111010, // 111 'o'
	 0b11001110, // 112 'p' SAME AS CAP
	 0b11100110, // 113 'q' SAME AS CAP
	 0b10001100, // 114 'r' SAME AS CAP
	 0b10110110, // 115 'S' SAME AS CAP
	 0b00011110, // 116 't' SAME AS CAP
	 0b00111000, // 117 'u'
	 0b00000000, // 118 'b'  NO DISPLAY
	 0b00000000, // 119 'w'  NO DISPLAY
	 0b00000000, // 120 'x'  NO DISPLAY
	 0b00000000, // 121 'y'  NO DISPLAY
	 0b00000000, // 122 'z'  NO DISPLAY
	 0b00000000, // 123 '0b'  NO DISPLAY
	 0b00000000, // 124 '|'  NO DISPLAY
	 0b00000000, // 125 ','  NO DISPLAY
	 0b00000000, // 126 '~'  NO DISPLAY
	 0b00000000, // 127 'DEL'  NO DISPLAY
 };
 
 
typedef struct seventSegments{
	uint8_t S1UPPER;
	uint8_t S2UPPER;
	uint8_t S3UPPER;
	
	uint8_t S1LOWER;
	uint8_t S2LOWER;
	uint8_t S3LOWER;
}g_SevenSegments;


//global segments data
g_SevenSegments Segment_Units={
	.S1LOWER=0,
	.S1UPPER=0,
	.S2LOWER=0,
	.S2UPPER=0,
	.S3LOWER=0,
	.S3UPPER=0
};

void SevenSegment_Init(void){
	Shifter_Init();
	SEVEN_SEGMENT_S1_DIRECTION_REG |= (1<<SEVEN_SEGMENT_S1_PIN_NUM); //output
	SEVEN_SEGMENT_S2_DIRECTION_REG |= (1<<SEVEN_SEGMENT_S2_PIN_NUM); //output
	SEVEN_SEGMENT_S3_DIRECTION_REG |= (1<<SEVEN_SEGMENT_S3_PIN_NUM); //output
}

void SevenSegment_Upper_Digit_Display(uint16_t x,uint8_t Auth){
	if(System_Failure_Flag==0){
		char digit_to_char[3]; //123
		if(x>999)x=999;
		for(uint8_t i=0 ; i<3;i++){
			digit_to_char[0]=(x/100)+'0';
			digit_to_char[1]=(x%100)/10+'0';
			digit_to_char[2]=x%10+'0';
		}
		SevenSegment_Upper_Display(digit_to_char,Auth);
	}
    if( Auth){
		char digit_to_char[3]; //123
		if(x>999)x=999;
		for(uint8_t i=0 ; i<3;i++){
			digit_to_char[0]=(x/100)+'0';
			digit_to_char[1]=(x%100)/10+'0';
			digit_to_char[2]=x%10+'0';
		}
		SevenSegment_Upper_Display(digit_to_char,Auth);		
	}
}




void SevenSegment_Lower_Digit_Display(uint16_t x,uint8_t Auth){
	if(System_Failure_Flag==0){
		char digit_to_char[3]; //123
		if(x>999)x=999;
		for(uint8_t i=0 ; i<3;i++){
			digit_to_char[0]=(x/100)+'0';
			digit_to_char[1]=(x%100)/10+'0';
			digit_to_char[2]=x%10+'0';
			}
		SevenSegment_Lower_Display(digit_to_char,Auth);
	}
	if( Auth){
		char digit_to_char[3]; //123
		if(x>999)x=999;
		for(uint8_t i=0 ; i<3;i++){
			digit_to_char[0]=(x/100)+'0';
			digit_to_char[1]=(x%100)/10+'0';
			digit_to_char[2]=x%10+'0';
		}
		SevenSegment_Lower_Display(digit_to_char,Auth);
	}
}





void SevenSegment_Upper_Display(char* ptoDisplay,uint8_t Auth){
	if(System_Failure_Flag==0){
		uint8_t SHIFTED =UpdateShifter(characterArray[(uint8_t)(*ptoDisplay)]);
		Segment_Units.S1UPPER = SHIFTED;
		ptoDisplay++;
		SHIFTED =UpdateShifter(characterArray[(uint8_t)(*ptoDisplay)]);
		Segment_Units.S2UPPER = SHIFTED;
		ptoDisplay++;
		SHIFTED =UpdateShifter(characterArray[(uint8_t)(*ptoDisplay)]);
		Segment_Units.S3UPPER = SHIFTED;
	}
	if(Auth){
		uint8_t SHIFTED =UpdateShifter(characterArray[(uint8_t)(*ptoDisplay)]);
		Segment_Units.S1UPPER = SHIFTED;
		ptoDisplay++;
		SHIFTED =UpdateShifter(characterArray[(uint8_t)(*ptoDisplay)]);
		Segment_Units.S2UPPER = SHIFTED;
		ptoDisplay++;
		SHIFTED =UpdateShifter(characterArray[(uint8_t)(*ptoDisplay)]);
		Segment_Units.S3UPPER = SHIFTED;
	}
}

void SevenSegment_Lower_Display(char* ptoDisplay,uint8_t Auth){
	if(System_Failure_Flag==0){
		uint8_t SHIFTED =UpdateShifter(characterArray[(uint8_t)(*ptoDisplay)]);
		Segment_Units.S1LOWER = SHIFTED;
		ptoDisplay++;
		SHIFTED =UpdateShifter(characterArray[(uint8_t)(*ptoDisplay)]);
		Segment_Units.S2LOWER = SHIFTED;
		ptoDisplay++;
		SHIFTED =UpdateShifter(characterArray[(uint8_t)(*ptoDisplay)]);
		Segment_Units.S3LOWER = SHIFTED;
	
	}
    if(Auth){
		uint8_t SHIFTED =UpdateShifter(characterArray[(uint8_t)(*ptoDisplay)]);
		Segment_Units.S1LOWER = SHIFTED;
		ptoDisplay++;
		SHIFTED =UpdateShifter(characterArray[(uint8_t)(*ptoDisplay)]);
		Segment_Units.S2LOWER = SHIFTED;
		ptoDisplay++;
		SHIFTED =UpdateShifter(characterArray[(uint8_t)(*ptoDisplay)]);
		Segment_Units.S3LOWER = SHIFTED;		
	}
}


void SevenSegment_Lower_Start_Blinking(void){
		Lower_Flash=1;
}

void SevenSegment_Lower_Stop_Blinking(void){
	Lower_Flash = 0;
}



// calls every 2 ms
void To_Display(void){
	static uint8_t i=0;
	static uint16_t count=0;
	
	static uint8_t S1Lower_Old=0,S2Lower_Old=0,S3Lower_Old=0;

	if(Lower_Flash ==1 && System_Failure_Flag==0){
		count ++;
		if(count==100){
			S1Lower_Old=characterArray[' '];
			S2Lower_Old=characterArray[' '];
			S3Lower_Old=characterArray[' '];
			
		}
		else if(count==200){
			S1Lower_Old = Segment_Units.S1LOWER;
			S2Lower_Old = Segment_Units.S2LOWER;
			S3Lower_Old = Segment_Units.S3LOWER;
			count=0;
		}
	}
	else{
			S1Lower_Old = Segment_Units.S1LOWER;
			S2Lower_Old = Segment_Units.S2LOWER;
			S3Lower_Old = Segment_Units.S3LOWER;
		
	}
	
	uint16_t SEG_UPPER_LOWER[3] ={
		(S1Lower_Old<<8)|Segment_Units.S1UPPER,
		(S2Lower_Old<<8)|Segment_Units.S2UPPER,
		(S3Lower_Old<<8)|Segment_Units.S3UPPER
	};
	ShiftOut_16bit(0,SEG_UPPER_LOWER[i]);
	Shifter_Display_Latch();
	
	if(i==0){
		HW_SET_BIT(SEVEN_SEGMENT_S1_DATA_REG,SEVEN_SEGMENT_S1_PIN_NUM);
		HW_RESET_BIT(SEVEN_SEGMENT_S2_DATA_REG,SEVEN_SEGMENT_S2_PIN_NUM);
		HW_RESET_BIT(SEVEN_SEGMENT_S3_DATA_REG,SEVEN_SEGMENT_S3_PIN_NUM);
	}
	else if(i==1){
		HW_RESET_BIT(SEVEN_SEGMENT_S1_DATA_REG,SEVEN_SEGMENT_S1_PIN_NUM);
		HW_SET_BIT(SEVEN_SEGMENT_S2_DATA_REG,SEVEN_SEGMENT_S2_PIN_NUM);
		HW_RESET_BIT(SEVEN_SEGMENT_S3_DATA_REG,SEVEN_SEGMENT_S3_PIN_NUM);
	}
	else if(i==2){
		HW_RESET_BIT(SEVEN_SEGMENT_S1_DATA_REG,SEVEN_SEGMENT_S1_PIN_NUM);
		HW_RESET_BIT(SEVEN_SEGMENT_S2_DATA_REG,SEVEN_SEGMENT_S2_PIN_NUM);
		HW_SET_BIT(SEVEN_SEGMENT_S3_DATA_REG,SEVEN_SEGMENT_S3_PIN_NUM);
		
	}
	i++;
	if(i==3)i=0;
	
	
}

//interrupt to display
// void Hold_Display(void){
// 	static uint8_t i=0;
// 	
// 		if(i==0){
// 			HW_SET_BIT(SEVEN_SEGMENT_S1_DATA_REG,SEVEN_SEGMENT_S1_PIN_NUM);
// 			HW_RESET_BIT(SEVEN_SEGMENT_S2_DATA_REG,SEVEN_SEGMENT_S2_PIN_NUM);
// 			HW_RESET_BIT(SEVEN_SEGMENT_S3_DATA_REG,SEVEN_SEGMENT_S3_PIN_NUM);
// 		}
// 		else if(i==1){
// 			HW_RESET_BIT(SEVEN_SEGMENT_S1_DATA_REG,SEVEN_SEGMENT_S1_PIN_NUM);
// 			HW_SET_BIT(SEVEN_SEGMENT_S2_DATA_REG,SEVEN_SEGMENT_S2_PIN_NUM);
// 			HW_RESET_BIT(SEVEN_SEGMENT_S3_DATA_REG,SEVEN_SEGMENT_S3_PIN_NUM);
// 		}
// 		else if(i==2){
// 			HW_RESET_BIT(SEVEN_SEGMENT_S1_DATA_REG,SEVEN_SEGMENT_S1_PIN_NUM);
// 			HW_RESET_BIT(SEVEN_SEGMENT_S2_DATA_REG,SEVEN_SEGMENT_S2_PIN_NUM);
// 			HW_SET_BIT(SEVEN_SEGMENT_S3_DATA_REG,SEVEN_SEGMENT_S3_PIN_NUM);
// 			
// 		}
// 		i++;
// 		if(i==3)i=0;
// 		
// 		
// 	
//}




static uint8_t UpdateShifter(uint8_t nShifted){

	uint8_t Shifted =0;

	Shifted = (nShifted&0x01)|
				((nShifted&0x02))|
					((nShifted&0x04)<<4)|
						((nShifted&0x08)<<1)|
							((nShifted&0x10)>>1)|
								((nShifted&0x20)>>3)|
									((nShifted&0x40)<<1)|
										((nShifted&0x80)>>2);


	return Shifted;
}



