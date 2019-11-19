/*
 * lab9-part1.c
 *
 * Created: 11/14/2019 10:09:33 AM
 * Author : Sean
 */ 

//#include <avr/io.h>

/*
int main(void)
{
    // Replace with your application code */
  //  while (1) 
    //{
    //}
//}
//
/*
 * Lab4-Part2-interrupt timer triggered samples
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
void setup_peripherals();
void setup_avrx_usart();
void setup_gpio(); 
/*end of fun */

int dark[2];

int dim[2];


int bright[2];



void setup_adcA(); //new for lab4
void setup_adcB(); //new for lab4
void parse_it();
volatile int count=0;
volatile int tx_ready=0;
volatile XUSARTst Ser;
 char *wordcount[]= {"one","two","three","four","five","six","seven","eight","nine","ten","eleven","twelve","thirteen","fourteen","fifteen"};
 char *word;
void automata();

unsigned long sClk, pClk; //sysclock and peripheral clock
 char rx_buf[1];
volatile int sweep=0;
volatile uint16_t adc_sample[3];
char *sample_word;
volatile int wait_for_it=0; //sephamore /flag
volatile int enable_count=0;
volatile int enable_light_adc=1;
volatile int enable_accel=1;
volatile int adccount=1;
volatile uint16_t light=0;

volatile int D,I,B=0;


ISR (TCC0_OVF_vect){

	if (enable_count==1){
		word=wordcount[count%15];
		count++;	
		USART_send(&Ser, word);
	}
	//if (enable_light_adc==1){

/*
dark[0]=0;
dark[1]=900;
dim[0]=500;
dim[1]=1600;
bright[0]=1200;
bright[1]=4095;*/

//calculate fuzziness truth value
if(light<=(dim[0]-300)){
	D= 100;
	I=0;
	B=0;
}
else if (light<=dark[1]){
	I=(light-200)/7;
	D=100-I;
	B=0;
}
else if (light<=bright[0]){
	I=100;
	D=0;
	B=0;
}
else if (light<=dim[1]){
	B=(light-bright[0])/4;
	I=100-B;
	D=0;
}
else{
	B=100;
	D=0;
	I=0;
}

//if (adc_sample[0] )
		//ADCA_CH0_CTRL =  ADC_CH_INPUTMODE_SINGLEENDED_gc|ADC_CH_GAIN_1X_gc;
	//	ADCA_CTRLA |= ADC_ENABLE_bm; // enable and wait ~24clks?
		ADCA_CTRLA |= ADC_CH0START_bm ; //
		ADCB_CTRLA |= ADC_CH0START_bm ; //
	//}
	
	/*
	if (light>=900 && light<=1600){
		I=(light-900)/7;
		
	}
	else
	{ 
		I=0;
	}
	if (light>=1600 &&light<=4095){
		B=(light-1600)/25;
	}
	else {
		B=0;
	}
	*/
	
		free(sample_word);
		sample_word=malloc(50);
		sprintf(sample_word,"Dark: %d , Dim: %d , Bright: %d  Hall effect:%d \r\n",D,I,B,adc_sample[0]);
		USART_send(&Ser,sample_word );
	
	
	
	//if (enable_accel==1){
//		if (adccount==2){
	//		sprintf(sample_word,"%d,%d,%d\n",adc_sample[0],adc_sample[1],adc_sample[2]);
	//		USART_send(&Ser,sample_word );
//			ADCB_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;//ADC_CH_MUXPOS_PIN4_gc|ADC_CH_MUXPOS_PIN5_gc;	
	//		ADCB_CTRLA |= ADC_CH0START_bm ; //
	//	}
	//}
			
}
	/*
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
*/
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
	//free(sample_word);
	//sample_word=malloc(50);
	light=ADCA.CH0.RES;
	//sprintf(sample_word,"The light value is : %d \n",light);
	//USART_send(&Ser,sample_word );
}

ISR(ADCB_CH0_vect){
	//adc_sample[adccount]=ADCB.CH0.RES;
//	PORTR_OUT^=0x02;

	adc_sample[0]=ADCB.CH0.RES;
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
	dark[0]=0;
	dark[1]=900;
	dim[0]=500;
	dim[1]=1600;
	bright[0]=1200;
	bright[1]=4095;

	sample_word=malloc(50);
	setup_peripherals();
	
while (1)
    {
		automata();

    }
}



void setup_adcA(){
		ADCA_CTRLB |= ADC_RESOLUTION_12BIT_gc;
		ADCA_REFCTRL |= ADC_REFSEL_INTVCC2_gc; // use 3V3 to chip
		ADCA_PRESCALER |= ADC_PRESCALER_DIV512_gc; //peripheral clock/512
		ADCA_CH0_CTRL =  ADC_CH_INPUTMODE_SINGLEENDED_gc|ADC_CH_GAIN_1X_gc;
		
		ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
		ADCA_CTRLA |= ADC_ENABLE_bm; 	
		ADCA_CH0_INTCTRL = ADC_CH_INTMODE_COMPLETE_gc| ADC_CH_INTLVL_HI_gc;	
		ADCA_CTRLA |= ADC_CH0START_bm ; //
		
}


void setup_adcB(){
	ADCB_CTRLB |= ADC_RESOLUTION_12BIT_gc;
	ADCB_REFCTRL |= ADC_REFSEL_INTVCC2_gc; // use 3V3 to chip
	ADCB_PRESCALER |= ADC_PRESCALER_DIV256_gc; //peripheral clock/512

	ADCB_CH0_CTRL =  ADC_CH_INPUTMODE_SINGLEENDED_gc|ADC_CH_GAIN_1X_gc;
	ADCB_CH0_MUXCTRL =ADC_CH_MUXPOS_PIN0_gc|ADC_CH_MUXNEG_PIN0_gc;//|ADC_CH_MUXPOS_PIN1_gc|ADC_CH_MUXPOS_PIN2_gc;
	ADCB_CTRLA |= ADC_ENABLE_bm; 
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
//set compare counters			
//	TCC0_CCA=TCC0_PER/8;
	//TCC0_CCB=TCC0_PER/4;
//	TCC0_CCC=TCC0_PER/2;

//	TCC0_CTRLB|= TC0_CCAEN_bm|TC0_CCBEN_bm|TC0_CCCEN_bm;
//	TCC0_INTCTRLB|=PMIC_MEDLVLEN_bm<<TC0_CCAINTLVL_gp|PMIC_MEDLVLEN_bm<<TC0_CCBINTLVL_gp|PMIC_MEDLVLEN_bm<<TC0_CCCINTLVL_gp;	
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
	Ser.fOutMode = _OUTPUT_CRLF;
	USART_enable(&Ser,(USART_TXEN_bm|USART_RXEN_bm));
	
}

void setup_gpio(){
	PORTR_DIR|=0x3; //enable leds
	PORTR_OUT|=0X02;//turn R0 led on

}


automata(){
	
	
	
	
}
