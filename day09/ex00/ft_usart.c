#include "ft_usart.h"
#include <avr/io.h>
#include <stdint.h>


void usart_init(void)
{
	PRR &= ~(1 << PRUSART0);			/* ensure USART is enabled */
	UBRR0 = F_CPU / 2 / (8 * BAUD);		/* 8 bits dataframe */
	UCSR0A &= ~0b10;					/* Disable 2x speed */
	UCSR0B |= (1 << TXEN0);				/* enable TX */
	UCSR0B |= (1 << RXEN0),				/* enable RX */
	UCSR0C |= 0b011 << UCSZ00;			/* 8-bits */
	UCSR0C |= 0 << USBS0;				/* 1 stop bit */

	UCSR0B |= 1 << RXCIE0;		/* Enable data reception interupt */

}

void usart_tx(uint8_t data)
{
	while (!( UCSR0A & (1 << UDRE0))) ;
	UDR0 = data;
}

uint8_t	usart_rx(void)
{
	while (!( UCSR0A & (1 << RXC0)));
	return UDR0;
}

void usart_printstr(const char *str)
{
	while (*str > 0)
	{
		usart_tx(*str);
		str++;
	}
}

static const char letters[16] = "0123456789ABCDEF";

void   usart_dump(uint8_t data)
{
	usart_tx(letters[(data >> 4) & 0x0f]);
	usart_tx(letters[data & 0x0f]);
	usart_tx(' ');
}

void usart_dumpln(const char *prompt, uint8_t data, const char *end)
{
	usart_printstr(prompt);
	usart_tx(letters[(data >> 4) & 0x0f]);
	usart_tx(letters[data & 0x0f]);
	usart_printstr(end);
}

void usart_dump16ln(const char *prompt, uint16_t data, const char *end)
{
	usart_printstr(prompt);
	usart_tx(letters[(data >> 12) & 0x0f]);
	usart_tx(letters[(data >> 8) & 0x0f]);
	usart_tx(letters[(data >> 4) & 0x0f]);
	usart_tx(letters[data & 0x0f]);
	usart_printstr(end);
}

void	usart_int16ln(uint16_t data)
{
	if (data > 9)
		usart_int16ln(data / 10);
	usart_tx(letters[data % 10]);
}
