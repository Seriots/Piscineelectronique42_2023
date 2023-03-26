/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_i2c.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 13:07:53 by gtoubol           #+#    #+#             */
/*   Updated: 2023/03/26 08:56:17 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_I2C_H
#define FT_I2C_H

#include <stdint.h>

#define I2C_START 0x08
#define I2C_RESTART 0x10

#define I2C_SLAW_ACK 0x18
#define I2C_SND_ACK 0x28
#define I2C_SND_NACK 0x30

#define I2C_SLAR_ACK 0x40
#define I2C_RCV_ACK 0x50
#define I2C_RCV_NACK 0x58

void i2c_init(void);
int i2c_start(void);
int i2c_restart(void);
void i2c_stop(void);
int i2c_start_send(uint8_t data);
int i2c_start_recv(uint8_t data);
int i2c_write(uint8_t data);
int	i2c_read(int mode, uint8_t *dest);
int	i2c_clock_clear(void);
int	i2c_clock_init(void);
int	i2c_clock_connect(void);
int	i2c_clock_set_all(uint8_t data);

#endif /* FT_I2C_H */
