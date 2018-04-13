/*
 * CRC.h
 *
 * Created: 2018-03-14 20:07:27
 *  Author: Adam
 */ 


#ifndef CRC_H_
#define CRC_H_


#include <avr/io.h>
#include <string.h>
#include <avr/pgmspace.h>


uint16_t doCRC16(uint8_t *Dane, uint8_t size);
void SendPkt(const char *txt);
void init_build_usb_serial_number(void);
void usb_printf_hex(uint8_t * word, uint8_t size);

#endif /* CRC_H_ */