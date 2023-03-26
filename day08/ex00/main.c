/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:42:27 by gtoubol           #+#    #+#             */
/*   Updated: 2023/03/23 13:00:55 by lgiband          ###   ########.fr       */
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

int	main()
{
	usart_init();
	SPI_MasterInit();

	for (;;) {
		boudary_frame(0);
		transmitLed(0xE50000FF);
		//transmitLed(0xE50000FF);
		//transmitLed(0xE50000FF);
		//boudary_frame(1);
		_delay_ms(1000);
	}
	return (0);
}

