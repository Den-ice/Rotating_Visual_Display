/*
Physics 402 Final Project 
*/

#define F_CPU 32000000L  //for wait util
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t *data[88];
uint8_t startframe=0x00;
uint8_t ledframe=0XE0;
uint8_t brightness=30;

uint8_t blue=0;
uint8_t red=127;
uint8_t green=255;
uint8_t stopframe=0xff;
int rampup=1;
float speed=5;

uint8_t count=0;
#include "setup.h"
void led(int num);
void frame();
volatile on=0;

ISR( PORTA_INT0_vect ){
	on^=1;
	brightness--%31;
	red++%255;
	blue++%255;
	green++%255;
	
	frame();
	
	PORTR_OUT^=0x01;
  }
ISR(RTC32_OVF_vect){
	
	
	
}

ISR (TCD0_OVF_vect){	
	
    }
	
ISR(TCD0_CCA_vect){  

}
		
ISR(TCD0_CCB_vect){ 

}
ISR(TCD0_CCC_vect){

	}
ISR( PORTF_INT0_vect ){

	
		}

ISR (TCC0_OVF_vect){	
	

    }
	
ISR(TCC0_CCA_vect){  
	}
		
ISR(TCC0_CCB_vect){ 
	

}
ISR(TCC0_CCC_vect){
	
	}



/* //Interrupt Service Handlers 
ISR(USARTC0_RXC_vect){
	Rx_Handler(&Ser);
}
*/
/*
ISR(USARTC0_TXC_vect){
	Tx_Handler(&Ser);
}
ISR(USARTC0_DRE_vect){
}
ISR(ADCA_CH0_vect){
}
ISR(ADCB_CH0_vect){
}
*/

ISR( PORTC_INT0_vect ){
}


int main(){
	cli();
	//event_system_init();
	system_clocks();
	rtc32_clock();


	gpio_setup();
	sei();
	data[0]=&startframe;
	data[1]=&startframe;
	data[2]=&startframe;
	data[3]=&startframe;
	data[84]=&stopframe;
	data[85]=&stopframe;
	data[86]=&stopframe;
	data[87]=&stopframe;
	
	for (int i=4;i<=8;i++){
	data[i+0]=&ledframe;
	data[i+1]=&blue;
	data[i+2]=&green;
	data[i+3]=&red;		
	}
	

	setup_spi();
	_delay_ms(500);
	
	
	
	
	//while(!(USARTC0_STATUS& USART_DREIF_bm));
	//USARTC0_DATA=stopframe;
	
	//setup_dmac((uint32_t) &data,(uint32_t) &SPIC_DATA);
	while(1){
		
		//test();
		//DMA.CH0.CTRLA=DMA_CH_TRFREQ_bm; //MANUAL TRIGGER
		//while(!(DMA.CH0.CTRLB&DMA_CH_TRNIF_bm));
		//_delay_us(1);
		

	
	

	}

}

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
	int color=random(8);
	for (int i=0;i<num;i++){
//START LED FRAME

while(!(USARTC0_STATUS& USART_DREIF_bm));
USARTC0_DATA=ledframe|brightness;
//_delay_us(1);
//BLUE

while(!(USARTC0_STATUS& USART_DREIF_bm));
//_delay_us(100);
USARTC0_DATA=blue;
//GREEN	

while(!(USARTC0_STATUS& USART_DREIF_bm));//green^=blue;
USARTC0_DATA=green;
//_delay_us(100);

while(!(USARTC0_STATUS& USART_DREIF_bm));
//_delay_us(1);
//red^=green;
USARTC0_DATA=red;

//_delay_us(1);
//end LED

	}
}

void test(){
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=startframe;
	
	
	
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
	USARTC0_DATA=blue;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=green;
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=red;


	
	
	
	//_delay_us(50);
	
	while(!(USARTC0_STATUS& USART_DREIF_bm));
	USARTC0_DATA=stopframe;
	//while(!(USARTC0_STATUS& USART_DREIF_bm));
	//USARTC0_DATA=stopframe;
	//while(!(USARTC0_STATUS& USART_DREIF_bm));
//	USARTC0_DATA=stopframe;;
	//while(!(USARTC0_STATUS& USART_DREIF_bm));
	//USARTC0_DATA=stopframe;;
}
