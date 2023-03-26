/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:42:27 by gtoubol           #+#    #+#             */
/*   Updated: 2023/03/23 12:50:12 by lgiband          ###   ########.fr       */
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
	//SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0) | (1<<SPR1);
	//SPSR &= ~(1<<SPIF);
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

//int	main()
//{
//	usart_init();
//	SPI_MasterInit();

//	for (;;) {
//		boudary_frame(0);
//		transmitLed(0xE50000FF);
//		//transmitLed(0xE00000FF);
//		//transmitLed(0xE00000FF);
//		//boudary_frame(1);
//		_delay_ms(1000);
//	}
//	return (0);
//}

uint32_t get_color(uint8_t state)
{
	uint32_t color = 0;
	if (state == 1)
		color = 0x0000FF;
	else if (state == 2)
		color = 0x00FF00;
	else if (state == 3)
		color = 0xFF0000;
	else if (state == 4)
		color = 0xFFFF00;
	else if (state == 5)
		color = 0x00FFFF;
	else if (state == 6)
		color = 0xFF00FF;
	else if (state == 7)
		color = 0xFFFFFF;
	return (color);
}

uint8_t state = 1;
uint32_t color = 1;

ISR(TIMER1_COMPA_vect)
{
	cli();
	state = ((state + 1) % 7) + 1;
	boudary_frame(0);
	color = get_color(state);
	transmitLed(color | 0xEA000000);
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
	OCR1A = 0b0011110100001001; //Set clock to match 7813 31250 = 0b
	
	//How to use Interrupts on the timer
	TCCR1A |= (1 << COM1A0); // Toggle OC1A on Compare Match (on/off at each match) 
	TCCR1B |= (1 << WGM12); //Set to CTC mode
	TIMSK1 |= (1 << OCIE1A);

	//TCNT0 = 0; //Clear the timer
	//TCCR0B |= (1 << CS02) | (1 << CS00); //set timing to 1024
	//OCR0A = 250; //set frequence on 62.5Hz
	//TCCR0A |= (1 << WGM01);
	//TIMSK0 |= (1 << OCIE0A);

		//TCNT1 = 0; //Clear the timer

	//TCNT1 = 0b0110000000001001;
	//TCCR1B |= (1 << CS10) | (1 << CS12); //set timing to 1024
	//OCR1A = 0b0001111010000101; //Set clock to match 7813
	
	////How to use Interrupts on the timer
	//TCCR1A |= (1 << COM1A0); // Toggle OC1A on Compare Match (on/off at each match) 
	//TCCR1B |= (1 << WGM12); //Set to CTC mode
	//TIMSK1 |= (1 << OCIE1A);


	for(;;) {
		//FT_printf(sens);
	}
	return (0);
}
