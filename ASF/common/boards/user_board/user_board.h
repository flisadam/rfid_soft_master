/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <conf_board.h>

#define SLAVE0 PIN0_bm;
#define SLAVE1 PIN1_bm;
#define SLAVE2 PIN2_bm;
#define SLAVE3 PIN3_bm;
#define SLAVE4 PIN4_bm;
#define SLAVE5 PIN5_bm;
#define SLAVE6 PIN6_bm;
#define SLAVE7 PIN7_bm;
#define SLAVE(which) 1 << which // wyb�r slava



void SPI_init(void);
void Timer_init(void);
void board_init(void);
uint8_t SPItransmit(uint8_t data);
void Send_spi(char byte);
void OscXtal(void);
void uart_string(const char * nazwa);


// External oscillator settings.
// Uncomment and set correct values if external oscillator is used.

// External oscillator frequency
//#define BOARD_XOSC_HZ          8000000

// External oscillator type.
//!< External clock signal
//#define BOARD_XOSC_TYPE        XOSC_TYPE_EXTERNAL
//!< 32.768 kHz resonator on TOSC
//#define BOARD_XOSC_TYPE        XOSC_TYPE_32KHZ
//!< 0.4 to 16 MHz resonator on XTALS
//#define BOARD_XOSC_TYPE        XOSC_TYPE_XTAL

// External oscillator startup time
//#define BOARD_XOSC_STARTUP_US  500000


#endif // USER_BOARD_H