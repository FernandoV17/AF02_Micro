#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/delay_basic.h>  // para _delay_loop_2
#include "DISPLAY_7SEG_MUX/DISPLAY_7SEG_MUX.h"
#include "BUTTON/BUTTON.h"

// Notas musicales (frecuencias en Hz)
#define NOTE_E5 659
#define NOTE_DS5 622
#define NOTE_B4 494
#define NOTE_D5 587
#define NOTE_C5 523
#define NOTE_A4 440

// Melodía de ejemplo

uint16_t melody[] = {
	NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_DS5, NOTE_E5,
	NOTE_B4, NOTE_D5, NOTE_C5, NOTE_A4
};

uint16_t note_durations[] = {
	200, 200, 200, 200, 400,
	200, 200, 200, 400
};
	
#define MELODY_LEN (sizeof(melody)/sizeof(melody[0]))

// Variables globales
volatile uint8_t minutes = 0;
volatile uint8_t seconds = 0;
volatile uint16_t ms_count = 0;
volatile uint8_t second_flag = 0;
enum TimerState { CONFIG = 0, RUNNING, BUZZ } state;
uint8_t tone_played = 0;

// Timer0 @1ms CTC
void timer0_init_1ms(void) {
	TCCR0A = (1 << WGM01);
	OCR0A  = 249;  // (16MHz / 64 / 1000) - 1
	TCCR0B = (1 << CS01) | (1 << CS00);
	TIMSK0 = (1 << OCIE0A);
}

ISR(TIMER0_COMPA_vect) {
	ms_count++;
	if (ms_count >= 1000) {
		ms_count = 0;
		second_flag = 1;
	}
}

// Reproduce un tono usando _delay_loop_2
void play_tone(uint16_t freq, uint16_t duration_ms) {
	// Número de ciclos de CPU para medio periodo
	uint32_t half_cycles = (F_CPU / (2UL * freq));
	// _delay_loop_2 tarda 4 ciclos por iteración
	uint16_t iter = half_cycles / 4;
	// Número total de pulsos
	uint32_t pulses = ((uint32_t)freq * duration_ms) / 1000;
	for (uint32_t i = 0; i < pulses; i++) {
		PORTC |=  (1 << PC0);
		_delay_loop_2(iter);
		PORTC &= ~(1 << PC0);
		_delay_loop_2(iter);
	}
}

// Reproduce la melodía completa
void play_melody(void) {
	for (uint8_t i = 0; i < MELODY_LEN; i++) {
		play_tone(melody[i], note_durations[i]);
		_delay_ms(50);
	}
}

int main(void) {
	// Configurar buzzer
	DDRC |= (1 << PC0);
	PORTC &= ~(1 << PC0);

	cli();
	init_display_mux();
	timer1_init();
	buttons_init();
	timer0_init_1ms();
	state = CONFIG;
	sei();

	while (1) {
		buttons_update();
		switch (state) {
			case CONFIG:
			if (button1_pressed() && button2_pressed()) {
				state = RUNNING;
				second_flag = 0;
				} else if (button1_pressed()) {
				seconds = (seconds + 1) % 60;
				} else if (button2_pressed()) {
				minutes = (minutes + 1) % 60;
			}
			tone_played = 0;
			break;

			case RUNNING:
			if (second_flag) {
				second_flag = 0;
				if (minutes == 0 && seconds == 0) {
					state = BUZZ;
					} else {
					if (seconds > 0) seconds--;
					else { seconds = 59; minutes--; }
				}
			}
			break;

			case BUZZ:
			if (!tone_played) {
				play_melody();
				tone_played = 1;
				PORTC |= (1 << PC0);
			}
			if (button1_pressed() || button2_pressed()) {
				PORTC &= ~(1 << PC0);
				minutes = seconds = 0;
				state = CONFIG;
			}
			break;
		}
		// Actualiza display MM:SS
		update_display(minutes / 10,
					   minutes % 10, 
					   seconds / 10, 
					   seconds % 10);
	}
	return 0;
}
