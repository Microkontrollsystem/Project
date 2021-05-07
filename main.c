/*
 * Project_Microcontr.c
 *
 * Created: 5/6/2021 8:30:09 AM
 * Author : Youssef Aldabak
 */ 

#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

// Følgende er definisjoner for pins for lcd.h
#define D0 eS_PORTD6
#define D1 eS_PORTD5
#define D2 eS_PORTB5
#define D3 eS_PORTC2
#define D4 eS_PORTD4
#define D5 eS_PORTC1
#define D6 eS_PORTB2
#define D7 eS_PORTD7
#define RS eS_PORTB0
#define EN eS_PORTC3

// Følgende er definisjoner for HC-SR04-sensoren.
#define trigPin PORTD3
#define echoPin PORTD2

//Header files
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "lcd.h"	
#include "avr_adc.h"
#include "avr_usart.h"
#include "i2c.h"
#include "LCD_I2C.h"

//Flyktige globale variabler, må være flyktige slik det nå kan endres i ISR

static volatile unsigned int echoFlag = 0;	// Flyktig int, echoFlag = 0.
static volatile unsigned int pulseEnd = 0;	// Flyktig int, pulseEnd = 0.


// Funksjon for å initialisere USART:
void initUSART()
{
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8); // Dette setter BAUD-hastigheten, må skifte UBRROH 8 bits til høyre
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);	 // Setter nedre 8 bits UBRR0
	UCSR0B = (1<<TXEN0); // Gjør det mulig å sende data.
	UCSR0C = (3<<UCSZ00); // Den er satt til 8 bit.
}

// Initialiser verdier:
void init()
{
	DDRB = 0xFF;//  Still  pinner på PortB for utdata.
	PORTB = 0x00;	// Sett pinnene til lave i utgangspunktet, unngå flyt.
	DDRC = 0xFF;	// Sett pinner som utgang, for LCD-skjerm.
	DDRD = 0xFF;	// Sett som utgangspinner for LCD-skjerm.
	DDRD &= ~(1<<echoPin); // EchoPin er inngang.
	PORTD = 0x00;	//  Still  pinnene på lave, unngå flyt.
	EIMSK |= (1<<INT0);		// Interrupt INT0 aktivert.
	TIMSK2 |= (1<<TOIE2);	// Aktiver overløpsavbrudd på Timer 2.
	EICRA |= (1<<ISC00); // Avbryt på stigende kant og fallende kant.
}

//initialise servo:
void initServo()
{
	DDRB |= (1<<PINB1); // Still pin 9 til utgang PWM.
	// 1. Sett hurtig PWM-modus 14: WGM11, WGM12, WGM13 til 1.
	// 2. Sett forhåndskala på 8.
	// 3. Sett hurtig PWM ikke-inverterende modus.
	
	TCCR1A |= (1 << WGM11) | (1 << COM1A1);
	TCCR1B |= (1 << WGM13) | (1 << WGM12) |(1 << CS11);
	 // 4. Sett ICR1: ICR1 er den øverste definerende PWM-perioden. 
	ICR1 = 40000;
	//5. Sett inn driftssyklus, kan trenge forskyvning for å rette.
}

void int_i2c_lcd()
{
	i2c_init();
	i2c_start();
	i2c_write(0x70);
	lcd_init();
	lcdi2c_msg(" Temperature is :");
	Lcdi2c_rite_String('LM35_Temp');
}
// Forsinkelsesfunksjon ved hjelp av Timer0.
void delayFunction_Timer0(int a)	// 8 bit Timer.
{
	OCR0A = a;	// Motgrense.
	TCNT0 = 0x00;		// Initialiser Timer 0 bits.
	TCCR0A |= (1<<WGM01);		// Initialiser-Timer 0 bits.
	TCCR0B |= (1<<CS00);		// Timer0, CS00 = ingen forhåndsskaler.
	while ((TIFR0 & (1<<OCF0A)) == 0) {	// Telle opp til OCF0-verdi.
	}
	TCCR0B = 0x00;		// Stopp klokke når OCF1-verdien er nådd.
	TIFR0 = (1<<OCF0A);	// Sett flagget til 1.	
}
// Pulse 10uSecs for å utløse inngang, dette starter rangeprosessen.
void signalPulse()
{
	PORTB |= (1<<trigPin);	// Sett trigPin høyt.
	delayFunction_Timer0(0xA4);	// 0x9F = 159 = 15 usek.
	PORTB &= ~(1<<trigPin);		// Sett trigPin lavt igjen.
}

