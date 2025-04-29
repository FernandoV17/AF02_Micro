#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "DISPLAY_7SEG_MUX/DISPLAY_7SEG_MUX.h"

int main(void) {
	// Inicializar display y timer
	init_display_mux();
	timer1_init();

	sei();
	
	update_display(0, 0, 0, 0);
	_delay_ms(1000);
	
	while(1) {
			update_display(1, 1, 1, 1);
			_delay_ms(1000);
			update_display(2, 2, 2, 2);
			_delay_ms(1000);
			update_display(3, 3, 3, 3);
			_delay_ms(1000);
			update_display(4, 4, 4, 4);
			_delay_ms(1000);
			update_display(5, 5, 5, 5);	
			_delay_ms(1000);
		// Loop principal vac?o
		// El refresco se maneja por interrupciones
	}
}