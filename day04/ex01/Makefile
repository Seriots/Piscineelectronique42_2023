
F_CPU=16000000UL
HEX = main.hex
BIN = main.bin
SRC = main.c

#simple makefile for ATmega328P with rule hex, flash, clean
all : hex flash
.NOTPARALLEL: all

#main.c to main.bin
#which -mmcu types is used for AtMega328 ?
#-Wall is used for warnings
#-Os is used for optimization
#-DF_CPU=$(F_CPU) is used for F_CPU
#main.c is the source file
#-o main.bin is the output file

#generate main.hex from main.bin

# -mmcu=atmega8 -Wall -Os
# -j .text -j .data
hex : $(BIN)

$(BIN): $(SRC)
	avr-gcc -mmcu=atmega328p -Os -Wl,-u,vfprintf -lprintf_flt -lm -DF_CPU=$(F_CPU) main.c -o main.bin
	avr-objcopy -O ihex main.bin main.hex


flash : $(HEX)
	avrdude -b 115200 -c arduino -p m328p -U flash:w:main.hex -P /dev/ttyUSB0

screen : hex flash
	python3 screen.py
.NOTPARALLEL: screen

clean :
	rm -f main.hex main.bin