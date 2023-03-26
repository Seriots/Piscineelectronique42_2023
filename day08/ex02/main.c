/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:42:27 by gtoubol           #+#    #+#             */
/*   Updated: 2023/03/23 13:15:43 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ft_usart.h"

void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output, all others input */
	DDRB = (1<<DDB3)|(1<<DDB5)|(1<<DDB2);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);// | (1<<SPR1);
}

void SPI_MasterTransmit(uint8_t cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
}
void	transmitLed(uint32_t data)
{
	SPI_MasterTransmit(data >> 24);
	SPI_MasterTransmit(data >> 16);
	SPI_MasterTransmit(data >> 8);
	SPI_MasterTransmit(data);
}

void	boudary_frame(uint8_t bound)
{
	if (bound == 0)
	{
		SPI_MasterTransmit(0);
		SPI_MasterTransmit(0);
		SPI_MasterTransmit(0);
		SPI_MasterTransmit(0);
	}
	else
	{
		SPI_MasterTransmit(0xFF);
		SPI_MasterTransmit(0xFF);
		SPI_MasterTransmit(0xFF);
		SPI_MasterTransmit(0xFF);
	}
}

uint8_t state = 0;
uint32_t color = 1;

ISR(TIMER1_COMPA_vect)
{
	cli();
	boudary_frame(0);
	switch (state) {
		case 3:
			transmitLed(0xE8000000);
			transmitLed(0xE8000000);
			transmitLed(0xE800FF00);
			break;
		case 2:
			transmitLed(0xE8000000);
			transmitLed(0xE800A5FF);
			transmitLed(0xE8000000);
			break;
		case 1:
			transmitLed(0xE80000FF);
			transmitLed(0xE8000000);
			transmitLed(0xE8000000);
			break;
		case 0:
			transmitLed(0xE8000000);
			transmitLed(0xE8000000);
			transmitLed(0xE8000000);
			break;
	}
	boudary_frame(1);
	//transmitLed(color | 0xE1000000);
	state = ((state + 1) % 4);
	sei();
}


int	main()
{
	usart_init();
	SPI_MasterInit();
	DDRD |= 1 << PD3 | 1 << PD5 | 1 << PD6;

	sei(); //Enable global interrupt

	TCNT1 = 0; //Clear the timer

	//TCNT1 = 0b0110000000001001;
	TCCR1B |= (1 << CS10) | (1 << CS12); //set timing to 1024
	OCR1A = 0b0000111101000010; //Set clock to match 7813 31250 = 0b
	
	//How to use Interrupts on the timer
	TCCR1A |= (1 << COM1A0); // Toggle OC1A on Compare Match (on/off at each match) 
	TCCR1B |= (1 << WGM12); //Set to CTC mode
	TIMSK1 |= (1 << OCIE1A);


	for(;;) {
		//FT_printf(sens);
	}
	return (0);
}
