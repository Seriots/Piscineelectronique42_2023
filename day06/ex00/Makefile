# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/13 19:14:11 by gtoubol           #+#    #+#              #
#    Updated: 2023/03/21 11:31:20 by lgiband          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DEVICE		?= /dev/ttyUSB0
F_CPU		?= 16000000
BAUD		?= 115200
ID			?= 1

# List of all sources (.c)
# -------------------------------------------------------------------------
SRCS		= main.c ft_usart.c

# List of all object files (.o)
# -------------------------------------------------------------------------

# Compilation options
# -------------------------------------------------------------------------
CC			= avr-gcc
CFLAGS		= -Wall -Wextra -Werror -DF_CPU=$(F_CPU) -DBAUD=$(BAUD) -DG_ADDR=$(ID)  -Os
SHELL		= /bin/bash

# Rules
# -------------------------------------------------------------------------
.PHONY:		all
all:		hex flash watch

hex:
			$(CC) $(CFLAGS) $(SRCS) -o main.bin -mmcu=atmega328p
			avr-objcopy -O ihex main.bin main.hex

flash:
			avrdude -p m328p -b $(BAUD)  -c arduino -P $(DEVICE) -U flash:w:main.hex

titi:
			DEVICE=/dev/ttyUSB0 ID=2 make
			DEVICE=/dev/ttyUSB1 ID=1 make

watch:
			python3 screen.py


clean:
			$(RM) main.bin main.hex

# Parameters
# -------------------------------------------------------------------------
_GREY		= \033[30m
_RED		= \033[31m
_GREEN		= \033[32m
_YELLOW		= \033[33m
_BLUE		= \033[34m
_PURPLE		= \033[35m
_CYAN		= \033[36m
_WHITE		= \033[37m
_NO_COLOR	= \033[0m