// Funksjon for å gjenta over meldingsstrengen.
// Kan ikke direkte itere over strengen, så må bruke char peker.
void putStringUSART(char* stringPtr)// Ta pekeren til char som parameter.
{
	while (*stringPtr != 0) {	// Mens dataene pekeren peker på er ikke null.
		while (!(UCSR0A & (1<<UDRE0)));	 // Vent med å motta data.
		UDR0 = *stringPtr;	// UDR0 = dereferensert stringPtr-tegn.
		stringPtr++; // ForhåndsstrengPtr med 1.
	}
}

// Denne funksjonen skriver ut avstandslinjen til LCD-skjermen;
void distanceBar()
{
// Dette angir hvor mange cm som representeres av hver røye på LCD-skjermen.
// Endring av denne verdien er mulig opp til en maksimumsverdi på 17.
// (17x16 = 272 cm rekkevidde, maks er 275 cm og kan derfor ikke gå utover 17).
	int cmPerChar = 1;
	
	if (pulseEnd > 0 && pulseEnd <= cmPerChar) {
		Lcd8_Write_String("1 cm");
		Lcd8_Write_String("              ");
	}
	
	else if (pulseEnd > cmPerChar && pulseEnd <= 2*cmPerChar) {
		Lcd8_Write_String("2 cm");
		Lcd8_Write_String("               ");
		
	}
	
	else if (pulseEnd > 2*cmPerChar && pulseEnd <= 3*cmPerChar) {
		Lcd8_Write_String("3 cm");
		Lcd8_Write_String("               ");
	}
	
	else if (pulseEnd > 3*cmPerChar && pulseEnd <= 4*cmPerChar) {
		Lcd8_Write_String("4 cm");
		Lcd8_Write_String("               ");
	}
	
	else if (pulseEnd > 4*cmPerChar && pulseEnd <= 5*cmPerChar) {
		Lcd8_Write_String("5 cm");
		Lcd8_Write_String("                ");
	}
	
	else if (pulseEnd > 5*cmPerChar && pulseEnd <= 6*cmPerChar) {
		Lcd8_Write_String("6 cm");
		Lcd8_Write_String("                ");
	}
	
	else if (pulseEnd > 6*cmPerChar && pulseEnd <= 7*cmPerChar) {
		Lcd8_Write_String("7 cm");
		Lcd8_Write_String("              ");
	}
	
	else if (pulseEnd > 7*cmPerChar && pulseEnd <= 8*cmPerChar) {
		Lcd8_Write_String("8 cm");
		Lcd8_Write_String("             ");
	}
	
	else if (pulseEnd > 8*cmPerChar && pulseEnd <= 9*cmPerChar) {
		Lcd8_Write_String("9 cm");
		Lcd8_Write_String("              ");
	}
	
	else if (pulseEnd > 9*cmPerChar && pulseEnd <= 10*cmPerChar) {
		Lcd8_Write_String("10 cm");
		Lcd8_Write_String("               ");
	}
	
	else if (pulseEnd > 10*cmPerChar && pulseEnd <= 11*cmPerChar) {
		Lcd8_Write_String("11 cm");
		Lcd8_Write_String("               ");
	}
	
	else if (pulseEnd > 11*cmPerChar && pulseEnd <= 12*cmPerChar) {
		Lcd8_Write_String("12 cm");
		Lcd8_Write_String("              ");
	}
	
	else if (pulseEnd > 12*cmPerChar && pulseEnd <= 13*cmPerChar) {
		Lcd8_Write_String("13 cm");
		Lcd8_Write_String("              ");
	}
	
	else if (pulseEnd > 14*cmPerChar && pulseEnd <= 15*cmPerChar) {
		Lcd8_Write_String("14 cm");
		Lcd8_Write_String("               ");
	}
	
	else if (pulseEnd > 16*cmPerChar && pulseEnd <= 17*cmPerChar) {
		Lcd8_Write_String("15 cm");
		Lcd8_Write_String("               ");
	}
	else if (pulseEnd > 17*cmPerChar && pulseEnd <= 18*cmPerChar) {
		Lcd8_Write_String("16 cm");
		Lcd8_Write_String("               ");
	}
	else if (pulseEnd > 18*cmPerChar && pulseEnd <= 19*cmPerChar) {
		Lcd8_Write_String("17 cm");
		Lcd8_Write_String("               ");
	}
	else if (pulseEnd > 18*cmPerChar && pulseEnd <= 19*cmPerChar) {
		Lcd8_Write_String("18 cm");
		Lcd8_Write_String("               ");
	}
	else if (pulseEnd > 19*cmPerChar && pulseEnd <= 20*cmPerChar) {
		Lcd8_Write_String("19 cm");
		Lcd8_Write_String("               ");
	}
	else if (pulseEnd > 20*cmPerChar && pulseEnd <= 21*cmPerChar) {
		Lcd8_Write_String("15 cm");
		Lcd8_Write_String("               ");
	}
	else {
		Lcd8_Write_String(" MORE THAN 20 CM ");
		Lcd8_Write_String("                 ");
	}
}

