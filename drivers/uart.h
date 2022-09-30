/*
 * uart.h
 *
 *  Created on: 20-Nov-2021
 *      Author: Mahesh Murty
 */

#ifndef __UART_H__
#define __UART_H__              1

#include <inttypes.h>
#include "appconfig.h"

#if defined (UART_LIB_USE_FIFO)
#include "fifo.h"
#endif

#define MAX_UARTS               4

typedef struct
{
    /* Pointers to UART SFRs. */
    volatile unsigned int *UCAxCTLW0;
    volatile unsigned int *UCAxBRW;
    volatile unsigned int *UCAxMCTLW;
    volatile unsigned int *UCAxIE;
    volatile unsigned int *UCAxRXBUF;
    volatile unsigned int *UCAxIFG;
    volatile unsigned int *UCAxTXBUF;
    /* Peripheral specific settings. */
    uint16_t baudRate;
#if defined (UART_LIB_USE_FIFO)
    FIFO_BUFFER fifo;
#else
    uint16_t index;
#endif
    /* Pointer to preallocated buffer (length must be a power of 2). */
    volatile uint8_t *buffer;
    uint16_t buffLen;
    /* Reserved for future use. */
    uint8_t controlDE;
    uint8_t useRxIdle;
}uart_config_t;

void InitUart(uint8_t pid, uart_config_t *config);
void UartEnableRx(uint8_t pid, uint8_t en);
uint16_t UartGetRxByteCount(uint8_t pid);
uint16_t UartReadBytes(uint8_t pid, uint8_t *dataBuff);
uart_config_t *UartGetConfigPtr(uint8_t pid);
void UartSend(uint8_t pid, uint8_t *buff, uint16_t len);
void UartSendStr(uint8_t pid, char *buff);

#endif /* __UART_H__ */
