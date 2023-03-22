/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgiband <lgiband@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:42:27 by gtoubol           #+#    #+#             */
/*   Updated: 2023/03/22 12:08:38 by lgiband          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ft_usart.h"

	//// Configurer la référence ADC sur AVCC
	//ADMUX |= (1 << REFS0);

	//// Configurer la résolution ADC à 8 bits
	//ADCSRA |= (1 << ADLAR) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);

	//// Configurer la broche d'entrée ADC sur PC0
	////ADMUX |= (1 << MUX0) ;//| (1 << MUX1) | (1 << MUX2);
	////ADMUX &= ~(1 << MUX3);
	////ADMUX &= ~(1 << MUX2);
	////ADMUX &= ~(1 << MUX1);
	////ADMUX &= ~(1 << MUX0);
	//// Activer l'ADC
	//ADCSRA |= (1 << ADEN);
	////ADCSRB |= (1 << ADTS0) ;//| (1 << ADTS1) | (1 << ADTS2);
	////ADCSRB &= ~(1 << ADTS0) ;//| (1 << ADTS1) | (1 << ADTS2);
	////ADCSRB |= (1 << ACME);

	////ACSR &= ~(1 << ACD);
	////ACSR &= ~(1 << ACBG);

// Fonction d'initialisation de l'ADC
void adc_init() {
	// Configurer la référence ADC sur AVCC
	ADMUX |= (1 << REFS0)  | (1 << ADLAR);

	// Configurer la résolution ADC à 8 bits
	ADCSRA |=  (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);

	// Configurer la broche d'entrée ADC sur PC0
	//ADMUX |= (1 << MUX1);

	// Activer l'ADC
	ADCSRA |= (1 << ADEN) | (1 << ADATE);
	//ADCSRB |= (1 << ADTS0);// | (1 << ADTS1) | (1 << ADTS2);

	// Démarrer une conversion ADC
	//ADCSRA |= (1 << ADSC);

	//// Attendre la fin de la conversion
	//while (ADCSRA & (1 << ADSC));
	ADCSRA |= (1 << ADSC);
}

// Fonction de lecture de la valeur du potentiomètre
uint8_t adc_read() {
	// Démarrer une conversion ADC
	//PRR &= ~(1 << PRADC);

	// Attendre la fin de la conversion
	//while (ADCSRA & (1 << ADSC));

	// Lire la valeur ADC
	return ADCH;
}



// Lire la valeur ADC
//uint8_t adc_value = ADCH;

int	main()
{
	uint8_t state = 0;
	uint8_t buffer[3] = {0, 0, 0};
	usart_init();
	adc_init();

	for(;;) {
		
        // Lecture de la valeur du potentiomètre
       // uint8_t val = adc_read();
        // Affichage de la valeur en hexadécimal
		ADMUX &= ~(1 << MUX3);
		ADMUX &= ~(1 << MUX2);
		ADMUX &= ~(1 << MUX1);
		ADMUX &= ~(1 << MUX0);			
		if (state == 1)
			ADMUX |= (1 << MUX0);
		else if (state == 2)
			ADMUX |= (1 << MUX1);
	   	buffer[state] = ADCH;
		if (state == 2) {
			usart_dumpln("", buffer[1], ", ");
			usart_dumpln("", buffer[2], ", ");
    		usart_dumpln("", buffer[0], "\r\n");
		}


        _delay_ms(20);
		state = (state + 1) % 3;
	}
	return (0);
}

