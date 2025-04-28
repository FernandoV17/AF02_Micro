/*
 * DISPLAY_7SEG_MUX.h
 *
 * Created: 15/04/2025 11:09:32 a. m.
 *  Author: fervi
 */ 
#ifndef DISPLAY_7SEG_MUX_H_
#define DISPLAY_7SEG_MUX_H_

#include <avr/io.h>

// Definiciones de pines - CORREGIDOS
#define display_7SEG_DDRX   DDRD
#define display_7SEG_PORTX  PORTD
#define display_mux_DDRX    DDRC
#define display_mux_PORTX   PORTC

// Pines de control de dígitos
#define DIG1 PC4
#define DIG2 PC3
#define DIG3 PC2
#define DIG4 PC1

// Variables compartidas
extern const uint8_t segment_map[];
extern volatile uint8_t display_buffer[4];
extern volatile uint8_t current_digit;

// Prototipos de funciones
void init_display_mux(void);
void timer1_init(void);
void update_display(uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4);

#endif