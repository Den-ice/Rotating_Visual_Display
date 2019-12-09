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



volatile uint8_t data[88];




uint8_t startframe=0x00;
uint8_t ledframe=0XE0;
uint8_t brightness=4;

uint8_t blue=255;
uint8_t red=255;
uint8_t green=255;
uint8_t stopframe=0xff;


//max is 256k
//(1 stored display pattern- 32*360=11520bits)

//fixed patterns
uint32_t ledpattern[360];

//all on








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
	//TRIGGER START OF ENTIRE FRAME
	TCE0_PER=(uint32_t)(TCF0.CCABUF+TCE0_PER)>>1;
	
	TCE0_CCA=TCF0.CCABUF/360;
	TCE0.INTFLAGS&=~TC0_CCAIF_bm;
	

	frame();
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
		
ISR(TCC0_CCB_vect){ 
	

}
ISR(TCC0_CCC_vect){
	
	}


ISR (TCF0_OVF_vect){	
	
    }
	
ISR(TCF0_CCA_vect){
	frame();
	}
		
ISR(TCF0_CCB_vect){ 
	

}
ISR(TCF0_CCC_vect){
	
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


ISR( PORTC_INT0_vect ){
}


int main(){
	
	ledpattern[0]=0b11111111111111111111;
	ledpattern[1]=0b10000000000000000000;
	ledpattern[2]=0b00000000011111111000;
	ledpattern[3]=0b10000000011111111000;
	ledpattern[4]=0b00000000000011000000;
	ledpattern[5]=0b100000000011111111000;
	ledpattern[6]=0b00000000001111111000;
	ledpattern[7]=0b10000000000000000000;
	ledpattern[8]=0b00000000000000000000;
	ledpattern[9]=0b10000000000000000000;
	ledpattern[10]=0b0000000000000000000;
	ledpattern[11]=0b10000000000000000000;
	ledpattern[12]=0b00000000000000000000;
	ledpattern[13]=0b10000000000000000000;
	ledpattern[14]=0b00000000000000000000;
	ledpattern[15]=0b10000000000000000000;
	ledpattern[16]=0b00000000000000000000;
	ledpattern[17]=0b10000000000000000000;
	ledpattern[18]=0b00000000000000000000;
	ledpattern[19]=0b10000000000000000000;
	ledpattern[20]=0b00000000000000000000;
	ledpattern[21]=0b10000000000000000000;
	ledpattern[22]=0b00000000000000000000;
	ledpattern[23]=0b10000000000000000000;
	ledpattern[24]=0b00000000000000000000;
	ledpattern[25]=0b10000000000000000000;
	ledpattern[26]=0b00000000000000000000;
	ledpattern[27]=0b10000000000000000000;
	ledpattern[28]=0b00000000000000000000;
	ledpattern[20]=0b10000000000000000000;

	cli();
	eventtrigger();
	system_clocks();
	tcd_pwm();
	tce_setup();
	gpio_setup();
	setup_spi();
	
	//setup_dmac((uint32_t) &data,(uint32_t)  &USARTC0_DATA);
	
	sei();
	data[0]=&startframe;
	data[1]=&startframe;
	data[2]=&startframe;
	data[3]=&startframe;
	data[4]=&ledframe;
	data[5]=&blue;
	data[6]=&green;
	data[7]=&red;		
	data[8]=&ledframe;
	data[91]=&blue;
	data[10]=&green;
	data[11]=&red;		
	data[12]=&ledframe;
	data[13]=&blue;
	data[14]=&green;
	data[15]=&red;		
	
	data[16]=&stopframe;
	data[17]=&stopframe;
	//data[86]=&stopframe;
	//data[87]=&stopframe;
	
	for (int i=4;i<=80;i=i+4){
	data[i+0]=ledframe;
	data[i+1]=&blue;
	data[i+2]=&green;
	data[i+3]=&red;		
	}
	
	_delay_ms(100);
	
	//while(!(USARTC0_STATUS& USART_DREIF_bm));
	//USARTC0_DATA=stopframe;
	//DMA.CH0.CTRLA=DMA_CH_TRFREQ_bm;
	
	init_motor();
	while(1){
		
		//test();
		 //MANUAL TRIGGER
	//	while(!(DMA.STATUS&0X01));
	//	DMA.INTFLAGS=0;
		//DMA.CH0.CTRLA=DMA_CH_ENABLE_bm| DMA_CH_TRFREQ_bm|DMA_CH_SINGLE_bm;
		//_delay_us(1);
while(!(USARTC0_STATUS& USART_DREIF_bm));
blinkoff();
	//blinkoff();
	}

}



init_motor(){
	
	//initialize motor
	tcd0_pwm_control(20,1000,5);
	_delay_ms(2000);
	tcd0_pwm_control(20,1000,5.2);
	_delay_ms(200);
	tcd0_pwm_control(20,1000,5.3);
	_delay_ms(200);
/*	tcd0_pwm_control(20,1000,5.4);
	_delay_ms(200);
	tcd0_pwm_control(20,1000,5.5);
	_delay_ms(200);
	tcd0_pwm_control(20,1000,5.6);
	_delay_ms(200);
	tcd0_pwm_control(20,1000,5.7);
	_delay_ms(200);
*/}




void frame(){
	
	//for (int i=0;i<=20;i++){
	//32BITS START FRAME
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;

	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;

	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	

	//END OF START FRAME

	led(20);


	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=stopframe;
		while(!(USARTC0_STATUS& USART_DREIF_bm));
USARTC0_DATA=stopframe;
	



	//	}
	
}

void led(int num){
	//int color=random(8);
	
	for (int i=0;i<20;i++){
		//START LED FRAME

		while(!(USARTC0_STATUS& USART_DREIF_bm));
		USARTC0_DATA=ledframe|30;
		//_delay_us(1);
		//BLUE
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		//_delay_us(100);
		USARTC0_DATA=blue&&((ledpattern[bitstream]>>i)&&1);
		//GREEN
		while(!(USARTC0_STATUS& USART_DREIF_bm));//green^=blue;
		USARTC0_DATA=green&&((ledpattern[bitstream]>>i)&&1);
		//red
		while(!(USARTC0_STATUS& USART_DREIF_bm));
		//_delay_us(1);
		USARTC0_DATA=red&&((ledpattern[bitstream]>>i)&&1);
		//end OF 1 LED


	}
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
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	//USARTC0_DATA=stopframe;
	//while(!(USARTC0_STATUS& USART_DREIF_bm));
	//	USARTC0_DATA=stopframe;;
	//while(!(USARTC0_STATUS& USART_DREIF_bm));
	//USARTC0_DATA=stopframe;;
}



void eventtrigger(){
	//PORTA;
	
	PORTA.PIN2CTRL|=PORT_ISC_FALLING_gc;
	PORTD.DIRCLR=0X04;
	
	TCF0.CTRLD=(uint8_t)TC_EVSEL_CH0_gc|TC_EVACT_CAPT_gc;
	TCF0.CTRLB|=TC0_CCAEN_bm;
	TCF0.PER=0XFFFFFF;
	TCF0.INTCTRLA=PMIC_MEDLVLEN_bm;
	TCF0.CTRLA=TC_CLKSEL_DIV256_gc;
	EVSYS.CH0MUX=EVSYS_CHMUX_PORTA_PIN2_gc;
	 
	
	
}

