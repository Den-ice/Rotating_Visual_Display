/*
 *  Physics 402 Final Project 
 *  Persistence of Vision Display 
 *  11/26/2019
 *  Author: Sean Gow & Denice Hickethier
 * 
 * Components Used:
 * xmega 256a3bu, CPU@32MHz
 * multistar 32bit esc drone motor controller, using pwm from tcd0 on port J4 PIN5
 * Hall effect sensor on PortA pin 2 routed through the on board analog filter, and pulldown resistor to increase the frequency of  low-pass cutoff
 * Event system capture of revolution timing
 ** DotStar LEDS/APA102 using USARTC0 Master SPI.
 */ 

#define F_CPU 32000000L  //for wait util
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "setup.h"
void init_motor();

void alpha(int i);


//led string frame signals
uint8_t startframe=0x00;
uint8_t stopframe=0xff;

//led individual signals
uint8_t ledframe=0XE0;
uint8_t brightness=4;
uint8_t blue=255;
uint8_t red=255;
uint8_t green=255;

volatile uint8_t sync=0;

ISR( PORTA_INT0_vect ){
	PORTR_OUT^=1;//analog filter troubleshooter
	sync=1;

//testing
//TCE0_PER=(uint32_t)(	TCE0_PER+TCF0_CCABUF)>>1;
//TCE1_PER=TCE0_PER/2;
//TCE1_CCA=TCE0_PER/4;
	//TCF0_CCC=TCF0.CCABUF/360;
//	TCE0_CCA=TCF0.CCABUF/360;
	//DMA.CTRL=DMA_ENABLE_bm;
//	DMA.CH0.CTRLA|=DMA_CH_ENABLE_bm|DMA_CH_BURSTLEN_1BYTE_gc;
//	USARTC0_DATA=0X01;
//DMA.CH0.CTRLA|=DMA_CH_TRFREQ_bm;
	//blinkoff();
  }

ISR(BADISR_vect){
	
}

int main(){

	cli();

	system_clocks();
	tcd_pwm();
	tce_setup();
	gpio_setup();
	setup_spi();	
 // setup_dmac((uint32_t) &data,(uint32_t)  &USARTC0_DATA);
	eventtrigger();
	PMIC_CTRL |= PMIC_HILVLEN_bm |PMIC_LOLVLEN_bm|PMIC_MEDLVLEN_bm;
	sei();
	
	_delay_ms(100);
	//DMA.CH0.CTRLA=DMA_CH_TRFREQ_bm;

	init_motor();
	
	_delay_ms(300);
	while(1){

		if(sync==1){
			sync=0;
			//H
			alpha(0);
			blinkoff();
			_delay_us(700);
			alpha(1);
			
			blinkoff();
			_delay_us(700);
			alpha(1);
			
			blinkoff();
			_delay_us(700);
			alpha(0);
			
			blinkoff();
			_delay_ms(3);
			//E
			alpha(0);
			blinkoff();
			_delay_us(700);
			alpha(2);
			blinkoff();		
			_delay_us(700);
			alpha(2);
			blinkoff();	
			_delay_ms(3);
			
			//L
			alpha(0);
			blinkoff();
			_delay_us(700);
			alpha(3);
			blinkoff();
			_delay_us(700);
			alpha(3);
			blinkoff();		
			_delay_ms(3);
			//L
			alpha(0);
			blinkoff();
			_delay_us(700);
			alpha(3);
			blinkoff();
			_delay_us(700);
			alpha(3);
			blinkoff();
			

					//o
					
			_delay_us(2500);
			alpha(4);
			blinkoff();
			_delay_us(700);
			alpha(5);
			blinkoff();
			_delay_us(700);
			alpha(5);
			blinkoff();
			_delay_us(700);
			alpha(4);
			blinkoff();
			_delay_us(200);
		
							blinkoff();
			
			blinkoff();
			
			blinkoff();
			
			blinkoff();
					sync=0;
		}
	}

}



void init_motor(){
	
	//initialize motor
	tcd0_pwm_control(20,1000,5);
	_delay_ms(2500);
	tcd0_pwm_control(20,1000,5.2);
	_delay_ms(400);
	tcd0_pwm_control(20,1000,5.3);
	_delay_ms(400);
	tcd0_pwm_control(20,1000,5.4);
	_delay_ms(300);
//cut off by analog lowpass filter
//	tcd0_pwm_control(20,1000,5.5);
//	_delay_ms(200);

//tcd0_pwm_control(20,1000,5.6);
	///_delay_ms(200);

    //motor gets warm
	/*tcd0_pwm_control(20,1000,5.7);
	_delay_ms(200);
*/
}




/* manually transmitting led signals
*/

void blinkoff(){
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe|0;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;

	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;

	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;

	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;

	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;

	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;

	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=0;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=stopframe;

}



/*
Due to timing and performance constraints Manually string the sequences together was the route that worked the best.
Dynamically generating a signal for 20x Leds with 32bits each resulted in incomplete characters
TODO: offload transmission  to dma

line pattern components for H,E,L,O
*/
void alpha(int i){
	if (i==0){
	//outside of H,L
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	
	
	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	

	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe|30;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe|30;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
USARTC0_DATA=255;

	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe|30;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe|30;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe|30;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;

	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe|30;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	

	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	

while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;

	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=stopframe;

	
	
	
	}
	else if( i==1){
    //middle of H		
		
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	
	
	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	

	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
USARTC0_DATA=255;

	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe|30;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;

	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	

	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	

while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;

	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=stopframe;

		
		
		

		
	}
	else if( i==2){
		//inside part of E
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=startframe;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=startframe;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=startframe;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=startframe;
		
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		

		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe|30;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=green;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;

		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=blue;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe|30;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=green;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;

		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe|30;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=blue;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		

		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		

		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=green;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;

		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=blue;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=green;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=stopframe;

	}else if( i==3){
		//bottom of L
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=startframe;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=startframe;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=startframe;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=startframe;
		
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		

		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=green;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;

		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=blue;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=green;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;

		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe|30;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=blue;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		

		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		

		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=green;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;

		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=blue;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=red;
		
		
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe;//0 brightness=OFF
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=green;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=255;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=stopframe;
	
	}
	else 	if (i==4){
	//left/right of O
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	
	
	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	

	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe|30;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
USARTC0_DATA=255;

	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe|30;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe|30;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe|30;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;

	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	

	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	

while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;

	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=stopframe;	



	
	
	
	}else 	if (i==5){
//top/bottom of O	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	
	
	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	

	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe|30;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
USARTC0_DATA=255;

	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;

	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe|30;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	

	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	

while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;

	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;
	
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=ledframe;//0 brightness=OFF
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=255;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=stopframe;
	
	
	
	
	}
}
