#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "DISPLAY_7SEG_MUX/DISPLAY_7SEG_MUX.h"


// Variables globales
volatile uint8_t horas = 0, minutos = 0, segundos = 0;
volatile uint16_t ms_counter = 0; // Contador de milisegundos

void actualizar_reloj_display() {
	update_display(horas / 10, horas % 10, minutos / 10, minutos % 10);
}

ISR(TIMER1_COMPA_vect) {
	// --- Multiplexado del display (cada 5ms) ---
	display_mux_PORTX &= ~((1 << DIG1) | (1 << DIG2) | (1 << DIG3) | (1 << DIG4)); // Apagar dígitos
	display_7SEG_PORTX = segment_map[display_buffer[current_digit]]; // Mostrar dígito actual
	switch(current_digit) {
		case 0: display_mux_PORTX |= (1 << DIG1); break;
		case 1: display_mux_PORTX |= (1 << DIG2); break;
		case 2: display_mux_PORTX |= (1 << DIG3); break;
		case 3: display_mux_PORTX |= (1 << DIG4); break;
	}
	current_digit = (current_digit + 1) % 4; // Rotar dígito

	// --- Contador de segundos (cada 1000ms) ---
	ms_counter += 5; // Cada interrupción suma 5ms
	if (ms_counter >= 1000) {
		ms_counter = 0;
		segundos++;
		if (segundos >= 60) {
			segundos = 0;
			minutos++;
			if (minutos >= 60) {
				minutos = 0;
				horas++;
				if (horas >= 24) horas = 0;
			}
		}
		actualizar_reloj_display(); // Actualizar display cada segundo
	}
}

int main(void) {
	init_display_mux();
	timer1_init();
	actualizar_reloj_display(); // Mostrar 00:00 inicial
	sei(); // Habilitar interrupciones

	while(1) {} // Todo se maneja por interrupciones
}