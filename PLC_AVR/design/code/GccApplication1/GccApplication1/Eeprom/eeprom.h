/*
 * eeprom.h
 *
 * Created: 7/19/2018 1:09:49 PM
 *  Author: Mohab
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include <stdint.h>

void eeprom_write(uint8_t, uint8_t ucData);
uint8_t eeprom_read(uint8_t uiAddress);
void eeprom_write_16b(uint8_t address, uint16_t value);
uint16_t eeprom_read_16b(uint8_t address);
#endif /* EEPROM_H_ */