/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include    <avr/io.h> 
#include    <util/delay.h>
#include "board.h"
#include "CRC.h"
#include "user_board.h"
#include <avr/pgmspace.h>

# define F_CPU 32000000UL
volatile bool SPI_flag=0;
uint8_t volatile receive =1;
uint8_t volatile SPI_timer=0;
uint8_t volatile byte_timer=0;
uint8_t volatile slave_nr=0; 
uint8_t volatile RFID_ID[5];
bool volatile next_slave=0;
int main(void)
{
	
		uint8_t tx_byte;
		uint8_t rx_byte;
			tx_byte = 10;
	
init_build_usb_serial_number();
  sysclk_init();                          //konfiguracja zegara-internal 32MHz
  sysclk_enable_module(SYSCLK_PORT_C,SYSCLK_SPI);
  sysclk_enable_module(SYSCLK_PORT_C,SYSCLK_TC0 );
  irq_initialize_vectors();       //inicjacja wektorów przerwañ (jeœli u¿ywamy)
  cpu_irq_enable();                       //w³¹czenie przerwañ- tu nie dziala
  board_init();                           //inicjacja p³ytki - tu powinno siê inicjowaæ porty IO
//  sleepmgr_init();                        //optionalny modu³ ospowiedzialny za oszczedzanie energii i uœpienie
 stdio_usb_init();                       //inicjacja os³ugi portu USB
SPI_init(); //inicjalizacja SPI
Timer_init();
//uint8_t myszy[4] =[1,2,3,4];
char * test = "test";


	SendPkt(test);
	while(1) {
					if (SPI_flag==1)
					{
						PORTA.OUTCLR= SLAVE(slave_nr);
						_delay_us(100);
						SPIC.DATA=0;// Start tranmsiji miêdzy uk³adami
						
						/////////////interrupt
						
						_delay_ms(1);
						printf(" data income %d  counter %d ", RFID_ID[SPI_timer-1],SPI_timer-1);//wydruk
						printf("\x1b""[40D");
						printf("\n");
						if(SPI_timer==5)
						{
							SPI_timer=0;
							slave_nr++;
							printf ("next slave    nr %d",slave_nr);
							printf("\x1b""[40D");
							printf("\n");
							if (slave_nr==0) PORTA.OUTSET = SLAVE(7);
							else PORTA.OUTSET = SLAVE(slave_nr-1);//odznaczamy poprzedniego slava
							
						}
						//slave_nr++;//nastepny slavy
						if (slave_nr==8)//jesli ostatni to zacyznamy od pocz¹tku
						{
							slave_nr=0;	
						}
							SPI_flag=0	;//zerujemy flage timera
							
					}      


	}
}
ISR(SPIC_INT_vect) {//tranmsija zakonczona-odczytywanie danych i incjalizacja nastepnej transmisji
	
	
	RFID_ID[SPI_timer] = SPIC.DATA;//zapisujemy odebrany rejestr do zmiennej
	PORTD.OUTTGL    =    PIN2_bm;  
	SPI_timer++;// zwiekszamy licznik
	/////byte_timer++;


	
	
}


ISR(TCC0_OVF_vect) {   
	SPI_flag=1;                                // przerwanie przepe³nienia TCC0
	PORTD.OUTTGL    =    PIN3_bm;      
}


