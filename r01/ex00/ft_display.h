/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_display.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 08:46:19 by lgiband           #+#    #+#             */
/*   Updated: 2023/03/26 15:21:39 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DISPLAY_H
# define FT_DISPLAY_H

uint8_t	get_digit(uint8_t digit);
uint8_t	get_value(uint8_t digit);
void	print_digit(uint8_t value, uint8_t digit);
void	print_digit_one_time(uint8_t value, uint8_t digit);
void	print_nbr(uint16_t nbr, uint8_t led);
void	print_tmcu(uint16_t nbr, uint8_t led);
void	print_42(uint8_t led);
void	print_text(uint16_t nbr, uint8_t led, uint8_t unite);
void	print_date(uint16_t nbr, uint8_t led);
void	print_msg(uint8_t *msg, uint8_t led);


#define TOP 0x01
#define TOP_RIGHT 0x02
#define BOTTOM_RIGHT 0x04
#define BOTTOM 0x08
#define BOTTOM_LEFT 0x10
#define TOP_LEFT 0x20
#define MIDDLE 0x40
#define DOT 0x80

#endif