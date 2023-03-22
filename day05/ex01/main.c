/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:42:27 by gtoubol           #+#    #+#             */
/*   Updated: 2023/03/20 13:48:41 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "ft_usart.h"
#include "ft_i2c.h"

#define BYTE_ADDRESS_1 0x01
#define BYTE_ADDRESS_2 0x02
#define BYTE_ADDRESS_3 0x03
#define BYTE_ADDRESS_4 0x04
#define BYTE_SELECTED 0x05
#define BYTE_INIT_ADDRESS 0x00
#define BYTE_INIT_VALUE 0xCC

uint8_t data[4];
uint8_t selected = 0;

void	FT_printf(uint8_t n)
{
	PORTB = (n & 1<<PB0) | (n & 1<<PB1) | (n & 1<<PB2) | (n<<1 & 1<<PB4);
}

void	update_selected()
{
	selected++;
	selected = selected % 4;
	FT_printf(data[selected]);
	usart_printstr("here\r\n");
}

ISR(PCINT2_vect)
{
	cli();
	static int down = 0;
	
	if (down == 0)
	{
		data[selected]++;
		eeprom_write_byte((uint8_t*)(BYTE_ADDRESS_1 + selected), data[selected]);
		FT_printf(data[selected]);
		_delay_ms(1);
		down = 1;
	}
	else
		down = 0;
	EIFR |= 1;
	sei();
}

void button_init(void)
{
	//EICRA |= 0b10;				/* Enable button */
	//EIMSK |= 1;
	PCMSK2 |= (1 << PCINT20);
    PCICR |= (1 << PCIE2);
}

int	main(void)
{
	uint8_t button = 0;
	uint8_t prev = 0;
	uint8_t reset = 1;
	DDRB = 0x17;

	usart_init();
	i2c_init();
	button_init();
	sei();

	usart_printstr("Hello World\r\n");
	
	if (eeprom_read_byte((uint8_t*)BYTE_INIT_ADDRESS) != (uint8_t)BYTE_INIT_VALUE)
	{
		eeprom_write_byte((uint8_t*)BYTE_ADDRESS_1, 0);
		eeprom_write_byte((uint8_t*)BYTE_ADDRESS_2, 0);
		eeprom_write_byte((uint8_t*)BYTE_ADDRESS_3, 0);
		eeprom_write_byte((uint8_t*)BYTE_ADDRESS_4, 0);
		eeprom_write_byte((uint8_t*)BYTE_SELECTED, 0);
		eeprom_write_byte((uint8_t*)BYTE_INIT_ADDRESS, BYTE_INIT_VALUE);
	}
	data[0] = eeprom_read_byte((uint8_t*)BYTE_ADDRESS_1);
	data[1] = eeprom_read_byte((uint8_t*)BYTE_ADDRESS_2);
	data[2] = eeprom_read_byte((uint8_t*)BYTE_ADDRESS_3);
	data[3] = eeprom_read_byte((uint8_t*)BYTE_ADDRESS_4);
	selected = eeprom_read_byte((uint8_t*)BYTE_SELECTED);
	FT_printf(data[selected]);
	
	i2c_start();
	usart_dumpln("start: ", TWSR, "\r\n");
	i2c_start_send((0b0100000 << 1) | 0);
	usart_dumpln("start send: ", TWSR, "\r\n");
	i2c_write(0b00000000);
	i2c_stop();
	i2c_start();
	i2c_start_recv((0b0100000 << 1) | 1);
	
	for(;;)
	{
		i2c_read(1, &button);
		button = button & 0x01;
		//usart_dumpln("Button: ", button, "\r\n");
		if (button == 0 && reset == 1)
		{
			update_selected();
			reset = 0;
		}
		else if (button == 1 && prev == 1 && reset == 0)
			reset = 1;
		prev = button;
		_delay_ms(20);
	}

	i2c_stop();
}
