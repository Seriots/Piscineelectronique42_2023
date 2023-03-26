/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_i2c.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 13:04:50 by gtoubol           #+#    #+#             */
/*   Updated: 2023/03/26 08:55:52 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_i2c.h"
#include <stdint.h>
#include <avr/io.h>

void	i2c_init(void)
{
	TWBR = 72;	/* base communication frequency  = 72 */
}

int	i2c_start(void)
{
	TWCR = (1 << TWINT)|(1 << TWSTA)|(1 << TWEN) ;
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != I2C_START)
		return (1);
	return (0);
}

int	i2c_restart(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN) ;
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xf8) != I2C_RESTART)
		return (1);
	return (0);
}

void	i2c_stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO) ;
}

int	i2c_start_send(uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN) ;
	while (! (TWCR & (1 << TWINT))) ;
	if ((TWSR & 0xf8) != I2C_SLAW_ACK)
		return (1);
	return (0);
}

int	i2c_start_recv(uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) ;
	while (! (TWCR & (1 << TWINT))) ;
	if ((TWSR & 0xf8) != I2C_SLAR_ACK)
		return (1);
	return (0);
}

int	i2c_write(uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (! (TWCR & (1 << TWINT))) ;
	if ((TWSR & 0xf8) != I2C_SND_ACK)
		return (1);
	return (0);
}

int	i2c_read(int mode, uint8_t *dest)
{
	TWCR = (1 << TWINT) | ((mode? 1 : 0) << TWEA) | (1 << TWEN);
	while (! (TWCR & (1 << TWINT))) ;
	*dest = TWDR;
	if ((TWSR & 0xf8) != (mode? I2C_RCV_ACK: I2C_RCV_NACK))
		return (1);
	return (0);
}

int	i2c_clock_clear(void)
{
	if (i2c_start()
		|| i2c_start_send((0b0100000 << 1) | 0)
		|| i2c_write(0b010)
		|| i2c_write(0x0F)
		|| i2c_write(0x00))
		return (1);

	i2c_stop();
	return (0);
}

int	i2c_clock_init(void)
{
	if (i2c_start()
		|| i2c_start_send((0b0100000 << 1) | 0)
		|| i2c_write(0b110)
		|| i2c_write(0b00000001) //set SW3 to input and all others to output
		|| i2c_write(0b00000000))
		return (1);
	
	i2c_stop();
	return (0);
}

int	i2c_clock_connect(void)
{
	if (i2c_start()
		|| i2c_start_send((0b0100000 << 1) | 0)
		|| i2c_write(0b010))
		return (1);

	return (0);
}

int	i2c_clock_set_all(uint8_t data)
{
	if (i2c_write(0x0F)
		|| i2c_write(data))
		return (1);

	i2c_stop();
	return (0);
}

