

// Merk Dette biblioteket bruker avbruddsbasert ikke-blokkerende overføring



#ifndef AVR_USART_H_
#define AVR_USART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "avr_macros.h"
#include "avr_utils.h"



/** @name USART_MODE
 *  Options for USART Operation Mode
 */
/**@{*/
#define USART_SYNCHRONOUS_CLOCK_PRIORITY 0   /**< USART Clock Priority when Synchronous External Clock is used */
#define USART_MODE_ASYNCHRONOUS (0<<UMSEL00)                                    /**< Sets the USART Mode of Operation to : Asynchronous UART */
#define USART_MODE_SYNCHRONOUS  (1<<UMSEL00) | USART_SYNCHRONOUS_CLOCK_PRIORITY /**< Sets the USART Mode of Operation to : Synchronous USRT */
#define USART_MODE_MASK         (1<<UMSEL00) /**< Mask for USART Mode of Operation */
/**@}*/

/** @name USART_PARITY_BIT
 *  Options for Parity Bit in USART Data Frame
 */
/**@{*/
#define USART_PARITY_BIT_NO   (0<<UPM00)              /**< Sets the Parity Bit of USART Data Frame to : No Parity Bit */
#define USART_PARITY_BIT_EVEN (1<<UPM01)              /**< Sets the Parity Bit of USART Data Frame to : Even Parity Bit */
#define USART_PARITY_BIT_ODD  (1<<UPM01) | (1<<UPM00) /**< Sets the Parity Bit of USART Data Frame to : Odd Parity Bit */
#define USART_PARITY_BIT_MASK (1<<UPM01) | (1<<UPM00) /**< Mask for USART Parity Bit */
/**@}*/

/** @name USART_STOP_BIT
 *  Options for Stop Bit in USART Data Frame
 */
/**@{*/
#define USART_STOP_BIT_ONE  (0<<USBS0) /**< Sets the Stop Bit of USART Data Frame to : One Stop Bit */
#define USART_STOP_BIT_TWO  (1<<USBS0) /**< Sets the Stop Bit of USART Data Frame to : Two Stop Bit */
#define USART_STOP_BIT_MASK (1<<USBS0) /**< Mask for USART Stop Bit */
/**@}*/

/** @name USART_DATA_BIT
 *  Options for Data Bit Length in USART Data Frame
 */
/**@{*/
#define USART_DATA_BIT_FIVE  (0<<UCSZ00)               /**< Sets the Data Bit length of USART Data Frame to : Five Bit */
#define USART_DATA_BIT_SIX   (1<<UCSZ00)               /**< Sets the Data Bit length of USART Data Frame to : Six Bit */
#define USART_DATA_BIT_SEVEN (1<<UCSZ01)               /**< Sets the Data Bit length of USART Data Frame to : Seven Bit */
#define USART_DATA_BIT_EIGHT (1<<UCSZ01) | (1<<UCSZ00) /**< Sets the Data Bit length of USART Data Frame to : Eight Bit */
#define USART_DATA_BIT_MASK  (1<<UCSZ01) | (1<<UCSZ00) /**< Mask for the Data Bit length of USART Data Frame */
/**@}*/

/** @name USART_INTERRUPT
 *  Options for USART Interrupts 
 */
/**@{*/
#define USART_RX_COMPLETE_INTERRUPT         (1<<RXCIE0) /**< USART Interrupt Control Bit : Receive Complete Interrupt */
#define USART_TX_COMPLETE_INTERRUPT         (1<<TXCIE0) /**< USART Interrupt Control Bit : Transmit Complete Interrupt */
#define USART_DATA_REGISTER_EMPTY_INTERRUPT (1<<UDRIE0) /**< USART Interrupt Control Bit : Data Register Empty Interrupt */
/**@}*/

/** @name USART_CONFIGURATION_DATA
 *  Structure used to Configure USART 
 */