// For måling av vinkel, i henhold til databladet:
// Inneboende målevinkel er 15 grader.
// Servomotor kan snu 180 grader.
// Så vi trenger bare å måle vinkelen fra (0 + 15) <= vinkel <= (180-15)
// Servo er imidlertid upålitelig i ekstreme forhold (nær 0 og 180 grader)
// Så, denne servoen vil gå mellom 50 og 165 grader.
void angleLoop()
{
	
	
	int16_t pulseRange = 0;	// Telleren er i utgangspunktet 0.
	char pulseString[5];		// Char array, brukes til å sende streng til USART.
	char angleString[5];		// Char array, for sending vinkel til USART.
	
	// Mål fra 50 til 165 grader.
	for (int angle = 50; angle <= 165; angle++) {
		
		
		OCR1A = angle*25;	// OCR1A skalerer med vinkelen, multiplisert med en konstant.
		_delay_ms(30);	// 30ms forsinkelse, forhindrer avstøting.
		signalPulse();		// Anropssignalpuls.
		pulseRange = pulseEnd;	// PulseRange tar verdien av TCNT2.
		putStringUSART("Distance: ");	// Skriv avstanden til datavisualisereren.
		itoa(pulseRange, pulseString, 10);		// Konverter int til streng
		putStringUSART(pulseString);		// Skriv streng til data visualizer.
		putStringUSART(" cm, ");	// skriv ut til datavisualiser, ny linje etter
		putStringUSART("Angle: ");	// skriv avstanden til datavisualisereren
		itoa(angle, angleString, 10);		// konvertere int til streng
		putStringUSART(angleString);	// skriv ut vinkelen til datavisualisereren
		
		putStringUSART(".\n");	// gjør deretter en ny linje
		_delay_ms(1);	// 1ms forsinkelse for å forhindre avstøting
		Lcd8_Set_Cursor(2,0);	// angi ny posisjon for markøren
		
		distanceBar();	// gå til distanceBar ()
		_delay_ms(1);	// 1ms forsinkelse for å forhindre avstøting
	}
	
	// Mål fra 165 tilbake til 50 grader
	for (int angle = 165; angle >= 50 ; angle--) {
		
		OCR1A = angle*25;		// OCR1A skalerer med vinkelen, multiplisert med en konstant.
		_delay_ms(30);		// 30ms forsinkelse, forhindrer avstøting.
		signalPulse();		// Anropssignalpuls.
		pulseRange = pulseEnd;		// PulseRange tar verdien av TCNT2.
		putStringUSART("Distance: ");	// Skriv avstanden til datavisualisereren.
		itoa(pulseRange, pulseString, 10);		// Konverter int til streng
		putStringUSART(pulseString);		// Skriv streng til data visualizer
		putStringUSART(" cm, ");	// skriv ut til datavisualiser, ny linje etter
		putStringUSART("Angle: ");	// skriv avstanden til datavisualisereren
		itoa(angle, angleString, 10);		// konvertere int til streng
		putStringUSART(angleString);	// skriv ut vinkelen til datavisualisereren
		putStringUSART(".\n");		// Gjør deretter en ny linje
		_delay_ms(1);			// 1ms forsinkelse for å forhindre avstøting
		Lcd8_Set_Cursor(2,0);	// angi ny posisjon for markøren
		
		distanceBar();		// gå til distanceBar ()
		
		_delay_ms(1);		// 1ms forsinkelse for å forhindre avstøting
		
	}
	

}

