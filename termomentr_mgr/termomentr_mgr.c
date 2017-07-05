//#define F_CPU 1000000L
#include <util/delay.h>
#include <avr/io.h>
#include "hd44780.h"

#define SET_ONEWIRE_PORT		PORTD |= _BV(7)
#define CLR_ONEWIRE_PORT		PORTD &= ~_BV(7)
#define IS_SET_ONEWIRE_PIN		PIND & _BV(7)
#define SET_OUT_ONEWIRE_DDR		DDRD |= _BV(7)
#define SET_IN_ONEWIRE_DDR		DDRD &= ~_BV(7)

unsigned char OneWireReadByte(void)
{
	unsigned char i, byte = 0;
	SET_IN_ONEWIRE_DDR;
	for (i=0; i<8; i++)
	{
		SET_OUT_ONEWIRE_DDR;
		_delay_us(7);
		SET_IN_ONEWIRE_DDR;
		_delay_us(7);
		byte >>= 1;
		if(IS_SET_ONEWIRE_PIN) byte |= 0x80;
		_delay_us(70);
	}
	return byte;
}
unsigned char OneWireReset()
{
	CLR_ONEWIRE_PORT;
	if (!(IS_SET_ONEWIRE_PIN)) return 0;
	SET_OUT_ONEWIRE_DDR;
	_delay_us(500);
	SET_IN_ONEWIRE_DDR;
	_delay_us(70);
	if(!(IS_SET_ONEWIRE_PIN))
	{
		_delay_us(500);
		return(1);
	}
	_delay_us(500);
	return(0);
}
void OneWireWriteByte(unsigned char byte)
{
	unsigned char i;
	CLR_ONEWIRE_PORT;
	for (i=0; i<8; i++)
	{
		SET_OUT_ONEWIRE_DDR;
		if (byte & 0x01)
		{
			_delay_us(7);
			SET_IN_ONEWIRE_DDR;
			_delay_us(70);
		}
		else
		{
			_delay_us(70);
			SET_IN_ONEWIRE_DDR;
			_delay_us(7);
		}
		byte >>= 1;
	}
}
unsigned char ds18b20_ConvertT(void)
{
	if (!OneWireReset()) return 0;
	OneWireWriteByte(0xcc); // SKIP ROM
	OneWireWriteByte(0x44); // CONVERT T
	
	return 1;
}
int ds18b20_Read(unsigned char scratchpad[])
{
	unsigned char i;
	if (!OneWireReset()) return 0;
	OneWireWriteByte(0xcc); // SKIP ROM
	OneWireWriteByte(0xbe); // READ SCRATCHPAD
	for(i=0; i<9; i++) scratchpad[i] = OneWireReadByte();
	return 1;
}

void set_USART(void)
{
	UCSRB&=0x0B;    //0b00001011
	UCSRB|=0x08;	//0b00001000      //deaktyw. przerwan
	UCSRC&=0x87;	//0b10000111
	UCSRC|=0x86;	//0b10000110	  //8 bit , 1 bit stopu, no parity
	UCSRC&=0x7F;
	UBRRH =0x00;
	UBRRL =0x33; 	//0b00110011      //9600 baud/s
}

char str[]=" Termometr medyczny. ";
char str1[17], strt[17];
int t = 0, p = 0, i, k, licznik=10;
int temp1, temp2;
unsigned char ds18b20_pad[9];

int main(void)
{
	k = strlen(str);
	set_USART();
	lcd_init();
	lcd_cls();

while(1)
{	
	if(ds18b20_ConvertT())	/* Funkcja 'ds18b20_ConvertT' wysy³a do uk³adu ds18b20	polecenie pomiaru */
	{		
		_delay_ms(1000);
		ds18b20_Read(ds18b20_pad);		
		temp1 = (((ds18b20_pad[1] << 8) + ds18b20_pad[0]) / 16.0);  /* Sk³ada dwa bajty wyniku pomiaru w ca³oœæ. Cztery pierwsze bity mniej	znacz¹cego bajtu to czêœæ u³amkowa wartoœci temperatury, wiêc ca³oœæ	dzielona jest przez 16 */
		temp2 = ((ds18b20_pad[0] & 0xF)*625); // obliczanie czesci u³amkowej 100 - 2 miejsca po przecinku, 1000 - jedno miejsce po przecinku
					
		if (licznik==10)						//wysylanie przez RS co 10s
			{	
				sprintf(strt,"%d"",", temp1);
	
				if (temp2==0)		sprintf(str1,"000""%d", temp2);			
					else	
					if(temp2<700 && temp2>0)	sprintf(str1,"0""%d", temp2);
						else	sprintf(str1,"%d", temp2);
	
				UDR = 13;				//kursor poczatek
				while(!(UCSRA&0b00100000));
				while(!(UCSRA&0b01000000));
				UCSRA|=0b01000000;
	
				UDR = 10;			//nowa linia
				while(!(UCSRA&0b00100000));
				while(!(UCSRA&0b01000000));
				UCSRA|=0b01000000;
	
				i=0;
				while(strt[i]) //czêœæ ca³kowita
				{
					UDR = strt[i];
					while(!(UCSRA&0b00100000));
					while(!(UCSRA&0b01000000));			
					UCSRA|=0b01000000;
					i++;
				}
		
				i=0;
				while(str1[i]) //czêœæ po przecinku
				{
					UDR = str1[i];
					while(!(UCSRA&0b00100000));
					while(!(UCSRA&0b01000000));		
					UCSRA|=0b01000000;
					i++;
				}
				licznik=0;
			}							// zakonczenie wysylania RS
		licznik++;		

		lcd_locate(0,0);		//tekst w pierwszej linii
		lcd_str("                ");
		lcd_locate(0,t--);
		lcd_str(str);
		if( t<=13-k )
		{
			lcd_locate(0,16-p);
			lcd_str(str);
			if (p==15)
			{t=0; p=0;}p++;
		}

		sprintf(strt,"%d"".", temp1);
		lcd_locate(1,0);
		lcd_str("  Temp= ");

		lcd_str(strt);				
		temp2 = ((ds18b20_pad[0] & 0xF)*625)/1000; // obliczanie czesci u³amkowej 100 - 2 miejsca po przecinku, 1000 - jedno miejsce po przecinku
		sprintf(strt,"%d\xdf""C ", temp2);
		lcd_str(strt);
	}
}
}
