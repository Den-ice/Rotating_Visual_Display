#include "setup.h"

#define F_CPU 32000000L

/* clock initialization with digital frequency locked loop */
void system_clocks(){
	
	//change osc
	//enable rtc with 32k
	CLK.RTCCTRL=CLK_RTCSRC_RCOSC_gc|1;
	
	//set external oscillator to 32k crystal
	OSC.XOSCCTRL=OSC_XOSCSEL_32KHz_gc|OSC_X32KLPM_bm|OSC_XOSCPWR_bm;
	//32k cal register to production calibration do not use with dfll
	//OSC.RC32KCAL=PRODSIGNATURES_RCOSC32K;
	
	//set protected register flag to modify
	CCP=CCP_IOREG_gc; 
	OSC.PLLCTRL=OSC_PLLSRC_RC32M_gc|OSC_PLLFAC3_bm; //0x04 32mhz*4=128mhz?
	//calibrate 32mhz to 32k crystal
	OSC.DFLLCTRL=OSC_RC32MCREF_RC32K_gc|OSC_RC2MCREF_RC32K_gc;
	
	OSC.CTRL=OSC_RC32MEN_bm|OSC_XOSCEN_bm|OSC_PLLEN_bm|OSC_RC32KEN_bm|OSC_FRQRANGE_04TO2_gc;
	
	while(!(OSC_STATUS & (OSC_RC32MRDY_bm|OSC_PLLRDY_bm|OSC_RC2MRDY_bm )));   
	CCP=CCP_IOREG_gc;	//signal protected ioreg change
	CLK.RTCCTRL|=CLK_RTCEN_bm;
	
	while(!(OSC_STATUS & (OSC_RC32MRDY_bm|OSC_PLLRDY_bm|OSC_RC2MRDY_bm )));   		
	CCP=CCP_IOREG_gc; 	//signal protected ioreg change
	//SET DEFAULT SYSCLOCK
	CLK_CTRL=CLK_SCLKSEL_RC32M_gc;
	while(!(OSC_STATUS & OSC_RC32KRDY_bm));  
	DFLLRC2M.CTRL=DFLL_ENABLE_bm;
	DFLLRC32M.CTRL=DFLL_ENABLE_bm;
		
	CCP=CCP_IOREG_gc; 
	//CHANGE CLOCK PRESCALERS
	CLK.PSCTRL=CLK_PSADIV_1_gc| CLK_PSBCDIV_4_1_gc;
   //systemclock output on pin 7 | 
	//PORTCFG.CLKEVOUT= PORTCFG_CLKOUT_PC7_gc|PORTCFG_CLKOUTSEL_CLK1X_gc;
	
	//PORTCFG.CLKEVOUT= PORTCFG_CLKEVPIN_PIN7_gc|PORTCFG_CLKOUT_PC7_gc|PORTCFG_CLKOUTSEL_CLK1X_gc|PORTCFG_EVOUT_PC7_gc;
	
}
/*rtc32 enabled*/
void rtc32_clock(){
	
	//rtc clock
	RTC32.CTRL;
	RTC32.SYNCCTRL;
	RTC32.INTCTRL;
	RTC32.INTFLAGS;
	RTC32.CNT;
	RTC32.PER;
	RTC32.COMP;


	//rtc battery  //must be turned on before rtc enabled
	VBAT.CTRL=VBAT_ACCEN_bm|VBAT_XOSCEN_bm|VBAT_XOSCSEL_bm;
	//VBAT.STATUS;
	//VBAT.BACKUP0;
	//VBAT.BACKUP1;
	
	RTC32.CTRL=RTC32_ENABLE_bm;	
}
//lab4 pt2
void  tcd_timer(){
		//	1s= 2Mhz/1024= 0x7a1
	//  1s=32m/1024=0x7a12
	TCD0_PER=0X7a12;					//1s
	TCD0_INTCTRLA = PMIC_MEDLVLEN_bm; // medium level interrupt
	//CTRLA /PRESCALER /1	/2	/4	/8	/64	/256	/1024
	//VALUE		      0X1	0X2	0X3	0X4	0X5	0X6		0X7	
	TCD0_CTRLA=0x7;
	
}
void tcd0_capture_comparator(){
		
	//set compare counters			
	//TCD0_CCA=TCC0_PER/8;
	//TCD0_CCB=TCC0_PER/4;
	//TCD0_CCC=TCC0_PER/2;

	TCD0_CTRLB|= TC0_CCAEN_bm|TC0_CCBEN_bm|TC0_CCCEN_bm;
	TCD0_INTCTRLB|=PMIC_MEDLVLEN_bm<<TC0_CCAINTLVL_gp|PMIC_MEDLVLEN_bm<<TC0_CCBINTLVL_gp|PMIC_MEDLVLEN_bm<<TC0_CCCINTLVL_gp;	
}
	




