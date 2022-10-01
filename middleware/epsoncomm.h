/*
 * epsoncomm.h
 *
 *  Created on: 09-Sep-2022
 *      Author: Mahesh Murty
 */

#ifndef __EPSONCOMM_H__
#define __EPSONCOMM_H__             1

#include "ussSwLib.h"

/* COMMAND-IDs. (Max limited by min protocol to 64). */
typedef enum
{
    /* 1Hz display refresh params. */
    EPSON_CMD_GET_DISP_PARAMS = 1,
    /* Debug params. */
    EPSON_CMD_GET_TRANSDUCER_STRENGTH,
    EPSON_CMD_GET_AGC_GAIN,
    EPSON_CMD_GET_MSP_POR_COUNT,
    EPSON_CMD_GET_MSP_SWR_COUNT,
    /* General data. */
    EPSON_CMD_GET_MSP_MODE,
    EPSON_CMD_SET_MSP_MODE,
    EPSON_CMD_GET_MSP_FW_VER,
    EPSON_CMD_GET_PCB_SRNO,
    EPSON_CMD_GET_DN_NO,
    /* Calibration params. */
    EPSON_CMD_GET_METER_CONST,
    EPSON_CMD_SET_METER_CONST,
    EPSON_CMD_GET_SLOPE,
    EPSON_CMD_SET_SLOPE,
    /* OTA commands. */
    EPSON_CMD_MSP_OTA_START,
    EPSON_CMD_MSP_OTA_WRITE,
    EPSON_CMD_MSP_OTA_STOP
}epson_cmd_id_t;

/* Initialize the communication stack. */
void EpsonCommInit(void);

/* Copy metrology variables to communication variables. */
void EpsonCommUpdateMetrologyParams(USS_Algorithms_Results *ptrData);

/* Runs comm state m/c periodically. */
void RunEpsonCommFSM(void);

#endif /* __EPSONCOMM_H__ */
