*
 * lab3-pt1.c
 *
 * Created: 9/12/2019 9:18:32 AM
 * Author : Sean
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <AVRXlib/AVRXSerial.h>


ISR( SPIC_INT_vect     ){
//	while(SPIC_STATUS &SPI_WRCOL_bm)
		SPIC_DATA=0b11001010;
	
}
/*
ISR (RTC32_OVF_vect)
{
PORTR_OUT=0X48;
}
*/
ISR (TCC0_OVF_vect){
	PORTR_OUT^=0X05;
	while(SPIC_STATUS & SPI_WRCOL_bm)
		SPIC_DATA=+1;
	
	
}

void spi_setup(){
	
	//slave select
	PORTC.DIRSET = PIN4_bm;
	PORTC.PIN4CTRL = PORT_OPC_WIREDANDPULL_gc;
	PORTC.OUTSET = PIN4_bm; 
	PORTC.PIN7CTRL= PORT_OPC_WIREDANDPULL_gc;
	
	//SPIC.STATUS  = !SPI_IF_bm;
	SPIC_CTRL=  SPI_ENABLE_bm|SPI_MASTER_bm|SPI_PRESCALER_DIV16_gc|SPI_MODE0_bm;  //| SPI_CLK2X_bm 
	PORTC.DIRSET|= PIN5_bm|PIN7_bm ;// 0X20|0X80;	 // = SPI_MOSI_bm | SPI_SCK_bm;


   SPIC.INTCTRL = PMIC_MEDLVLEN_bm ;
SPIC_DATA= 0b11010011 ;

	}

	

int main(void)
{
	
	
	//leds somewhere on port r
	PORTR_DIR=0xFF;
	PORTR_OUT=0X00;
	
	//portc 
	PORTC_DIR=PIN4_bm|PIN5_bm|PIN7_bm;//PORTC_DIR& ~(1<<5) ); 
	PORTC_PIN5CTRL=PORT_OPC_PULLUP_gc;
	//PORTC_DIR
	
	cli();                           //clear interrupt counter & register
    
/*
//clock interrupt setup
	CLK_RTCCTRL = CLK_RTCSRC_RCOSC32_gc | CLK_RTCEN_bm;//rtc clock setup
	RTC32_PER0=1; //set 500 ticks
	RTC32_INTCTRL=PMIC_LOLVLEX_bm;//set low priority
	RTC32_CTRL = RTC32_ENABLE_bm|RTC32_OVFINTLVL_gm; //set granularity to 1ms
*/
/*CLKSEL[3:0] Group configuration Description
0000 OFF None (i.e, timer/counter in OFF state)
0001 DIV1 Prescaler: Clk
0010 DIV2 Prescaler: Clk/2
0011 DIV4 Prescaler: Clk/4
0100 DIV8 Prescaler: Clk/8
0101 DIV64 Prescaler: Clk/64
0110 DIV256 Prescaler: Clk/256
0111 DIV1024 Prescaler: Clk/1024
1nnn EVCHn Event channel n, n= [0,...,7]
*/
	   cli();                           //clear interrupt counter & register
	   TCC0_PER=0Xff;					// rough isr saturation point  
	   TCC0_INTCTRLA= PMIC_MEDLVLEN_bm; // medium interrupt
	   TCC0_CTRLA=0x02;			//use 1024 prescaler
	   PMIC_CTRL = PMIC_MEDLVLEN_bm;   // turn on medium priority interrupts
       SPIC_CTRL= SPI_ENABLE_bm| SPI_MASTER_bm|SPI_MODE0_bm|SPI_PRESCALER_DIV128_gc;//ENABLE SPI | MASTER

	   SPIC_INTCTRL= SPI_INTLVL_MED_gc;
	   PMIC_CTRL = SPI_INTLVL_MED_gc|PMIC_MEDLVLEN_bm|PMIC_LOLVLEX_bm;   // turn on medium priority interrupts & LOW PRIORITY INTERRUPT

	//   sei();


//spi data
//    SPIC_DATA=0XAA;	
	//SETUP SPIC(PORT C)


	SPIC_CTRL= SPI_ENABLE_bm| SPI_MASTER_bm|SPI_MODE0_bm|SPI_PRESCALER_DIV4_gc;//ENABLE SPI | MASTER

	SPIC_INTCTRL= SPI_INTLVL_MED_gc;
	PMIC_CTRL = SPI_INTLVL_MED_gc|PMIC_MEDLVLEN_bm|PMIC_LOLVLEX_bm;   // turn on medium priority interrupts & LOW PRIORITY INTERRUPT
	
	sei();
	
	/*
	CTRLA;
	CTRLB;
	CTRLC; 
	BAUDCTRLA;
	BAUDCTRLB;
	
	*/
    /* Replace with your application code */
	spi_setup();
	
	asm("nop");
/*		SPI_MasterInit(&spiMasterC,
	               &SPIC,
	               &PORTC,
				   false,
	               SPI_MODE_0_gc,
	               SPI_INTLVL_LO_gc,
	               false,
	               SPI_PRESCALER_DIV4_gc);
*/
	
    while (1) 
    {
		
		
    }
}