/**@{*/
typedef struct USART_ConfigData
{
	uint8_t  UsartMode;
	uint16_t BaudRate;
	uint8_t  ParityBit;
	uint8_t  StopBit;
	uint8_t  DataBit;
} USART_ConfigData;
/**@}*/

/** @name USART_GLOBAL_VARIABLES
 *  Global Variables used by USART Functions
 */
/**@{*/
static volatile uint8_t *USART_TransmitBuffer;           /**< USART Global Variable to store the address of Transmit Buffer used by the ISR */
static volatile uint8_t  USART_TransmitBusyFlag = 0;     /**< USART Global Variable to store the status of USART transmission */
static volatile uint16_t USART_TransmitBufferIndex = 0;  /**< USART Global Variable to store the index of Transmit Buffer used by the ISR */
static volatile uint16_t USART_TransmitBufferLength = 0; /**< USART Global Variable to store the length of Transmit Buffer used by the ISR */

static volatile uint8_t *USART_ReceiveBuffer;            /**< USART Global Variable to store the address of Receive Buffer used by the ISR */
static volatile uint8_t  USART_ReceiveBusyFlag = 0;		 /**< USART Global Variable to store the status of USART reception */
static volatile uint16_t USART_ReceiveBufferIndex = 0;	 /**< USART Global Variable to store the index of Receive Buffer used by the ISR */
static volatile uint16_t USART_ReceiveBufferLength  = 0; /**< USART Global Variable to store the length of Receive Buffer used by the ISR */
/**@}*/

/** @name USART_FLUSH_TRANSMIT_RECEIVE_BUFFER
 *  Public Functions to Flush USART Transmit and Receive Buffer
 */
/**@{*/
void USART_FlushTransmitBuffer();
void USART_FlushReceiveBuffer();
/**@}*/

/** @name USART_TRANSMIT_INTERRUPT_CONTROL
 *  Public Functions to Control USART Transmit Interrupts
 */
/**@{*/
inline void USART_EnableTransmitInterrupt();
inline void USART_DisableTransmitInterrupt();
/**@}*/

/** @name USART_RECEIVE_INTERRUPT_CONTROL
 *  Public Functions to Control USART Receive Interrupts
 */
/**@{*/
inline void USART_EnableReceiveInterrupt();
inline void USART_DisableReceiveInterrupt();
/**@}*/

/** @name USART_CONFIGURATION_AND_CONTROL
 *  Public Functions to Control and Configure USART
 */
/**@{*/
void USART_Init(USART_ConfigData Data);
void USART_DeInit();
/**@}*/

/** @name USART_RECEIVE
 *  Public Functions to Receive Data via USART
 */
/**@{*/
uint8_t USART_ReceiveBlocking();
uint8_t USART_ReceiveByte(uint8_t *Buffer);
uint8_t USART_ReceiveTwoBytes(uint16_t *Buffer);
uint8_t USART_ReceiveBytes(uint8_t *Buffer, uint16_t Size);
uint8_t USART_ReceiveChar(char *Character);
uint8_t USART_ReceiveInt(int *Number, uint8_t Size);
uint8_t USART_ReceiveLong(long *Number, uint8_t Size);
uint8_t USART_ReceiveString(char *Buffer, uint16_t Size);
void USART_WaitTillReceiveFree();
void USART_CancelReceive();
/**@}*/

/** @name USART_TRANSMIT
 *  Public Functions to Transmit Data via USART
 */
/**@{*/
uint8_t USART_TransmitBlocking(uint8_t *Buffer);
uint8_t USART_TransmitByte(uint8_t *Buffer);
uint8_t USART_TransmitTwoBytes(uint16_t *Buffer);
uint8_t USART_TransmitBytes(uint8_t *Buffer, uint16_t Size);
uint8_t USART_TransmitChar(char Character);
uint8_t USART_TransmitInt(int Number);
uint8_t USART_TransmitLong(long Number);
uint8_t USART_TransmitFloat(float Number, uint8_t Precision);
uint8_t USART_TransmitString(char *Buffer);
void USART_WaitTillTransmitFree();
void USART_CancelTransmit();
/**@}*/

