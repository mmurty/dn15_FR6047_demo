/*
 * appconfig.h
 *
 *  Created on: 12-May-2022
 *      Author: Mahesh Murty
 */

#ifndef __APPCONFIG_H__
#define __APPCONFIG_H__             1

/* Define Board-ID macros here. */
#define BOARD_ID_V1_0               1

/* Define application specific settings macros here. */
#define SELECT_BOARD                BOARD_ID_V1_0
#define DEBUG_UART_BAUD_RATE        115200
#define COMM_UART_BAUD_RATE         9600

/* Error codes returned by functions. */
#define APP_ERR_NONE                0
#define APP_ERR_INV_CMDID           -1
#define APP_ERR_UNKNOWN             -2
#define APP_ERR_INV_CHECKSUM        -3
#define APP_ERR_INV_LEN             -4
#define APP_ERR_INV_ARG             -5
#define APP_ERR_INV_PACKET          -6
#define APP_ERR_TIMEOUT             -7
#define APP_ERR_INV_MEM             -8
#define APP_ERR_NO_MEM              -9
#define APP_ERR_Q_EMPTY             -10

#define ARRAY_SIZE(foo)             (sizeof(foo)/sizeof(foo[0]))

#ifdef TEST
    #define STATIC
#else
    #define STATIC static
#endif

#ifndef SELECT_BOARD
    #error "Board not selected."
#endif

#endif /* __APPCONFIG_H__ */
