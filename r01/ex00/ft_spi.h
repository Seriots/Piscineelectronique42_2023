/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_spi.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 11:48:35 by lgiband           #+#    #+#             */
/*   Updated: 2023/03/26 12:20:12 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SPI_H
# define FT_SPI_H

void SPI_MasterInit(void);
void	SPI_reset(void);
void SPI_MasterTransmit(uint8_t cData);
void transmitLed(uint32_t data);
void boudary_frame(uint8_t bound);
#endif