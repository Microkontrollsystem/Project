// fil avr_adc.h
// kort bibliotek for analog til digital omformer (ADC)

#ifndef AVR_ADC_H_
#define AVR_ADC_H_

#include <avr/io.h>



/** @name ADC_PINS
 *  Options for ADC Pin / Analog Input Channel
 */
/**@{*/
#define ADC0 0x00 /**< Sets the Analog Input Channel of ADC to : ADC0 Pin */
#define ADC1 0x01 /**< Sets the Analog Input Channel of ADC to : ADC1 Pin */
#define ADC2 0x02 /**< Sets the Analog Input Channel of ADC to : ADC2 Pin */
#define ADC3 0x03 /**< Sets the Analog Input Channel of ADC to : ADC3 Pin */
#define ADC4 0x04 /**< Sets the Analog Input Channel of ADC to : ADC4 Pin */
#define ADC5 0x05 /**< Sets the Analog Input Channel of ADC to : ADC5 Pin */
#define ADC6 0x06 /**< Sets the Analog Input Channel of ADC to : ADC6 Pin */
#define ADC7 0x07 /**< Sets the Analog Input Channel of ADC to : ADC7 Pin */
#define ADC8 0x08 /**< Sets the Analog Input Channel of ADC to : ADC8 Pin */
#define VBG  0x0E /**< Sets the Analog Input Channel of ADC to : Internal VGB 1.1V */
#define GND  0x0F /**< Sets the Analog Input Channel of ADC to : Internal GND 0.0V */
#define ADC_PIN_MASK 0x0F /**< Mask for Analog Input Channel */
/**@}*/

/** @name ADC_VOLTAGE_REFERENCE
 *  Options for ADC Voltage Reference 
 */
/**@{*/
#define ADC_VOLTAGE_REFERENCE_AREF (0<<REFS0)              /**< Sets the VREF of ADC to : External AREF Pin, Internal VREF turned off */
#define ADC_VOLTAGE_REFERENCE_AVCC (1<<REFS0)              /**< Sets the VREF of ADC to : AVCC with external capacitor at AREF Pin */
#define ADC_VOLTAGE_REFERENCE_IREF (1<<REFS1) | (1<<REFS0) /**< Sets the VREF of ADC to : Internal 1.1V VREF with external capacitor at AREF Pin */
#define ADC_VOLTAGE_REFERENCE_MASK (1<<REFS1) | (1<<REFS0) /**< Mask for ADC Voltage Reference */
/**@}*/

/** @name ADC_PRESCALER
 *  Options for ADC Input Clock Prescaler
 */
/**@{*/
#define ADC_PRESCALER_2    (1<<ADPS0)                           /**< Sets the ADC Clock Source to : F_CPU/2 : System Clock Divided by 2 */
#define ADC_PRESCALER_4    (1<<ADPS1)                           /**< Sets the ADC Clock Source to : F_CPU/4 : System Clock Divided by 4 */
#define ADC_PRESCALER_8    (1<<ADPS1) | (1<<ADPS0)              /**< Sets the ADC Clock Source to : F_CPU/8 : System Clock Divided by 8 */
#define ADC_PRESCALER_16   (1<<ADPS2)                           /**< Sets the ADC Clock Source to : F_CPU/16 : System Clock Divided by 16 */
#define ADC_PRESCALER_32   (1<<ADPS2) | (1<<ADPS0)              /**< Sets the ADC Clock Source to : F_CPU/32 : System Clock Divided by 32 */
#define ADC_PRESCALER_64   (1<<ADPS2) | (1<<ADPS1)              /**< Sets the ADC Clock Source to : F_CPU/64 : System Clock Divided by 64 */
#define ADC_PRESCALER_128  (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0) /**< Sets the ADC Clock Source to : F_CPU/128 : System Clock Divided by 128 */
#define ADC_PRESCALER_MASK (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0) /**< Mask for ADC Prescaler */
/**@}*/

/** @name ADC_TEMP
 *  Constants for the Chip's Temperature Calculation
 */
/**@{*/
#define ADC_TEMP_TOS 0x01
#define ADC_TEMP_K   0x01
/**@}*/

/** @name ADC_CONFIGURATION_DATA
 *  Structure used to Configure ADC
 */
/**@{*/
typedef struct ADC_ConfigData
{
	uint8_t VoltageReference;
	uint8_t Prescaler;
} ADC_ConfigData;
/**@}*/

/** @name ADC_INTERRUPT_CONTROL
 *  Public Functions to Control ADC Interrupts
 */
/**@{*/
inline void ADC_DisableInterrupt();
inline void ADC_EnableInterrupt();
/**@}*/

/** @name ADC_CONFIGURATION_AND_CONTROL
 *  Public Functions to Control and Configure ADC
 */
/**@{*/
inline void ADC_Init(ADC_ConfigData Data);
inline void ADC_DeInit();
/**@{*/

/** @name ADC_READ
 *  Public Functions to Read Analog Data from ADC
 */
/**@{*/
uint16_t ADC_ReadPin(uint8_t Pin);
uint16_t ADC_ReadTemperature();
/**@{*/

/**
 *  \brief Public Function to Disable ADC Interrupt
 *  
 *  \return void
 */
inline void ADC_DisableInterrupt()
{
	
}

/**
 *  \brief Public Function to Enable ADC Interrupt
 *  
 *  \return void
 */
inline void ADC_EnableInterrupt()
{
	
} 

/**
 *  \brief Public Function to Configure and Initialize ADC
 *  
 *  \param [in] Data : Structure Variable containing ADC Configuration Data
 *  \return void
 *
 *	ADC_ConfigData ADC_Data;
 *	ADC_Data.Prescaler = ADC_PRESCALER_128;
 *	ADC_Data.VoltageReference = ADC_VOLTAGE_REFERENCE_AVCC;
 */
inline void ADC_Init(ADC_ConfigData Data)
{
	ADMUX  = Data.VoltageReference | VBG; // Set Voltage Reference and Connect the ADC H/W to GND
	ADCSRA =  (1<<ADEN) | Data.Prescaler;
}

/**
 *  \brief Public Function to De-Initialize ADC
 *  
 *  \return void
 */
inline void ADC_DeInit()
{
	ADMUX  = 0x00;
	ADCSRA = 0x00;
	ADC_DisableInterrupt();
}

/**
 *  \brief Public Function to Read Analog Voltage in Digital Value from a Pin / Analog Input Channel
 *  
 *  \param [in] Pin : Sets the Analog Input Channel for Analog to Digital Converter
 *  \return Converted Digital Value for Input Analog Voltage at Selected Pin / Analog Input Channel
 */
uint16_t ADC_ReadPin(uint8_t Pin)
{
	uint8_t Low, High;
	ADMUX = (ADMUX & (ADC_VOLTAGE_REFERENCE_MASK)) | Pin;
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	Low  = ADCL;
	High = ADCH;
	return (High<<8) | Low;
}

/**
 *  \brief Public Function to Read the Chip's Temperature in Degree C
 *  
 *  \return Digital Value of Temperature in Degree C
 */
uint16_t ADC_ReadTemperature()
{
	uint8_t Low, High;
	ADMUX = ADC_VOLTAGE_REFERENCE_IREF | ADC8;
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	Low  = ADCL;
	High = ADCH;
	return (((High<<8) | Low) - ADC_TEMP_TOS) / ADC_TEMP_K;
}

#endif /* AVR_ADC_H_ */