/**
 *  \brief Public Function to check USART Transmission Status and wait till USART Transmission is free
 *  
 *  \return void
 */
void USART_WaitTillTransmitFree()
{	
	while (USART_TransmitBusyFlag);
}

/**
 *  \brief Public Function to check USART Reception Status and wait till USART Reception is free
 *  
 *  \return void
 */
void USART_WaitTillReceiveFree()
{	
	while (USART_ReceiveBusyFlag);
}

/**
 *  \brief Public Function to Flush the Receive Buffer
 *  
 *  \return void
 */
void USART_FlushReceiveBuffer()
{	
	for (int i = 0; i < USART_ReceiveBufferLength; i++)
	{
		USART_ReceiveBuffer[i] = 0;
	}
	
	USART_ReceiveBuffer = 0;
	USART_ReceiveBusyFlag = 0;
	USART_ReceiveBufferIndex = 0;
	USART_ReceiveBufferLength = 0;
}

/**
 *  \brief Public Function to Flush the Transmit Buffer
 *  
 *  \return void
 */
void USART_FlushTransmitBuffer()
{
	for (int i = 0; i < USART_TransmitBufferLength; i++)
	{
		USART_TransmitBuffer[i] = 0;
	}
	
	USART_TransmitBuffer = 0;
	USART_TransmitBusyFlag = 0;
	USART_TransmitBufferIndex = 0;
	USART_TransmitBufferLength = 0;
}

/**
 *  \brief Public Function to Enable Transmit Interrupt
 *  
 *  \return void
 */
inline void USART_EnableTransmitInterrupt()
{
	UCSR0B |= USART_DATA_REGISTER_EMPTY_INTERRUPT;
}

/**
 *  \brief Public Function to Disable Transmit Interrupt
 *  
 *  \return void
 */
inline void USART_DisableTransmitInterrupt()
{
	UCSR0B &= ~USART_DATA_REGISTER_EMPTY_INTERRUPT;
}

/**
 *  \brief Public Function to Enable Receive Interrupt
 *  
 *  \return void
 */
inline void USART_EnableReceiveInterrupt()
{
	UCSR0B |= USART_RX_COMPLETE_INTERRUPT;
}

/**
 *  \brief Public Function to Disable Receive Interrupt
 *  
 *  \return void
 */
inline void USART_DisableReceiveInterrupt()
{
	UCSR0B &= ~USART_RX_COMPLETE_INTERRUPT;
}


void USART_Init(USART_ConfigData Data)
{	
	uint16_t USART_BaudPrescaler = 0;
	 
	if (Data.UsartMode == (USART_MODE_ASYNCHRONOUS))
	{
		USART_BaudPrescaler =  (((F_CPU / (Data.BaudRate * 16UL))) - 1);
	} 
	else if (Data.UsartMode == (USART_MODE_SYNCHRONOUS))
	{
		USART_BaudPrescaler =  (((F_CPU / (Data.BaudRate * 2UL))) - 1);
	}
	
	/* Set Baud Rate */
	UBRR0H = USART_BaudPrescaler >> 8;
	UBRR0L = USART_BaudPrescaler;
	
	/* Set Frame Format */
	UCSR0C = Data.UsartMode | Data.ParityBit | Data.StopBit | Data.DataBit;
	
	/* Enable Receiver and Transmitter */
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	
	/* Enable Global Interrupt */
	sei();
}

/**
 *  \brief Public Function to De-Initialize USART 
 *  
 *  \return void
 */
void USART_DeInit()
{
	UCSR0B = 0x00;
	UCSR0C = 0x06;
}

/**
 *  \brief Public Function to Receive 1 Byte of Data in Polling
 *  
 *  \return Received Byte of Data
 */
uint8_t USART_ReceiveBlocking()
{
	uint8_t DataByte;
	while ((UCSR0A & (1<<RXC0)) == 0); // Do nothing until data have been received
	DataByte = UDR0;
	return DataByte;
}

