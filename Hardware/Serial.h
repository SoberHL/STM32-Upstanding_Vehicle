#ifndef __SERIAL_H__
#define __SERIAL_H__
#include <stdio.h>

extern char Serial_RxPacket[100];
void Serial3_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array,uint16_t Length);
void Serial_SendString(char *string);
void Serial_SendNum(uint32_t Num,uint8_t Length);
void Serial_Printf(char *format, ...);

#endif
