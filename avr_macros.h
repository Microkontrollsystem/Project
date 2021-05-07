// file avr_macros.h
// brief Library for all common Macro Definition

#ifndef AVR_MACROS_H_
#define AVR_MACROS_H_



#ifndef F_CPU
#define F_CPU     16000000UL /**< AVR CPU Frequency in Hz */
#endif

#define F_50Hz    50UL		 /**< Time Period 20ms */
#define F_100Hz   100UL		 /**< Time Period 10ms */
#define F_200Hz   200UL		 /**< Time Period 5ms */
#define F_500Hz   500UL		 /**< Time Period 2ms */
#define F_1kHz    1000UL	 /**< Time Period 1ms */
#define F_5kHz    5000UL	 /**< Time Period 200us */
#define F_10kHz   10000UL	 /**< Time Period 100us */
#define F_20kHz   20000UL	 /**< Time Period 50us */
#define F_40kHz   40000UL	 /**< Time Period 25us */
#define F_50kHz   50000UL	 /**< Time Period 20us */
#define F_100kHz  100000UL	 /**< Time Period 10us */
#define F_200kHz  200000UL	 /**< Time Period 5us */
#define F_500kHz  500000UL	 /**< Time Period 2us */
#define F_1000kHz 1000000UL	 /**< Time Period 1us */
#define F_1Mhz    1000000UL  /**< Time Period 1us */

#define F_62500Hz 62500UL	 /**< Time Period 16us */
#define F_7812Hz  7812UL	 /**< Time Period 128us */
#define F_976Hz   976UL		 /**< Time Period 1024us */
#define F_244Hz   244UL		 /**< Time Period 4098us */
#define F_61Hz    61UL       /**< Time Period 16393us */

#define ENABLE  0x01
#define DISABLE 0x00

#define BUSY    2
#define ERROR   1
#define OK      0

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~(1<<bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |=  (1<<bit))

#define SETBIT(ADDRESS,BIT)   (ADDRESS |=  (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define FLIPBIT(ADDRESS,BIT)  (ADDRESS ^=  (1<<BIT))
#define CHECKBIT(ADDRESS,BIT) (ADDRESS  &  (1<<BIT))

#endif /* AVR_MACROS_H_ */