/**
 *  \brief Public Function to Receive 1 Byte of Data
 *  
 *  \param [out] Buffer : Pointer to Memory Location to Store Data
 *  \return BUSY / OK
 */
uint8_t USART_ReceiveByte(uint8_t *Buffer)
{
	while (USART_ReceiveBusyFlag)
	{
		return BUSY;
	}
	USART_ReceiveBuffer = Buffer;
	USART_ReceiveBufferLength = 1;
	USART_ReceiveBufferIndex = 0;
	USART_ReceiveBusyFlag = 1;
	USART_EnableReceiveInterrupt();
	return OK;
}

/**
 *  \brief Public Function to Receive 2 Bytes of Data with LSB Byte First
 *  
 *  \param [out] Buffer : Pointer to Memory Location to Store Data
 *  \return BUSY / OK
 */
uint8_t USART_ReceiveTwoBytes(uint16_t *Buffer)
{
	while (USART_ReceiveBusyFlag)
	{
		return BUSY;
	}
	USART_ReceiveBuffer = (uint8_t*)Buffer;
	USART_ReceiveBufferLength = 2;
	USART_ReceiveBufferIndex = 0;
	USART_ReceiveBusyFlag = 1;
	USART_EnableReceiveInterrupt();
	return OK;
}

/**
 *  \brief Public Function to Receive Multiple Bytes of Data
 *  
 *  \param [out] Buffer : Pointer to Memory Location to Store Data
 *  \param [in] Size : Variable to Store Number of Bytes to Receive
 *  \return BUSY / OK
 */
uint8_t USART_ReceiveBytes(uint8_t *Buffer, uint16_t Size)
{
	if (Size == 0) return ERROR;
	
	while (USART_ReceiveBusyFlag)
	{
		return BUSY;
	}
	USART_ReceiveBuffer = Buffer;
	USART_ReceiveBufferLength = Size;
	USART_ReceiveBufferIndex = 0;
	USART_ReceiveBusyFlag = 1;
	USART_EnableReceiveInterrupt();
	return OK;
}

/**
 *  \brief Public Function to Receive char Data
 *  
 *  \param [out] Character : Pointer to Memory Location to Store char Data
 *  \return BUSY / OK
 */
uint8_t USART_ReceiveChar(char *Character)
{
	while (USART_ReceiveBusyFlag)
	{
		return BUSY;
	}
	USART_ReceiveBuffer = (uint8_t*)Character;
	USART_ReceiveBufferLength = 1;
	USART_ReceiveBufferIndex = 0;
	USART_ReceiveBusyFlag = 1;
	USART_EnableReceiveInterrupt();
	return OK;
}

/**
 *  \brief Public Function to Receive int Data as ASCII
 *  
 *  \param [out] Number : Pointer to Memory Location to Store int Data
 *  \param [in] Size : Variable to Store Number of Digits to Receive
 *  \return BUSY / OK
 */
uint8_t USART_ReceiveInt(int *Number, uint8_t Size)
{
	if (Size == 0) return ERROR;
	
	char localbuffer[7] = {0};
	while (USART_ReceiveBusyFlag)
	{
		return BUSY;
	}
	USART_ReceiveBuffer = (uint8_t*)localbuffer;
	USART_ReceiveBufferLength = Size;
	USART_ReceiveBufferIndex = 0;
	USART_ReceiveBusyFlag = 1;
	USART_EnableReceiveInterrupt();
	while (USART_ReceiveBusyFlag);
	*Number = UTILS_StringToInt(localbuffer);
	return OK;
}

/**
 *  \brief Public Function to Receive long Data as ASCII
 *  
 *  \param [out] Number : Pointer to Memory Location to Store long Data
 *  \param [in] Size : Variable to Store Number of Digits to Receive
 *  \return BUSY / OK
 */
