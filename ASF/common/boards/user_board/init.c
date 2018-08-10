/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include "user_board.h"
#include <conf_board.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

void uart_string(const char * nazwa)
{
	for(int i=0;i<strlen(nazwa);i++)
	{

		udi_cdc_putc(nazwa[i]);

	}
}

void Send_spi( char byte)
{
	SPIC.DATA = byte;
	while (SPIC.STATUS == 0);
	
}



uint8_t SPItransmit(uint8_t data) {
	SPIC.DATA	=	data;
	while (SPIC.STATUS == 0);
	return SPIC.DATA;
}


void board_init(void)
{
		PORTD.DIR	 = PIN0_bm | PIN1_bm | PIN2_bm  |PIN3_bm; //inicjalizacja diod do kana³ów
		PORTE.DIR	 = PIN0_bm | PIN1_bm | PIN2_bm  |PIN3_bm;
		PORTD.OUTSET = PIN0_bm | PIN1_bm | PIN2_bm	|PIN3_bm;
		PORTE.OUTSET = PIN0_bm | PIN1_bm | PIN2_bm  |PIN3_bm;
}

void SPI_init(void)
{
	PORTA.DIR =		0b11111111; // CS portA output
	PORTA.OUTSET=	0b11111111;// CS highstate
	
	PORTC.DIRSET=	 PIN7_bm | PIN5_bm | PIN4_bm ;	//ouput MOSI,SCK, SS Xmega output !
	PORTC.DIRCLR=	PIN6_bm;			//input MISO
	PORTC.PIN6CTRL   =    PORT_OPC_PULLUP_gc;
	PORTC.OUTCLR=	PIN7_bm| PIN5_bm | PIN4_bm; // initialization on state low
	SPIC.CTRL	= SPI_ENABLE_bm | SPI_MASTER_bm | SPI_MODE_3_gc | SPI_PRESCALER_DIV128_gc ; // config of SPI MODE(enable, master, falling edge, 4x prescaller)
	SPIC.INTCTRL = SPI_INTLVL_LO_gc;
	PMIC_CTRL	=	PMIC_LOLVLEN_bm; // high interrupt priority
	
}

void OscXtal(void) {
	
	// konfiguracja generatora kwarcowego
	OSC.XOSCCTRL	=	OSC_FRQRANGE_12TO16_gc |		// wybór kwarcu od 12 do 16 MHZ
	OSC_XOSCSEL_XTAL_16KCLK_gc;		// czas na uruchomienie generatora
	OSC.CTRL		=	OSC_XOSCEN_bm;					// uruchomienie generatora kwarcowego
	
	// czekanie na ustabilizowanie siê generatora
	for(uint8_t i=0; i<255; i++) {
		if(OSC.STATUS & OSC_XOSCRDY_bm) {
			CPU_CCP			=	CCP_IOREG_gc;			// odblokowanie zmiany Ÿród³a sygna³u zegarowego
			CLK.CTRL		=	CLK_SCLKSEL_XOSC_gc;	// wybór Ÿród³a sygna³u zegarowego na XTAL 16MHz							// komunikat o uruchomieniu generatora
			
			// uk³ad nadzoruj¹cy kwarc
			CPU_CCP			=	CCP_IOREG_gc;			// odblokowanie modyfikacji wa¿nych rejestrów
			OSC.XOSCFAIL	=	OSC_XOSCFDEN_bm;		// w³¹czenie uk³adu detekcji b³êdu sygna³u zegarowego
			return;										// wyjœcie z funkcji jeœli generator siê uruchomi³
		}
		_delay_us(10);
	}
}


void Timer_init(void)
{
	  TCC0.INTCTRLA     =    TC_OVFINTLVL_LO_gc;         // przepe³nienie ma generowaæ przerwanie LO
	  PMIC.CTRL         =    PMIC_LOLVLEN_bm;            // odblokowanie przerwañ o priorytecie LO
    TCC0.CTRLB        =    TC_WGMODE_NORMAL_gc;        // tryb normalny
    TCC0.CTRLA        =    TC_CLKSEL_DIV1024_gc;       // ustawienie preskalera i uruchomienie timera
	TCC0.PER		=	500;
	
}

