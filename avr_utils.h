#ifndef AVR_UTILS_H_
#define AVR_UTILS_H_

#include <stdlib.h>
#include <stddef.h>
#include <math.h>


/**
 *  \brief Public Function to Reverse a Buffer of Size Length
 *  
 *  \param [in,out] Buffer : Pointer to Memory Location to Store Data
 *  \param [in] Size : Variable to Store Length of Buffer
 *  \return void
 */
void UTILS_ReverseBuffer(uint8_t *Buffer, size_t Size)
{
	uint8_t ch;
	uint8_t *begin_ptr = Buffer;
	uint8_t *end_ptr = Buffer + Size - 1;
	for (size_t i = 0; i < Size / 2; i++)
	{
		ch = *end_ptr;
		*end_ptr = *begin_ptr;
		*begin_ptr = ch;
		begin_ptr++;
		end_ptr--;
	}
}

/**
 *  \brief Public Function to Reverse a string Data
 *  
 *  \param [in,out] Buffer : Pointer to Memory Location to Store string Data
 *  \return void
 */
void UTILS_ReverseString(char *Buffer)
{
	size_t size;
	char ch;
	char *begin_ptr = Buffer;
	char *end_ptr = Buffer;
	while (*++end_ptr);
	size = end_ptr - begin_ptr; 
	for (size_t i = 0; i < size / 2; i++)
	{
		ch = *end_ptr;
		*end_ptr = *begin_ptr;
		*begin_ptr = ch;	
		begin_ptr++;
		end_ptr--;
	}
}


//kort offentlig funksjon for å reversere en buffer med størrelseslengde
//param [inn, ut] Buffer: Peker til minneplassering for å lagre data
// param [in] Størrelse: Variabel for å lagre bufferlengde
// return ugyldig

uint8_t UTILS_IntToString(int Number, char *Buffer)
{
	uint8_t i = 0;
	uint8_t sign = 0;
	if (Number < 0)
	{
		Number *= -1;
		sign = 1;
	}
	do
	{
		Buffer[i++] = Number % 10 + '0';
	}
	while ((Number /= 10) > 0);
	if (sign == 1)
	{
		Buffer[i++] = '-';
	}
	UTILS_ReverseBuffer((uint8_t*)Buffer,i);
	Buffer[i] = '\0';
	return i;
}

//kort offentlig funksjon for å konvertere en lang data til en streng data
//param [in] Antall: Variabel for å lagre lange data
//param [ut] Buffer: Peker til minneplassering for å lagre strengdata
// return Lengde på strengdata

uint8_t UTILS_LongToString(long Number, char *Buffer)
{
	uint8_t i = 0;
	uint8_t sign = 0;
	if (Number < 0)
	{
		Number *= -1;
		sign = 1;
	}
	do
	{
		Buffer[i++] = Number % 10 + '0';
	}
	while ((Number /= 10) > 0);
	if (sign == 1)
	{
		Buffer[i++] = '-';
	}
	UTILS_ReverseBuffer((uint8_t*)Buffer,i);
	Buffer[i] = '\0';
	return i;
}

//brief Public Function to Convert an float Data into a string Data
//param [in] Number : Variable to Store float Data
//param [out] Buffer : Pointer to Memory Location to Store string Data
//param [in] Precision : Variable to Store Precision of Decimal Number
//return Length of string Data

uint8_t UTILS_FloatToString(float Number, char *Buffer, uint8_t Precision)
{
	int ipart = (int)Number;
	float fpart = Number - (float)ipart;
	size_t length1 = UTILS_IntToString(ipart,Buffer);
	if (Precision != 0) 
	{
		Buffer[length1] = '.';
	}
	if ((fpart < 0.1 && fpart > 0.0) || (fpart < -0.0 && fpart > -1.0))
	{
		length1++;
		Buffer[length1] = '0';
	}
	fpart = fpart * pow(10, Precision);
	fpart = abs(fpart);
	size_t length2 = UTILS_IntToString((int)fpart,Buffer+length1+1);
	Buffer[length1 + 1 + length2] = '\0';
	return length1 + 1 + length2;
}

//kort offentlig funksjon for å konvertere en strengdata til en int-data
//param [in] Buffer: Peker til minneplassering for å lagre strengdata
//return Konverterte int-data

int UTILS_StringToInt(char* Buffer)
{
	uint8_t sign = 0;
	int number = 0;
	if (*Buffer == '-')
	{
		sign = 1;
		Buffer++;
	}
	while (*Buffer)
	{
		number = number*10 + (*Buffer -'0');
		Buffer++;
	}
	if (sign)
	{
		number = (-1) * number;
	}
	return number;
}

//kort offentlig funksjon for å konvertere en strengdata til en lang data
 //param [in] Buffer: Peker til minneplassering for å lagre strengdata
// return Konverterte lange data
int UTILS_StringToLong(char* Buffer)
{
	uint8_t sign = 0;
	long number = 0;
	if (*Buffer == '-')
	{
		sign = 1;
		Buffer++;
	}
	while (*Buffer)
	{
		number = number*10 + (*Buffer -'0');
		Buffer++;
	}
	if (sign)
	{
		number = (-1) * number;
	}
	return number;
}

#endif /* AVR_UTILS_H_ */