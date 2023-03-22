#ifndef FT_USART_H
#define FT_USART_H

#include <stdint.h>

void usart_init(void);
void usart_tx(uint8_t data);
uint8_t usart_rx(void);
void usart_printstr(const char *str);
void usart_dump(uint8_t data);
void usart_dumpln(const char *prompt, uint8_t data, const char *end);
void usart_dump16ln(const char *prompt, uint16_t data, const char *end);
void	usart_int16ln(uint16_t data);

#endif /* FT_USART_H */
