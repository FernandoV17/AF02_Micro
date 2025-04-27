#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "DISPLAY_7SEG_MUX/DISPLAY_7SEG_MUX.h"
#include "BUTTON/BUTTON.h"

// Variables globales
volatile uint16_t counter = 0;  // Cambiado de uint8_t a uint16_t para soportar hasta 9999
volatile uint8_t display_values[4] = {0, 0, 0, 0};

void update_display_values(void) {
	display_values[0] = counter / 1000;        // Millares
	display_values[1] = (counter / 100) % 10;  // Centenas
	display_values[2] = (counter / 10) % 10;   // Decenas
	display_values[3] = counter % 10;          // Unidades
}

int main(void) {
	// Inicializar display, timer y botones
	init_display_mux();
	timer1_init();
	buttons_init();
	
	// Valores iniciales - ahora con el tipo correcto
	counter = 1234;
	update_display_values();
	update_display(display_values[0], display_values[1],
	display_values[2], display_values[3]);
	
	// Habilitar interrupciones globales
	sei();
	
	while(1) {
		// Actualizar estados de los botones
		buttons_update();
		
		// Botón 1 (PD2) - Incrementar
		if(button1_pressed()) {
			if(counter < 9999) counter++;
			update_display_values();
		}
		
		// Botón 2 (PD3) - Decrementar
		if(button2_pressed()) {
			if(counter > 0) counter--;
			update_display_values();
		}
		
		// Botón 3 (PD4) - Reset
		if(button3_pressed()) {
			counter = 0;
			update_display_values();
		}
		
		// Actualizar display con los nuevos valores
		update_display(display_values[0], display_values[1],
		display_values[2], display_values[3]);
		
		// Pequeño retardo para evitar rebotes adicionales
		_delay_ms(10);
	}
}