#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "DISPLAY_7SEG_MUX/DISPLAY_7SEG_MUX.h"

int main(void) {
	// Inicializar display y timer
	init_display_mux();
	timer1_init();
	
	// Habilitar interrupciones globales
	sei();
	
	// Mostrar "1234" inicialmente
	update_display(1, 2, 3, 4);
	
	uint8_t counter = 0;
	
	while(1) {
		// Incrementar contador cada segundo
		_delay_ms(1000);
		counter++;
		
		// Mostrar contador (formato 00-99)
		update_display(counter/10, counter%10, 0, 0);
		
		// Resetear contador después de 99
		if(counter > 99) counter = 0;
	}
}