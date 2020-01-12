/*
 * hw_types.h
 *
 * Created: 7/15/2018 5:01:20 PM
 *  Author: Mohab
 */ 


#ifndef HW_TYPES_H_
#define HW_TYPES_H_

#define HW_WRITE_REG_8BIT(reg,value) (reg = value )
#define HW_READ_REG_8BIT(reg) (reg)

#define HW_WRITE_REG8_MASKED(reg , value , mask) HW_WRITE_REG_8BIT(reg,\
											(HW_READ_REG_8BIT(reg) & ~mask)|(value & mask))


#define HW_SET_BIT(reg,bit) \
	(HW_READ_REG_8BIT(reg) |=(1<<bit))
	
#define HW_RESET_BIT(reg,bit) \
	(HW_READ_REG_8BIT(reg) &=~(1<<bit))


#endif /* HW_TYPES_H_ */