int main()
{
	ADC_ConfigData ADC_Data;
	ADC_Data.Prescaler = ADC_PRESCALER_128;
	ADC_Data.VoltageReference = ADC_VOLTAGE_REFERENCE_AVCC;
	ADC_Init(ADC_Data);
	
	// USART-konfigurasjon og start
	USART_ConfigData USART_Data;
	USART_Data.BaudRate = 9600;
	USART_Data.DataBit = USART_DATA_BIT_EIGHT;
	USART_Data.ParityBit = USART_PARITY_BIT_NO;
	USART_Data.StopBit = USART_STOP_BIT_ONE;
	USART_Data.UsartMode = USART_MODE_ASYNCHRONOUS;
	USART_Init(USART_Data);
	
	// Lokale variabler
	uint16_t ADC_RawData = 0x0000;
	int LM35_Temp = 0.0f;
	
	
	initUSART(); // USART initialisering
	init();	// utgangsinitialisering
	initServo();
	int_i2c_lcd();
	Lcd8_Init();	// lcd8 initialisering
	_delay_ms(50);
	
	sei();		// sett ekstern avbrudd
	while(1) {
		ADC_RawData = ADC_ReadPin(ADC0);
		
		//Konverter den rå ADC-verdien til temperatur i Kelvin 
		LM35_Temp = ( (ADC_RawData )) * 4.88;
		LM35_Temp = LM35_Temp /10;
		
		USART_TransmitString("\n Temperature is : ");
		USART_WaitTillTransmitFree();
		USART_TransmitFloat(LM35_Temp,3);
		putStringUSART(".\n");
		putStringUSART(".\n");
		Lcd8_Set_Cursor(1,0);	
		Lcd8_Write_String("Distance:");		
		
		

		angleLoop();	
	}
	
	return 0;
}

ISR(INT0_vect)
{
	// hvis flagget er 0, start timer2 med 1024 forhåndsskaler
	if(echoFlag == 0) {
		TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);	//1024 pre-scaler
		echoFlag = 1;	//set flag to 1
	}
	else {
		TCCR2B = 0;	//stop timer
		pulseEnd = TCNT2;	//pulse takes values of counter
		TCNT2 = 0;		//reset counter
		echoFlag = 0;		//reset flag
	}
}

// Dette avbruddet vil nullstille Timer 2 hvis overløp oppstår.
// Men fordi timeren er 8 bit, er den begrenset til å vise bare verdier opp til 275 cm.
// Derfor vil tidtakeren aldri telle utenfor sensorens rekkevidde, 400 cm.
// Dermed vil overløp aldri forekomme. Dette er ganske enkelt her for å øke robustheten i systemet.
ISR(TIMER2_OVF_vect){
	TCCR2B = 0;	//stop timer
	TCNT2 = 0;	//reset counter
	echoFlag = 0;	//reset flag
}

