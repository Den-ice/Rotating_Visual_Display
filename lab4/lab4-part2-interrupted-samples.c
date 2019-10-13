/*
 * Lab4-Part2-interrupt timer triggered samples
* samples and outputs x,y,z  of acelerometer on pins adc0,adc1,adc2 to usart
* Created: 9/19/2019 3:16:32 PM
 * Author : Sean Gow and Denice Hickethier
 */
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>

#include <AVRXlib/AVRXClocks.h>
#include <AVRXlib/AVRXSerial.h>


/* fun_prototypes.h */
//prototypes for peripheral initialization functions
void sys_clock();
void setup_timer();
void setup_spi();
//void setup_usart();
void setup_peripherals();
void setup_avrx_usart();
void setup_gpio(); 
/*end of fun */

void setup_adcA(); //new for lab4
void setup_adcB(); //new for lab4
void parse_it();
volatile int count=0;
volatile int tx_ready=0;
volatile XUSARTst Ser;
 char *wordcount[]= {"one","two","three","four","five","six","seven","eight","nine","ten","eleven","twelve","thirteen","fourteen","fifteen"};
 char *word;

unsigned long sClk, pClk; //sysclock and peripheral clock
 char rx_buf[1];
volatile int sweep=0;
volatile uint16_t adc_sample[3];
char *sample_word;
volatile int wait_for_it=0; //sephamore /flag
volatile int enable_count=0;
volatile int enable_light_adc=0;
volatile int enable_accel=1;
volatile int adccount=1;


ISR (TCC0_OVF_vect){

	if (enable_count==1){
		word=wordcount[count%15];
		count++;	
		USART_send(&Ser, word);
	}
	if (enable_light_adc==1){

		ADCA_CH0_CTRL =  ADC_CH_INPUTMODE_SINGLEENDED_gc|ADC_CH_GAIN_1X_gc;
		ADCA_CTRLA |= ADC_ENABLE_bm; // enable and wait ~24clks?
		ADCA_CTRLA |= ADC_CH0START_bm ; //
	}
	
	if (enable_accel==1){
		if (adccount==2){
			sprintf(sample_word,"%d,%d,%d\n",adc_sample[0],adc_sample[1],adc_sample[2]);
			USART_send(&Ser,sample_word );
			ADCB_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;//ADC_CH_MUXPOS_PIN4_gc|ADC_CH_MUXPOS_PIN5_gc;	
			ADCB_CTRLA |= ADC_CH0START_bm ; //
		}
	}
			
}
	
	ISR(TCC0_CCA_vect){  //start pin 0 adc  x axis
			ADCB_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;//ADC_CH_MUXPOS_PIN4_gc|ADC_CH_MUXPOS_PIN5_gc;
			ADCB_CTRLA |= ADC_CH0START_bm ; //
			adccount=0;
	}
		
	ISR(TCC0_CCB_vect){ //start pin 1 adc y axis
			ADCB_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;//ADC_CH_MUXPOS_PIN4_gc|ADC_CH_MUXPOS_PIN5_gc;
			ADCB_CTRLA |= ADC_CH0START_bm ; //
			adccount=1;
	}

	ISR(TCC0_CCC_vect){
			ADCB_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN2_gc;//ADC_CH_MUXPOS_PIN4_gc|ADC_CH_MUXPOS_PIN5_gc;
			ADCB_CTRLA |= ADC_CH0START_bm ; //
			adccount=2;
	}

/* Interrupt Service Handlers */
ISR(USARTC0_RXC_vect){
	Rx_Handler(&Ser);
	wait_for_it=1;
}

ISR(USARTC0_TXC_vect){
	Tx_Handler(&Ser);	
}

ISR(USARTC0_DRE_vect){
}

ISR(ADCA_CH0_vect){
	free(sample_word);
	sample_word=malloc(50);
	adc_sample[0]=ADCA.CH0.RES;
	sprintf(sample_word,"The adc value is : %d \n",adc_sample[0]);
	USART_send(&Ser,sample_word );
}

