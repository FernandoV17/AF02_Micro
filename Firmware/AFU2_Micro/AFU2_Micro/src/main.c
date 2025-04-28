#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "DISPLAY_7SEG_MUX/DISPLAY_7SEG_MUX.h"

// Variables globales para el reloj regresivo
volatile uint8_t minutos = 59, segundos = 59;  // Inicia en 59:59
volatile uint8_t alarma_activada = 0;          // Bandera para alarma

void actualizar_reloj_display() {
	update_display(minutos / 10,   // Primer dígito minutos
	minutos % 10,    // Segundo dígito minutos
	segundos / 10,  // Primer dígito segundos
	segundos % 10); // Segundo dígito segundos
}

ISR(TIMER1_COMPA_vect) {
	// Multiplexado
	display_mux_PORTX &= ~((1 << DIG1) | (1 << DIG2) | (1 << DIG3) | (1 << DIG4));
	display_7SEG_PORTX = (segment_map[display_buffer[current_digit]] << 1);
	
	switch(current_digit) {
		case 0: display_mux_PORTX |= (1 << DIG1); break;
		case 1: display_mux_PORTX |= (1 << DIG2); break;
		case 2: display_mux_PORTX |= (1 << DIG3); break;
		case 3: display_mux_PORTX |= (1 << DIG4); break;
	}
	current_digit = (current_digit + 1) % 4;
	
	// Contador de tiempo REGRESIVO
	static uint16_t ms_count = 0;
	ms_count += 5;
	
	if(ms_count >= 1000) {
		ms_count = 0;
		
		if(!alarma_activada) {
			if(segundos > 0) {
				segundos--;  // Decrementa segundos
				} else {
				if(minutos > 0) {
					minutos--;    // Decrementa minutos
					segundos = 59; // Reinicia segundos
					} else {
					// Al llegar a 00:00
					alarma_activada = 1;
					// Opcional: Aquí puedes activar un buzzer o LED
				}
			}
			actualizar_reloj_display();
		}
	}
	
	// Parpadeo cuando llega a 00:00
	if(alarma_activada && (ms_count % 400 < 200)) {
		display_mux_PORTX &= ~((1 << DIG1) | (1 << DIG2) | (1 << DIG3) | (1 << DIG4));
	}
}

int main(void) {
	// Inicialización
	init_display_mux();
	timer1_init();
	
	// Valores iniciales (59:59)
	minutos = 59;
	segundos = 59;
	alarma_activada = 0;
	actualizar_reloj_display();
	
	// Habilitar interrupciones
	sei();
	
	// Bucle principal
	while(1) {
		// Todo se maneja por interrupciones
		// Opcional: Aquí puedes agregar control por botones
	}
}