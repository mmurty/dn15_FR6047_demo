/*
 * board.h
 *
 *  Created on: 09-Sep-2022
 *      Author: Mahesh Murty
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include "appconfig.h"
#include <stdint.h>

#if (SELECT_BOARD == BOARD_ID_V1_0)
    #include "board_v1_0.h"
#endif

void Board_InitPeripherals(void);

#endif /* __BOARD_H__ */
