#include <avr/io.h>
#include <util/delay.h>

#include <avr/io.h>
#include <util/delay.h>

int main()
{
    // Configure Timer/Counter1 en mode PWM 8 bits
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00); // Fast PWM mode, non-inverting output on OC0B (PD5)
    TCCR0B |= (1 << CS01);  // Prescaler = 8

    TCCR2A |= (1 << COM2B1) | (1 << WGM21) | (1 << WGM20); // Fast PWM mode, non-inverting output on OC2B (PD3)
    TCCR2B |= (1 << CS21);  // Prescaler = 8

    // Configure PD5 en sortie
    DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);

    
	uint8_t i1 = 0;
	uint8_t i2 = 0;
	uint8_t i3 = 0;
    
	while (1) {
		OCR0B = i1;
    	OCR0A = i2;
    	OCR2B = i3;
		i1++;
		i2+=2;
		i3+=3;
		if (i1 == 256)
			i1 -= 255;
		if (i2 == 256)
			i2 -= 255;
		if (i3 == 256)
			i3 -= 255;
		_delay_ms(60);
        // Boucle infinie
    }
}