
F_CPU=1000000L

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
hex :
	avr-gcc -mmcu=atmega328p -DF_CPU=$(F_CPU) main.c -o main.bin
	avr-objcopy -O ihex main.bin main.hex


flash : 
	avrdude -b 115200 -c arduino -p m328p -U flash:w:main.hex -P /dev/ttyUSB0

clean :
	rm -f main.hex main.bin