ISR(ADCB_CH0_vect){
	adc_sample[adccount]=ADCB.CH0.RES;
}


// call used peripheral setups
void setup_peripherals(){
	cli();
	sys_clock();
	setup_gpio();
	setup_timer();
	setup_adcA();   //light sensor
	setup_adcB();	//accelerometer on adc0,1,2

	//setup_spi();
	setup_avrx_usart();
	PMIC_CTRL = PMIC_HILVLEN_bm|PMIC_MEDLVLEN_bm|PMIC_LOLVLEX_bm;   // enable all priorities
	sei();
}


int main(void)
{	
	
	sample_word=malloc(50);
	setup_peripherals();
	
while (1)
    {
		if (wait_for_it==1)			
		{
			wait_for_it=0;
			USART_read(&Ser,rx_buf);
			USART_RxFlush(&Ser);
			parse_it(rx_buf);	
		}
    }
}

void parse_it(char *text){	
	switch (text[0]){
		case '1': count=1;break;
		case '2': count=2;break;
		case '3': count=3;break;
		case '4': count=4;break;
		case '5': count=5;break;
		case '6': count=6;break;
		case '7': count=7;break;
		case '8': count=8;break;
		case '9': count=9;break;
		case 'a': count=10;break;
		case 'b': count=11;break;
		case 'c': count=12;break;
		case 'd': count=13;break;
		case 'e': count=14;break;
		case 'f': count=15;break;
		case 's': enable_count^=0x01;
			USART_send(&Ser,"toggle_count");
			break;
		case 'l': enable_light_adc^=0x01;
			USART_send(&Ser,"sample_light");
			ADCA_CH0_CTRL =  ADC_CH_INPUTMODE_SINGLEENDED_gc|ADC_CH_GAIN_1X_gc;
			ADCA_CTRLA |= ADC_ENABLE_bm; // enable and wait ~24clks?
			ADCA_CTRLA |= ADC_CH0START_bm ; //
			break;
		case 'x': //toggle accel sample printout
			enable_accel^=1;
			break;
			
	}
	count =count-1;
	return;	
}

void setup_adcA(){
		ADCA_CTRLB |= ADC_RESOLUTION_12BIT_gc;
		ADCA_REFCTRL |= ADC_REFSEL_INTVCC2_gc; // use 3V3 to chip
		ADCA_PRESCALER |= ADC_PRESCALER_DIV512_gc; //peripheral clock/512
		ADCA_CH0_CTRL =  ADC_CH_INPUTMODE_SINGLEENDED_gc|ADC_CH_GAIN_1X_gc;
		
		ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
		ADCA_CTRLA |= ADC_ENABLE_bm; // enable and wait ~24clks?	
		ADCA_CH0_INTCTRL = ADC_CH_INTMODE_COMPLETE_gc| ADC_CH_INTLVL_HI_gc;	
		ADCA_CTRLA |= ADC_CH0START_bm ; //
		
}


void setup_adcB(){
	ADCB_CTRLB |= ADC_RESOLUTION_12BIT_gc;
	ADCB_REFCTRL |= ADC_REFSEL_INTVCC2_gc; // use 3V3 to chip
	ADCB_PRESCALER |= ADC_PRESCALER_DIV256_gc; //peripheral clock/512

	ADCB_CH0_CTRL =  ADC_CH_INPUTMODE_SINGLEENDED_gc|ADC_CH_GAIN_1X_gc;
	ADCB_CH0_MUXCTRL =ADC_CH_MUXPOS_PIN0_gc|ADC_CH_MUXPOS_PIN1_gc|ADC_CH_MUXPOS_PIN2_gc;
	ADCB_CTRLA |= ADC_ENABLE_bm; // enable and wait ~24clks?
	ADCB_CH0_INTCTRL = ADC_CH_INTMODE_COMPLETE_gc| ADC_CH_INTLVL_HI_gc;
	ADCB_CTRLA |= ADC_CH0START_bm ; //
	
}

