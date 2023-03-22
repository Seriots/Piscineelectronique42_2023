/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:42:27 by gtoubol           #+#    #+#             */
/*   Updated: 2023/03/20 14:04:25 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdbool.h>

#include "ft_usart.h"
#include "ft_i2c.h"

#define BYTE_INIT_VALUE 0xDD

uint8_t data[4];
uint8_t selected = 0;

void	FT_printf(uint8_t n)
{
	PORTB = (n & 1<<PB0) | (n & 1<<PB1) | (n & 1<<PB2) | (n<<1 & 1<<PB4);
}

bool safe_eeprom_read(void *buffer, size_t offset, size_t length)
{
	uint8_t buf;
	
	buf = eeprom_read_byte((uint8_t*)(offset--));
	if (buf != BYTE_INIT_VALUE)
		return (1);
	eeprom_read_block(buffer, (uint8_t*)(offset), length);
	return (0);
}

bool safe_eeprom_write(void *buffer, size_t offset, size_t length)
{
	eeprom_write_byte((uint8_t*)(offset--), BYTE_INIT_VALUE);
	for (size_t i = 0; i < length; i++)
	{
		if (eeprom_read_byte((uint8_t*)(offset + i)) != ((uint8_t *)buffer)[i])
			eeprom_write_byte((uint8_t*)(offset + i), ((uint8_t *)buffer)[i]);			
	}
	return (0);
}

int	main(void)
{

	//usart_init();
	////i2c_init();
	//button_init();
	
	for (;;) {}
	
}
