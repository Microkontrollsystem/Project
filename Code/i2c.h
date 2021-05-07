/* Heder file for I2C */

#include <avr/io.h>

#define  F_CPU 16000000UL

void i2c_init(){
	TWBR = 0x62;		// Baudrate er satt ved å beregne
	TWCR = (1<<TWEN);	//Enable I2C
	TWSR = 0x00;		// Prescaler satt til 1

}
	// Starttilstand
void i2c_start(){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);	// starttilstand
	while (!(TWCR & (1<<TWINT)));			// sjekk for starttilstand

}
		//I2C stop condition
void i2c_write(char x){				// Cpn esta funcion se escribe en el bus de TWDR
	TWDR = x;						//Move value to I2C
	TWCR = (1<<TWINT) | (1<<TWEN);	// Aktiver I2C og fjern avbrudd
	while  (!(TWCR &(1<<TWINT)));
}

char i2c_read(){
	TWCR  = (1<<TWEN) | (1<<TWINT);	// Aktiver I2C og fjern avbrudd
	while (!(TWCR & (1<<TWINT)));	// Les vellykket med alle data mottatt i TWDR
	return TWDR;
}