void sys_clock(){
	//set mcu clock/frequency
	SetSystemClock(CLK_SCLKSEL_RC32M_gc, CLK_PSADIV_1_gc,CLK_PSBCDIV_1_1_gc);
	GetSystemClocks(&sClk, &pClk);
	}
	
void setup_timer(){
	//	1s= 2Mhz/1024= 0x7a1
	//  1s=32m/1024=0x7a12
	TCC0_PER=0X7a12;					//1s
	TCC0_INTCTRLA = PMIC_MEDLVLEN_bm; // medium level interrupt
	
	//CTRLA /PRESCALER /1	/2	/4	/8	/64	/256	/1024
	//VALUE		      0X1	0X2	0X3	0X4	0X5	0X6		0X7	
	setup_compare_timer();
	TCC0_CTRLA=0x7;
	
}

void setup_compare_timer(){
	TCC0_CTRLB= (((TCC0_CTRLB)>>3)<<3)|TC_WGMODE_NORMAL_gc;
		
TCC0_CCA=TCC0_PER/8;
TCC0_CCB=TCC0_PER/4;
TCC0_CCC=TCC0_PER/2;
TCC0_CTRLB|= TC0_CCAEN_bm|TC1_CCAEN_bm;
TCC0_INTCTRLB|=PMIC_MEDLVLEN_bm<<TC0_CCAINTLVL_gp|PMIC_MEDLVLEN_bm<<TC0_CCBINTLVL_gp|PMIC_MEDLVLEN_bm<<TC0_CCCINTLVL_gp;	
}



//Configure SPI
void setup_spi(){
	PORTC.DIR|=PIN4_bm|PIN5_bm|PIN7_bm;;
	PORTC.PIN5CTRL=PORT_OPC_PULLUP_gc; 
	SPIC.CTRL=  SPI_ENABLE_bm|SPI_MASTER_bm|SPI_PRESCALER_DIV128_gc|SPI_MODE0_bm;  //| SPI_CLK2X_bm
	SPIC.DATA=0b11001010;  //something to transmit
}

//Configure USART

void setup_avrx_usart(){
	
	USART_init(&Ser, 0xc0, pClk, (_USART_TXCIL_LO | _USART_RXCIL_LO), 576, -4,_USART_CHSZ_8BIT, _USART_PM_DISABLED, _USART_SM_1BIT);
	USART_buffer_init(&Ser, 160, 180);
	Ser.fInMode = _INPUT_CR ; //| _INPUT_ECHO | _INPUT_TTY;
	//Ser.fOutMode = _OUTPUT_CRLF;
	USART_enable(&Ser,(USART_TXEN_bm|USART_RXEN_bm));
	
}

void setup_gpio(){
	PORTR_DIR|=0x3; //enable leds
	PORTR_OUT|=0X02;//turn R0 led on

}




/*
void setup_usart(){
	
	int nBScale =-5;
	unsigned long nBSel;
	long fCLK =2000000L ;  //cpu is at 32mhz, 2000000  works for 57.6k sync
	long fbaud= 57611 ;
	
	PORTC.DIRSET=PIN3_bm;		//set pin3 to output
	PORTC.PIN3CTRL=PORT_OPC_PULLUP_gc;	//pullup pin3
	nBSel=fCLK/(16*(pow(2,nBScale)*fbaud))-1; //calculate settings, tweaked to sync at ~57.6k
	
	USARTC0_BAUDCTRLA=(unsigned char)(nBSel &0x00FF);	
	USARTC0_BAUDCTRLB=(char)((nBScale &0x00F)<<4)|((nBSel & 0x0F00>>8));	
	USARTC0.CTRLA= USART_TXCINTLVL_LO_gc;
				//ASYNC MODE                //8 BIT CHARS        //PARITY OFF               //NO STOP BIT     //low interrupt
	USARTC0.CTRLC=USART_CMODE_ASYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc	|0x00	;
	USARTC0.CTRLB=USART_RXEN_bm|USART_TXEN_bm;  
	
}*/
