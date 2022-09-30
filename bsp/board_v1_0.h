/*
 * board_v1_0.h
 *
 *  Created on: 09-Sep-2022
 *      Author: Mahesh Murty
 */

#ifndef __BOARD_V1_0_H__
#define __BOARD_V1_0_H__

#include "board.h"

#if (SELECT_BOARD == BOARD_ID_V1_0)

#define BOARD_COMM_UART_PID             0
/* NOTE: Only a few limited baudrates are supported. */
#define BOARD_COMM_UART_BAUDRATE        9600
#define BOARD_COMM_UART_RX_BUFF_SIZE    32

#endif /* SELECT_BOARD */

#endif /* __BOARD_V1_0_H__ */
