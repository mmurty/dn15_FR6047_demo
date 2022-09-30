/*
 * board_v1_0.c
 *
 *  Created on: 09-Sep-2022
 *      Author: Mahesh Murty
 */
#include "board.h"
#if (SELECT_BOARD == BOARD_ID_V1_0)

#include "driverlib.h"
#include "drivers/uart.h"

/* Allocate these variables in FRAM if running on FR6047. */
#if defined (__MSP430FR6047__)
    #pragma PERSISTENT(commUart)
    #pragma PERSISTENT(commUartRxBuff)
#endif

static uart_config_t commUart = { 0 };
static volatile uint8_t commUartRxBuff[BOARD_COMM_UART_RX_BUFF_SIZE] = { 0 };

void Board_InitPeripherals(void)
{
    /* Initialize all the application specific peripherals. */

    /* Init COMM UART port. */
    commUart.baudRate = BOARD_COMM_UART_BAUDRATE;
    /* Setup rx buffer. */
    commUart.buffer = commUartRxBuff;
    commUart.buffLen = sizeof(commUartRxBuff);
    InitUart(BOARD_COMM_UART_PID, &commUart);
}

#endif /* SELECT_BOARD */

