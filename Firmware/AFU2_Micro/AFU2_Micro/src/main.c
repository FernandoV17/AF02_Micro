#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "DISPLAY_7SEG_MUX/DISPLAY_7SEG_MUX.h"

int main(void) {
	init_display_mux();
	timer1_init();
	sei(); // Habilitar interrupciones
	
	while(1) {
		// Ejemplo: Mostrar "12:34"
		update_display(12, 34);
		_delay_ms(1000);
		// Ejemplo: Mostrar "56:78"
		update_display(56, 78);
		_delay_ms(1000);
	}
}