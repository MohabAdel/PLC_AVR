/*
 * eeprom.c
 *
 * Created: 7/19/2018 1:08:00 PM
 *  Author: Mohab
 */ 






#include <avr/io.h>
#include <stdint.h>

void eeprom_write(uint8_t uiAddress, uint8_t ucData) {
	while(EECR & (1 << EEPE));
	EEARL = uiAddress;
	EEDR = ucData;
	EECR |= 1 << EEMPE;
	EECR |= 1 << EEPE;
}


uint8_t eeprom_read(uint8_t uiAddress) {
	while (EECR & (1 << EEPE));
	EEARL = uiAddress;
	EECR |= (1 << EERE);
	return EEDR;
}

uint16_t eeprom_read_16b(uint8_t address){
	// multiply address by 2 to convert from 8 to 16
	address *= 2;
	uint16_t value;
	uint8_t bvalH, bvalL;
	/*High byte in address and low bye in the next address */
	bvalH = eeprom_read(address);
	bvalL = eeprom_read(address+1);
	
	value = (bvalH<<8)+bvalL;             // 16 bits address
	return value;
}

void eeprom_write_16b(uint8_t address, uint16_t value){
	// multiply address by 2 to convert from 8 to 16
	address *= 2;
	/*double check for data change compare to the previous and compare to saved*/
	static uint16_t oldValue = 0;
	 oldValue = !value;
	static uint16_t oldAddress = 0;
	 oldAddress = !address;
	if( !((value == oldValue) && (address == oldAddress)) ){
		// Read first if the same do nothing
		uint16_t savedValue = eeprom_read_16b(address);
		if(savedValue != value){
			uint8_t bvalH, bvalL;
			bvalL = value&0xFF;                    // 8 bits value Low
			bvalH = (value>>8)&0xFF;
			/*High byte in address and low bye in the next address */
			eeprom_write(address, bvalH);
			eeprom_write(address+1, bvalL);
		}
		oldValue = value;
		oldAddress = address;
	}
}