uint8_t USART_ReceiveLong(long *Number, uint8_t Size)
{
	if (Size == 0) return ERROR;
	
	char localbuffer[12] = {0};
	while (USART_ReceiveBusyFlag)
	{
		return BUSY;
	}
	USART_ReceiveBuffer = (uint8_t*)localbuffer;
	USART_ReceiveBufferLength = Size;
	USART_ReceiveBufferIndex = 0;
	USART_ReceiveBusyFlag = 1;
	USART_EnableReceiveInterrupt();
	while (USART_ReceiveBusyFlag);
	*Number = UTILS_StringToLong(localbuffer);
	return OK;
}

/**
 *  \brief Public Function to Receive string Data
 *  
 *  \param [out] Buffer : Pointer to Memory Location to Store string Data
 *  \param [in] Size : Variable to Store Number of Letters to Receive
 *  \return BUSY / OK
 */
uint8_t USART_ReceiveString(char *Buffer, uint16_t Size)
{
	if (Size == 0) return ERROR;
	
	while (USART_ReceiveBusyFlag)
	{
		return BUSY;
	}
	USART_ReceiveBuffer = (uint8_t*)Buffer;
	USART_ReceiveBufferLength = Size;
	USART_ReceiveBufferIndex = 0;
	USART_ReceiveBusyFlag = 1;
	USART_EnableReceiveInterrupt();
	return OK;
}

/**
 *  \brief Public Function to Cancel any Interrupt based Reception
 *  
 *  \return void
 */
void USART_CancelReceive()
{
	USART_DisableReceiveInterrupt();
	USART_ReceiveBusyFlag = 0;
}

/**
 *  \brief Public Function to Transmit 1 Byte of Data in Polling
 *  
 *  \return OK
 */
uint8_t USART_TransmitBlocking(uint8_t *Buffer)
{
	while ((UCSR0A & (1<<UDRE0)) == 0); // Do nothing until UDR is ready
	UDR0 = *Buffer;
	return OK;
}

/**
 *  \brief Public Function to Transmit 1 Byte of Data
 *  
 *  \param [in] Buffer : Pointer to Memory Location to Store Data
 *  \return BUSY / OK
 */
uint8_t USART_TransmitByte(uint8_t *Buffer)
{
	while (USART_TransmitBusyFlag)
	{
		return BUSY;
	}
	USART_TransmitBuffer = Buffer;
	USART_TransmitBufferLength = 1;
	USART_TransmitBufferIndex = 0;
	USART_TransmitBusyFlag = 1;
	USART_EnableTransmitInterrupt();
	return OK;
}

/**
 *  \brief Public Function to Transmit 2 Bytes of Data with LSB Byte First
 *  
 *  \param [in] Buffer : Pointer to Memory Location to Store Data
 *  \return BUSY / OK
 */
uint8_t USART_TransmitTwoBytes(uint16_t *Buffer)
{	
	while (USART_TransmitBusyFlag)
	{
		return BUSY;
	}
	USART_TransmitBuffer = (uint8_t*)Buffer;
	USART_TransmitBufferLength = 2;
	USART_TransmitBufferIndex = 0;
	USART_TransmitBusyFlag = 1;
	USART_EnableTransmitInterrupt();
	return OK;
}

/**
 *  \brief Public Function to Transmit Multiple Bytes of Data
 *  
 *  \param [in] Buffer : Pointer to Memory Location to Store Data
 *  \param [in] Size : Variable to Store Number of Bytes to Transmit
 *  \return BUSY / OK
 */
uint8_t USART_TransmitBytes(uint8_t *Buffer, uint16_t Size)
{
	if (Size == 0) return ERROR;
	
	while (USART_TransmitBusyFlag)
	{
		return BUSY;
	}
	USART_TransmitBuffer = Buffer;
	USART_TransmitBufferLength = Size;
	USART_TransmitBufferIndex = 0;
	USART_TransmitBusyFlag = 1;
	USART_EnableTransmitInterrupt();
	return OK;
}

/**
 *  \brief Public Function to Transmit char Data
 *  
 *  \param [in] Character : Variable to Store char Data
 *  \return BUSY / OK
 */
