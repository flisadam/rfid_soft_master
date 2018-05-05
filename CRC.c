/*
 * CRC.c
 *
 * Created: 2018-03-14 20:07:42
 *  Author: Adam
 */ 
#include <asf.h>
#include    <avr/io.h>
#include "CRC.h"
#include "conf_usb.h"
#define PKT_HEADER  0xFFFF  //Nag³ówek - znak wyró¿niaj¹cy pocz¹tek pakietu
#define PKT_HEADER2  0xFF
uint8_t serial_number[USB_DEVICE_GET_SERIAL_NAME_LENGTH];

uint16_t crc16=0;
typedef struct
{
	uint16_t  Header;     //Nag³ówek pakietu
	//uint8_t  Addr;       //Adres urz¹dzenia
	//uint8_t  Len;        //D³ugoœæ pola danych
	char     Data[0];    //Pole danych o zmiennej d³ugoœci
	uint16_t CRC16;      //CRC ca³ego pakietu
} Pakiet;


uint16_t doCRC16(uint8_t *Dane, uint8_t size)
{
	
	CRC.CTRL=CRC_RESET_RESET1_gc;
	CRC.CTRL=CRC_SOURCE_IO_gc; //CRC16, pocz¹tkowo 0xFFFF, dane z rejestru IO
	for(;size>1; size--)
	{
		//printf("%X", *Dane);
		CRC.DATAIN=*(Dane++);
	}
	CRC.STATUS=CRC_BUSY_bm; //Koniec danych
	return CRC.CHECKSUM0 | CRC.CHECKSUM1<<8;
}

void SendPkt( const char * txt)
{
	
	uint8_t pkt[sizeof(Pakiet) + strlen(txt)+1];
	Pakiet *pkt_p=(Pakiet*)pkt;
	pkt_p->Header=PKT_HEADER;
	strcpy(pkt_p->Data, txt);
	uint16_t crc16=doCRC16(pkt, sizeof(pkt) - sizeof(((Pakiet*)0)->CRC16));
	pkt_p->Data[sizeof(pkt) - sizeof(Pakiet)]=crc16>>8;
	pkt_p->Data[sizeof(pkt) - sizeof(Pakiet)+1]=crc16 & 0xFF;

	usb_printf_hex(pkt,sizeof(pkt));
	
}

	void usb_printf_hex(uint8_t * word, uint8_t size)
	{
		for (int i=0;i<size;i++)
		{
		
			//if (i!=6)// pomijamy 6 bo algorytm  wrzuca niepotrzebne 0 do pakietu
			if(*(word+i) >15)
			printf("%X",*(word+i)); //FF
			else
			printf("%X%X",0,*(word+i));
		}
	}

void init_build_usb_serial_number(void)
{
	serial_number[0] = 'a';// bajty startowe
	serial_number[1] = 'b';// bajty startowe
	serial_number[2] = 'c';
	serial_number[3] ='d';
	serial_number[4] = 'e';
	serial_number[USB_DEVICE_GET_SERIAL_NAME_LENGTH-1] = 'f';//CRC
}