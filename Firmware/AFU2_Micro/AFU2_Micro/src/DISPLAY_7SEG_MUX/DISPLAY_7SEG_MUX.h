/*
 * MUXDISPLAY7S.h
 *
 * Created: 15/04/2025 11:09:32 a. m.
 *  Author: fervi
 */ 


#ifndef DISPLAY_7SEG_MUX_H_
#define DISPLAY_7SEG_MUX_H_

#include <avr/io.h>

// Configuración para cátodo común (Segmentos se encienden con HIGH)
#define display_7SEG_DDRX   DDRB
#define display_7SEG_PORTX  PORTB

// Pines de segmentos (Ajustar según conexión física)
#define seg_a   PB0
#define seg_b   PB1
#define seg_c   PB2
#define seg_d   PB3
#define seg_e   PB4
#define seg_f   PB5
#define seg_g   PB6
#define dp      PB7  // Puntos

// Pines de control de dígitos (Activación con LOW)
#define display_mux_DDRX    DDRC
#define display_mux_PORTX   PORTC
#define DIG1    PC1  // Dígito 1 (LEFT)
#define DIG2    PC2
#define DIG3    PC3
#define DIG4    PC4  // Dígito 4 (RIGHT)

// Variables globales compartidas
extern volatile uint8_t display_buffer[4];
extern volatile uint8_t dp_position;

// Prototipos de funciones
void init_display_mux(void);
void write_display_number(uint8_t number);
void write_mux_display_number(uint8_t digit, uint8_t value, uint8_t show_dp);
void display_refresh(void);          
void timer1_init(void);              

#endif