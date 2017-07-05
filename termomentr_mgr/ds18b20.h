/*
Plik ds18b20.h
(xyz.isgreat.org)
*/
#ifndef DS18B20_H
#define DS18B20_H

/* DS18B20 przy��czony do portu PD7 AVRa */
#define SET_ONEWIRE_PORT		PORTB |= _BV(0)
#define CLR_ONEWIRE_PORT		PORTB &= ~_BV(0)
#define IS_SET_ONEWIRE_PIN		PINB & _BV(0)
#define SET_OUT_ONEWIRE_DDR		DDRB |= _BV(0)
#define SET_IN_ONEWIRE_DDR		DDRB &= ~_BV(0)

unsigned char ds18b20_ConvertT(void);
int ds18b20_Read(unsigned char []);
void OneWireStrong(char);
unsigned char OneWireReset(void);
void OneWireWriteByte(unsigned char);
unsigned char OneWireReadByte(void);

#endif