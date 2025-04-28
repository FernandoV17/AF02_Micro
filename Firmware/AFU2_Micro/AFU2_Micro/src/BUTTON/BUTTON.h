/*
 * BUTTON.h
 *
 * Created: 26/04/2025 05:44:47 p. m.
 *  Author: fervi
 */ 

#ifndef BUTTONS_H
#define BUTTONS_H
#define F_CPU 16000000UL

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#define BUTTON1_PIN PD2
#define BUTTON2_PIN PD3
#define BUTTON3_PIN PD4

#define BUTTON1_MASK (1 << BUTTON1_PIN)
#define BUTTON2_MASK (1 << BUTTON2_PIN)
#define BUTTON3_MASK (1 << BUTTON3_PIN)

#define BUTTONS_READ (PIND & (BUTTON1_MASK | BUTTON2_MASK | BUTTON3_MASK))

// Prototipos de funciones
void buttons_init(void);
uint8_t button1_pressed(void);
uint8_t button2_pressed(void);
uint8_t button3_pressed(void);
void buttons_update(void);

#endif // BUTTONS_H