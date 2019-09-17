 
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <AVRXlib/AVRXSerial.h>

ISR( SPIC_INT_vect     ){
			SPIC_DATA=0b11001010; //transmit data?
}

ISR (TCC0_OVF_vect){
	PORTR_OUT^=0X05;  //toggle led, visual for when spi should fire
	SPIC_DATA+=1; //trigger spic interrupt?
}

void spi_setup(){
	//portc
	//PORTC_DIR=PIN4_bm|PIN5_bm|PIN7_bm;
	//PORTC_PIN5CTRL=PORT_OPC_PULLUP_gc;
	
	//slave select pins
	PORTC.DIRSET = PIN4_bm;
	PORTC.PIN4CTRL = PORT_OPC_WIREDANDPULL_gc;
	PORTC.OUTSET = PIN4_bm; 
	
	//PORTC.PIN7CTRL= PORT_OPC_WIREDANDPULL_gc; // do we need this?
	
		           //PIN5/0X20 PIN7/0X80 =1
	PORTC.DIRSET|= PIN5_bm|PIN7_bm ;// 0X20|0X80;
	 
	            //ENABLE SPI   MASTER MODE   PRESCALER 4,16,64,128          SPI MODE0-4  2X CLOCK
	SPIC.CTRL=  SPI_ENABLE_bm|SPI_MASTER_bm|SPI_PRESCALER_DIV128_gc|SPI_MODE0_bm;  //| SPI_CLK2X_bm 
  SPIC.INTCTRL = PMIC_MEDLVLEN_bm ;	//SET SPI PRIORITY
	return;
	}

void tcc_setup(){
/*1s timer
period =	2M/1024=1953 =0x7a1
.5s	=	3d0
.25s=   0x1e8
.125s=   0xf4
*/
	   TCC0_PER=0Xf4;					//period
	   TCC0_INTCTRLA= PMIC_MEDLVLEN_bm; // medium LEVEL interrupt
	   //PRESCALER /1	/2	/4	/8	/64	/256	/1024
	  //VALUE		1	2	3	4	5	6		7
	   TCC0_CTRLA=0x02;			//?
return;
}

void uart_setup(){
	/*
  uart registers /settings
	CTRLA;
	CTRLB;
	CTRLC; 
	BAUDCTRLA;
	BAUDCTRLB;
	more/less?
  */
  
	return;
}
	

void setup_interrupts(){
	cli();//clear interrupts
	tcc_setup();//setup a timer counter
	spi_setup();//setup spi
  uart_setup();//setup uart
   PMIC_CTRL = PMIC_MEDLVLEN_bm;   // turn on medium priority interrupts
   PMIC_CTRL = SPI_INTLVL_MED_gc|PMIC_MEDLVLEN_bm|PMIC_LOLVLEX_bm;   // turn on medium priority interrupts & LOW PRIORITY INTERRUPT

	sei();// turn on interrupts
	return;
}

void gpio_setup(){
	PORTR_DIR=0xFF;
	PORTR_OUT=0X00;
}

int main(void)
{
  gpio_setup(); //setup general io ports
  
setup_interrupts();
	
	
    while (1) 
    {
		
    }
}

