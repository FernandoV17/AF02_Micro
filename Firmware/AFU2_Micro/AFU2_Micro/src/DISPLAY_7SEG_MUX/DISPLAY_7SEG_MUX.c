/*
 * mux_display_7seg.c
 *
 * Created: 15/04/2025 11:10:43 a. m.
 *  Author: fervi
 */ 

#include "DISPLAY_7SEG_MUX.h"
#include <avr/interrupt.h>

/*
   Mapa de bits para display de 7 segmentos (CÁTODO COMÚN)

    Segmento:   Bit:    Visual:
       A         0       ---
      F B      5   1    |   |
       G         6       ---
      E C      4   2    |   |
       D         3       ---

   Orden de bits: 0b0GFEDCBA
*/

const uint8_t segment_map[] = {
	0b11000000, // 0  A B C D E F
	0b11111001, // 1    B C         
	0b10100100, // 2  A B   D E   G
	0b10110000, // 3  A B C D     G
	0b10011001, // 4    B C     F G
	0b10010010, // 5  A   C D   F G
	0b10000010, // 6  A   C D E F G
	0b11111000, // 7  A B C         
	0b10000000, // 8  A B C D E F G
	0b10010000  // 9  A B C D   F G
};


volatile uint8_t display_buffer[4] = {0};
volatile uint8_t current_digit = 0;

void init_display_mux(void) {
	// Configurar segmentos como salidas (PB0-PB7)
	display_7SEG_DDRX = 0xFF;
	
	// Configurar dígitos del mux como salidas (PC1-PC4)
	display_mux_DDRX |= (1 << DIG1) | (1 << DIG2) | (1 << DIG3) | (1 << DIG4);
	
	// Apagar todos los dígitos inicialmente (LOW para desactivar mux)
	display_mux_PORTX &= ~((1 << DIG1) | (1 << DIG2) | (1 << DIG3) | (1 << DIG4));
}

void update_display(uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4) {
	display_buffer[0] = d1;
	display_buffer[1] = d2;
	display_buffer[2] = d3;
	display_buffer[3] = d4;
}

ISR(TIMER1_COMPA_vect) {
	// 1. Apagar todos los dígitos (poner en LOW los pines del mux)
	display_mux_PORTX &= ~((1 << DIG1) | (1 << DIG2) | (1 << DIG3) | (1 << DIG4));
	
	// 2. Preparar segmentos para el dígito actual
	display_7SEG_PORTX = segment_map[display_buffer[current_digit]];
	
	// 3. Activar solo el dígito actual (HIGH en el pin del mux correspondiente)
	switch(current_digit) {
		case 0: display_mux_PORTX |= (1 << DIG1); break;
		case 1: display_mux_PORTX |= (1 << DIG2); break;
		case 2: display_mux_PORTX |= (1 << DIG3); break;
		case 3: display_mux_PORTX |= (1 << DIG4); break;
	}
	
	// 4. Rotar al siguiente dígito
	current_digit = (current_digit + 1) % 4;
}

void timer1_init(void) {
	TCCR1A = 0;
	TCCR1B = (1 << WGM12) | (1 << CS11); // Modo CTC, prescaler 8
	OCR1A = 9999; // 16MHz/(8 * 10000) = 200Hz (5ms ciclo completo)
	TIMSK1 = (1 << OCIE1A);
}