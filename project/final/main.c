/*
Physics 402 Final Project 
*/

#define F_CPU 32000000L  //for wait util
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#include "setup.h"
void frame();
void leds();
void init_motor();

void alpha(int i);

//volatile uint8_t data[88];





uint8_t startframe=0x00;
uint8_t ledframe=0XE0;
uint8_t brightness=4;

uint8_t blue=255;
uint8_t red=255;
uint8_t green=255;
uint8_t stopframe=0xff;
volatile uint8_t line;

//max is 256k
//(1 stored display pattern- 32*360=11520bits)

//fixed patterns
uint32_t ledpattern[360];

//all on


volatile uint8_t sync=0;

int rampup=1;
float duty=5;

uint8_t count=0;
volatile on=1;
uint16_t cycle_dur=0;
volatile cnt=0;

volatile int bitstream;



ISR( PORTA_INT0_vect ){
	
	on^=1;
	PORTR_OUT^=1;
	//TCE0_PER=(uint32_t)(	TCE0_PER+TCF0_CCABUF)>>1;
	sync=1;
//TCE1_PER=TCE0_PER/2;
//TCE1_CCA=TCE0_PER/4;


	//TCF0_CCC=TCF0.CCABUF/360;
//	TCE0_CCA=TCF0.CCABUF/360;
	//DMA.CTRL=DMA_ENABLE_bm;
//	DMA.CH0.CTRLA|=DMA_CH_ENABLE_bm|DMA_CH_BURSTLEN_1BYTE_gc;
//	USARTC0_DATA=0X01;
//DMA.CH0.CTRLA|=DMA_CH_TRFREQ_bm;
	//frame();
	//TRIGGER LENGTH OF FRAME CAPTURE;
	//frame();
	//blinkoff();
  }
ISR(RTC32_OVF_vect){
}

ISR (TCE0_OVF_vect){	
    }
	
ISR(TCE0_CCA_vect){

	}
		
ISR(TCE0_CCB_vect){ 
	

}
ISR(TCE0_CCC_vect){
	
	}


ISR (TCE1_OVF_vect){	
    }
	
ISR(TCE1_CCA_vect){
	}
		
ISR(TCE1_CCB_vect){ }
	
	


ISR (TCF0_OVF_vect){	
	
    }
	
ISR(TCF0_CCA_vect){

	}
		
ISR(TCF0_CCB_vect){ 
	

}
ISR(TCF0_CCC_vect){
	
	}

ISR(DMA_CH0_vect){

DMA.CH0.CTRLA&=~DMA_CH_TRNIF_bm;
}
 //Interrupt Service Handlers 
ISR(USARTC0_RXC_vect){
}

ISR(USARTC0_TXC_vect){
}
ISR(USARTC0_DRE_vect){
}
ISR(ADCA_CH0_vect){
}
ISR(ADCB_CH0_vect){
}
ISR(BADISR_vect){
	
}

ISR( PORTC_INT0_vect ){
}

int main(){
/*
	ledpattern[0 ]=0b00000000000000000000;
	ledpattern[1 ]=0b00000000000000000000;
	ledpattern[2 ]=0b00000000000000000000;
	ledpattern[3 ]=0b00000000011111111100;
	ledpattern[4 ]=0b00000000000011000000;
	ledpattern[5 ]=0b00000000011111111100;
	ledpattern[6 ]=0b00000000000000000000;
	
	
	
	
	
	
	ledpattern[7 ]=0b00000000011111111100;
	ledpattern[8 ]=0b00000000010001000100;
	ledpattern[9 ]=0b00000000010001000100;
	ledpattern[10]=0b00000000000000000000;
	
	
	
	
	
	
	
	ledpattern[11]=0b00000000011111111100;
	ledpattern[12]=0b00000000010000000000;
	ledpattern[13]=0b00000000010000000000;
	
	
	
	
	
	
	ledpattern[14]=0b00000000000000000000;
	ledpattern[15]=0b00000000011111111000;
	ledpattern[16]=0b00000000010000000000;
	ledpattern[17]=0b00000000010000000000;
	
	
	
	
	
	
	
	
	
	ledpattern[18]=0b00000000000000000000;
	ledpattern[19]=0b00000000001111111000;
	ledpattern[20]=0b00000000010000000100;
	ledpattern[21]=0b00000000010000000100;
	ledpattern[22]=0b00000000001111111100;
	ledpattern[23]=0b00000000000000000000;
	ledpattern[24]=0b00000000000000000000;
	ledpattern[25]=0b00000000000000000000;
	ledpattern[26]=0b00000000000000000000;
	ledpattern[27]=0b00000000000000000000;
	ledpattern[28]=0b00000000000000000000;
	ledpattern[20]=0b00000000000000000000;
*/

	//generate_frame();
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
	
	//while(!(USARTC0_STATUS& USART_DREIF_bm));
	//USARTC0_DATA=stopframe;
	//DMA.CH0.CTRLA=DMA_CH_TRFREQ_bm;
	//PORTC.PIN0CTRL=PORT_INVEN_bm;
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
//	tcd0_pwm_control(20,1000,5.5);
//	_delay_ms(200);
//tcd0_pwm_control(20,1000,5.6);
	///_delay_ms(200);
	/*tcd0_pwm_control(20,1000,5.7);
	_delay_ms(200);
*/
}


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






void alpha(int i){
	if (i==0){
	
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
	
	
	
	
	

	
	
	