uint8_t USART_TransmitChar(char Character)
{
	uint8_t* data = (uint8_t*)&Character;
	return USART_TransmitByte(data);
}

/**
 *  \brief Public Function to Transmit int Data as ASCII
 *  
 *  \param [in] Number : Variable to Store int Data
 *  \return BUSY / OK
 */
uint8_t USART_TransmitInt(int Number)
{
	while (USART_TransmitBusyFlag)
	{
		return BUSY;
	}
	static uint8_t localbuffer[7] = {0};
	uint16_t Length = UTILS_IntToString(Number,(char*)localbuffer);
	return USART_TransmitBytes(localbuffer,Length);
}

/**
 *  \brief Public Function to Transmit long Data as ASCII
 *  
 *  \param [in] Number : Variable to Store long Data
 *  \return BUSY / OK
 */
uint8_t USART_TransmitLong(long Number)
{
	while (USART_TransmitBusyFlag)
	{
		return BUSY;
	}
	static uint8_t localbuffer[12] = {0};
	uint16_t Length = UTILS_LongToString(Number,(char*)localbuffer);
	return USART_TransmitBytes(localbuffer,Length);
}

/**
 *  \brief Public Function to Transmit float Data as ASCII
 *  
 *  \param [in] Number : Variable to Store float Data
 *  \param [in] Precision : Variable to Store Precision of Decimal Number
 *  \return BUSY / OK
 */
uint8_t USART_TransmitFloat(float Number,uint8_t Precision)
{
	while (USART_TransmitBusyFlag)
	{
		return BUSY;
	}
	static uint8_t localbuffer[17] = {0};
	uint16_t Length = UTILS_FloatToString(Number,(char*)localbuffer,Precision);
	return USART_TransmitBytes(localbuffer,Length);
}

/**
 *  \brief Public Function to Transmit string Data
 *  
 *  \param [in] Buffer : Pointer to Memory Location to Store string Data
 *  \return BUSY / OK
 */
uint8_t USART_TransmitString(char *Buffer)
{
	while (USART_TransmitBusyFlag)
	{
		return BUSY;
	}
	const char *p = Buffer;
	while (*p) ++p;
	uint16_t Size = p - Buffer;
	USART_TransmitBuffer = (uint8_t*)Buffer;
	USART_TransmitBufferLength = Size;
	USART_TransmitBufferIndex = 0;
	USART_TransmitBusyFlag = 1;
	USART_EnableTransmitInterrupt();
	return OK;
}

/**
 *  \brief Public Function to Cancel any Interrupt based Transmission
 *  
 *  \return void
 */
void USART_CancelTransmit()
{
	USART_DisableTransmitInterrupt();
	USART_TransmitBusyFlag = 0;
}

/**
 *  \brief Interrupt Service Routine for Reception
 *  
 *  \param [in] USART_RX_vect
 */
ISR(USART_RX_vect)
{
	if(USART_ReceiveBufferIndex < USART_ReceiveBufferLength)
	{
		USART_ReceiveBuffer[USART_ReceiveBufferIndex] = UDR0;
		USART_ReceiveBufferIndex++;
	}
	if(USART_ReceiveBufferIndex >= USART_ReceiveBufferLength)
	{
		USART_ReceiveBusyFlag = 0;
		USART_DisableReceiveInterrupt();
	}
}

/**
 *  \brief Interrupt Service Routine for Transmission
 *  
*  \param [in] USART_UDRE_vect
 */
ISR(USART_UDRE_vect)
{
	if(USART_TransmitBufferIndex < USART_TransmitBufferLength)
	{
		UDR0 = USART_TransmitBuffer[USART_TransmitBufferIndex];
		USART_TransmitBufferIndex++; 
	}
	if(USART_TransmitBufferIndex >= USART_TransmitBufferLength)
	{
		USART_TransmitBusyFlag = 0;
		USART_DisableTransmitInterrupt();
	}
}

#endif /* AVR_USART_H_ */