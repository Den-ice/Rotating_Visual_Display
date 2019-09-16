//INITIALIZE TIMER
cei();
  TCC0_PER=0xFFFF;					// rough isr saturation point  
	   TCC0_INTCTRLA= PMIC_MEDLVLEN_bm; // medium interrupt
	   TCC0_CTRLA=0x02		;			//use 1X prescaler
	   PMIC_CTRL = PMIC_MEDLVLEN_bm;   // turn on medium priority interrupts
       SPIC_CTRL= SPI_ENABLE_bm| SPI_MASTER_bm|SPI_MODE0_bm|SPI_PRESCALER_DIV4_gc;//ENABLE SPI | MASTER
sei();
...

ISR(TCC0_OVF_vect){

}
