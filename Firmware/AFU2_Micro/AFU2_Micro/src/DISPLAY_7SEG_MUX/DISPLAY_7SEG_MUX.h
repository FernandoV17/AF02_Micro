/*
 * DISPLAY_7SEG_MUX.h
 *
 * Created: 15/04/2025 11:09:32 a. m.
 *  Author: fervi
 */ 


#ifndef DISPLAY_7SEG_MUX_H_
#define DISPLAY_7SEG_MUX_H_

#include <avr/io.h>

// Definiciones de pines
#define display_7SEG_DDRX   DDRD
#define display_7SEG_PORTX  PORTD

#define display_mux_DDRX    DDRC
#define display_mux_PORTX   PORTC

// Pines de control de d�gitos (ajustar seg�n conexi�n f�sica)
#define DIG1 PC1
#define DIG2 PC2
#define DIG3 PC3
#define DIG4 PC4

// Prototipos de funciones
void init_display_mux(void);
void timer1_init(void);
void update_display(uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4);

#endif /* DISPLAY_7SEG_MUX_H_ */