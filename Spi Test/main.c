#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

#define  SS_PIN PA7
#define DD_SS_PIN DDA7

#define DO_PIN PA5
#define DD_DO_PIN DDA5

#define DI_PIN PA6
#define DD_DI_PIN DDA6

#define CLK_PIN PA4
#define DD_CLK_PIN DDA4

//LED output used to indicate whether data is being sent or not 
#define LED PA0 
#define DD_LED DDA0


void SPISetup();
uint8_t WriteSPI(uint8_t data);


int main(void)
{
	uint8_t data = 1; 
	
	DDRA |= (1<<DD_LED); //setting pin A0 as output	
	PORTA &= ~(1<<LED); //setting pin A0 on low
	
	DDRB=0xff; // setting port b as outputs
	PORTB=0; //setting all port b pins on low
	
	SPISetup();//setting registers to use three wire mode
	_delay_ms(1000);
	
	
	//sending numbers from 1 to 7 through SPI and repeating the process
	while(1)
	{
		PORTA |= (1<<LED);
	
		PORTB=WriteSPI(data++);
		
		_delay_ms(3000);
		
		PORTA &= ~(1<<LED);
		
		_delay_ms(1000);
			
		if(data>7)
		{
			data=1;
		}	
			
	}
}


void SPISetup()
{
	//outputs
	DDRA|= (1<<DD_DO_PIN) | (1<<DD_SS_PIN) | (1<<DD_CLK_PIN);
	
	//inputs
	DDRA &= ~(1<<DD_DI_PIN);
	
	//spi config
	USICR = (1<<USIWM0);
	
	//ss_pin on, di_pin enable pull-up
	PORTA |=(1<<SS_PIN) | (1<<DI_PIN); 	
}

uint8_t WriteSPI(uint8_t message)
{
	//load usi data register with the data to be transmitted
	USIDR = message;
	USISR |= (1<<USIOIF);
	
	PORTA &= ~(1<<SS_PIN);

	//transmit data
	while((USISR & (1<<USIOIF)) == 0)
	{
		USICR = (1<<USIWM0) | (1<<USICS1) | (1<<USICLK) | (1<<USITC);
	}
	
	PORTA |= (1<<SS_PIN);
	return USIDR;
}