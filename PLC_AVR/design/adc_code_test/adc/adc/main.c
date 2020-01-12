







#ifndef F_CPU
#define  F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>


 
#define ADC_PIN
#define	LED_PIN

void ADC_Init(void)
{
	DDRC = 0x00;
	DDRB |=(1<<4);
	ADMUX = 0x04;
	ADCSRA = 0xDF; 
	sei();
}

uint32_t data=0;

uint8_t converted=0;
ISR(ADC_vect)
{
	//static  uint16_t count=0;
	uint32_t an_data=0;
	an_data = (ADCL) | (ADCH << 8);
	data =  3300 * an_data/1024;
	converted=1;
	
	//count++;
}
	




int main(){
	ADC_Init();
	while (1){
		if(converted){
			ADCSRA |= (1<<4)|(1<<6);
			converted=0;
		}
		if(data>=3000){
			PORTB |= (1<<4);
		}
		if(data<3000){
			PORTB &=~(1<<4);
		}
		
		//_delay_ms(100);		
	}
	return 0;
}