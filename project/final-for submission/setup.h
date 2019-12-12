/*
 * setup.h
 *
 * Created: 12/3/2019 
 *  Author: Sean Gow & Denice Hickethier
 */ 


#ifndef SETUP_H_
#define SETUP_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void eventtrigger();

void system_clocks();
//void rtc32_clock();
void timer();
void cap_comp();
void gpio_setup();
void setup_spi();
void setup_timer();

void setup_compare_timer();
//motor pwm
void tcd_pwm();
void setup_dmac(uint32_t const src, uint32_t const dst);
void tcd0_pwm_control(uint8_t time, uint16_t t_scale, float duty_cycle);
unsigned long calc_period(uint8_t time, uint16_t t_scale);
void tce_setup();

#endif /* SETUP_H_ */