/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:42:27 by gtoubol           #+#    #+#             */
/*   Updated: 2023/03/20 12:47:39 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "ft_usart.h"
#include "ft_i2c.h"

#define BYTE_ADDRESS 0x00
#define BYTE_INIT_ADDRESS 0x01
#define BYTE_INIT_VALUE 0xBB

uint8_t data;
//uint8_t eeprom_initialized EEMEM = 0;

void	FT_printf(uint8_t n)
{
	PORTB = (n & 1<<PB0) | (n & 1<<PB1) | (n & 1<<PB2) | (n<<1 & 1<<PB4);
}

ISR(INT0_vect)
{
	data++;
	eeprom_write_byte((uint8_t*)BYTE_ADDRESS, data);
	FT_printf(data);
	EIFR |= 1;
}

void button_init(void)
{
	EICRA |= 0b10;				/* Enable button */
	EIMSK |= 1;
}

int	main(void)
{
	DDRB = 0x17;

	usart_init();
	//i2c_init();
	button_init();
	sei();
	
	if (eeprom_read_byte((uint8_t*)BYTE_INIT_ADDRESS) != (uint8_t)BYTE_INIT_VALUE)
	{
		eeprom_write_byte((uint8_t*)BYTE_ADDRESS, 0);
		eeprom_write_byte((uint8_t*)BYTE_INIT_ADDRESS, BYTE_INIT_VALUE);
	}
	data = eeprom_read_byte((uint8_t*)BYTE_ADDRESS);
	FT_printf(data);
	
	for(;;)
	{
	}
}
