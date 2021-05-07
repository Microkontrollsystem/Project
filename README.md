# Project
 Prosjekt medlemmer er:
 Youssef A Aldabak og Mohammad Hamza Zaidan
Det overordnede målet for dette prosjektet er å lage en SONAR-skanner ved å grensesnitt en ultralydssensor med ATmega328P. Ultralydssensoren festes til en servomotor som kan rotere 180 grader. Servoen vil rotere, og i hver rotasjonsvinkel vil ultralydssensoren måle. Sensoren registrerer avstandene til objekter i nærheten. Målingen vil være denne registrerte avstanden. Den registrerte avstanden, og den tilhørende vinkelen, sendes ut til Data Visualiser-terminalen i Atmel Studio 7 via USART. I tillegg vil en LCD-skjerm vise avstandene. Det vil vise en "bar" - jo lengre linjen er, jo større avstand er det mellom objektet og sensoren. Dermed vil dette prosjektet implementere en SONAR-skanner.
I tilegg denne prosjektet innholder til temperatursenso som måler tempaeraturen og vies ved I2c_LCD_skjerm.