void gpio_setup(){
	
	PORTC_DIR|=0xff;
	PORTB_DIR|=0x10;
	PORTR_DIR|=0X01;
	PORTD_DIR|=0X0;
	
	/*  SETUP^^^*/
	

	
	
	//trigger edge
	//PORTCFG_MPCMASK=0x01|0x02|0x04|0x08|0x10|0x20;  //0:5
	//PORTC_PIN0CTRL= PORT_OPC_PULLUP_gc |    PORT_ISC_FALLING_gc; //SET MPCMASKED PINS TO 0X02 sense low (capacitive touch buttons are active low)
	
	
	//interrupts
	PORTD_INT0MASK=0x01|0x02|0x04|0x08|0x10|0x20;  //turn on interrupts for ports 0:5
	PORTD_INTCTRL= PMIC_MEDLVLEN_bm;
	
	/*
	//simple hall effect sensor	trigger, needs filtering/ converted to analog comparator
	PORTA_OUT=0x10;
	
	PORTA.PIN4CTRL = PORT_ISC_RISING_gc|PORT_OPC_TOTEM_gc;
	PORTA.INT0MASK = PIN4_bm;
	PORTA.INTCTRL = PORT_INT0LVL_LO_gc;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	
	//end hall effect
	*/
	//simple hall effect sensor	trigger, needs filtering/ converted to analog comparator
	PORTA_OUT=0X00;
	
	PORTA.PIN2CTRL = PORT_ISC_FALLING_gc|PORT_OPC_PULLDOWN_gc;
	PORTA.INT0MASK = PIN2_bm;
	PORTA.INTCTRL = PORT_INT0LVL_HI_gc;
	PMIC.CTRL |= PMIC_HILVLEN_bm;
	
	//end hall effect
}


void setup_spi(){
	PORTC.DIR|=PIN4_bm|PIN5_bm|PIN7_bm;;
	PORTC.PIN5CTRL=PORT_OPC_PULLUP_gc;
	SPIC.CTRL=  SPI_ENABLE_bm|SPI_MASTER_bm |SPI_DORD_bm|SPI_MODE_2_gc;
	USARTC0.CTRLB=USART_TXEN_bm;
	USARTC0.CTRLC= USART_CMODE_MSPI_gc;	//MASTER SPI

}


void setup_dmac(uint32_t src, uint32_t dst){
	DMA.CTRL|=DMA_ENABLE_bm;
	DMA.CH0.CTRLA|= DMA_CH_ENABLE_bm;
	DMA.CH0.CTRLA |=DMA_CH_BURSTLEN_1BYTE_gc;
	DMA.CH0.CTRLA |=DMA_CH_DESTDIR_FIXED_gc;
	//DMA.CH0.ADDRCTRL= DMA_CH_SRCRELOAD_BLOCK_gc;
	DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_NONE_gc |  DMA_CH_SRCDIR_INC_gc;
	DMA.CH0.ADDRCTRL |= DMA_CH_DESTRELOAD_NONE_gc | DMA_CH_DESTDIR_FIXED_gc;
	DMA.CH0.TRIGSRC =DMA_CH_TRIGSRC_USARTC0_DRE_gc;  //SOFTWARE TRIGGER CHANGE TO TIMER TRIGGER
	DMA.CH0.TRFCNT =88;
	
	//FIX ADDRESSES
	//uint32_t src=&data;
	DMA.CH0.SRCADDR0=  (uint8_t)(( (uint32_t) src)) & 0xFF;;
	DMA.CH0.SRCADDR1= (uint8_t)(( (uint32_t) src) >> 1*8 ) & 0xFF;
	DMA.CH0.SRCADDR2= (uint8_t)(( (uint32_t) src) >> 1*8 ) & 0xFF;
	
	//uint32_t dst=&SPIC_DATA;
	DMA.CH0.DESTADDR0=(uint8_t)(( (uint32_t) dst)) & 0xFF;;
	DMA.CH0.DESTADDR1=(uint8_t)(( (uint32_t) dst) >> 1*8 ) & 0xFF;;
	DMA.CH0.DESTADDR2=(uint8_t)(( (uint32_t) dst) >> 1*8 ) & 0xFF;;
	
	//DMA.CH0.CTRLA|=DMA_CH_TRFREQ_bm; //MANUAL TRIGGER	
//	while(!(DMA.CH0.CTRLB&DMA_CH_TRNIF_bm));
	//while(1);
}


void analog_comparator(){
	//ACA_AC0MUXCTRL= ACA_MUXPOS_PIN
	

	
	
	
}
