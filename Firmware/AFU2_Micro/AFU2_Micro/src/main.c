#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "DISPLAY_7SEG_MUX/DISPLAY_7SEG_MUX.h"

int main(void) {
	// Inicializar display y timer
	init_display_mux();
	timer1_init();
	
	update_display(1, 2, 3, 4);
	
	sei();
	
	while(1) {
		// Loop principal vac?o
		// El refresco se maneja por interrupciones
	}
}