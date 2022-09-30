/*
 * epsoncomm.c
 *
 *  Created on: 09-Sep-2022
 *      Author: Mahesh Murty
 */

#include "epsoncomm.h"
#include "min.h"
#include "port_min.h"
#include "bsp/board.h"
#include "drivers/uart.h"

struct MSP_TO_EPSON {
    // TOTAL SIZE: 648 BYTES
    // FLOW RELATED DATA
    uint8_t HEADER[8];
    uint8_t FLOW_ON:1;
    uint8_t FLOW_DIRECTION:1;
    uint8_t DUMMY_BYTES_1:6;
    uint8_t DUMMY_BYTES_2[7];
    uint32_t FLOW_CONDITION;
    int32_t TEMPERATURE;
    float FLOW_RATE;
    float REVERSE_CUMULATIVE_DISCHARGE;
    double CUMULATIVE_DISCHARGE;
    // DEBUG DATA
    signed int TRANSDUCER_SIGNAL_STRENGTH;
    signed int AGC_GAIN;
    signed short int MSP_POWER_ON_RESET_COUNT;
    signed short int MSP_SOFTWARE_RESET_COUNT;
    signed short int SYSTEM_ERROR_CODE;
    unsigned char DUMMY_BYTES_3[2];
    double CORRELATION_ERROR_VALUE;
    double CYCLE_SLIPS;
    //GENERAL DATA
    unsigned char MODE;
    unsigned char DN;
    unsigned char DUMMY_BYTES_4[6];
    unsigned char MSP_FIRMWARE_VERSION[8];
    unsigned char PCB_SERIAL_NUMBER[8];
    unsigned char MSP_FIRMWARE[512];
    //CALIBRATION DATA
    double CALIBRATION_PARAMS;
    float METER_CONSTANT;
    float SLOPE;
    float INTERCEPT;
    unsigned char DUMMY_BYTES_5[4];
    // FIRST RUN DATA
    unsigned char FIRMWARE_FIRST_RUN:1;
    unsigned char CALIBRAITON_FIRST_RUN:1;
    unsigned char DUMMY_BYTES_6:6;
    unsigned char DUMMY_BYTES_7[7];
    unsigned char FOOTER[8];
};

/* Allocate these variables in FRAM if running on FR6047. */
#if defined (__MSP430FR6047__)
    #pragma PERSISTENT(dataVar)
    #pragma PERSISTENT(min_ctx)
#endif

static struct MSP_TO_EPSON dataVar = {0};
static struct min_context min_ctx = {0};

static void RxCallback(uint8_t min_id, const uint8_t *min_payload, uint8_t len_payload, uint8_t port)
{
    /* Added to avoid unused variable warning. */
    (void)port;

    /* Handle all the incoming requests here. */
    switch(min_id)
    {
        case EPSON_CMD_GET_DISP_PARAMS:
            dataVar.FLOW_CONDITION++;
            min_send_frame(&min_ctx, min_id, (uint8_t *)&dataVar.HEADER, 40);
            break;
    }
}

void EpsonCommInit(void)
{
    min_init_context(&min_ctx, BOARD_COMM_UART_PID);
    min_register_rx_callback(BOARD_COMM_UART_PID, RxCallback);
    /* TODO: Initialize default values of communication variable. */
    dataVar.FLOW_RATE = 10.5f;
    dataVar.TEMPERATURE = 32;
}

void RunEpsonCommFSM(void)
{
    uint16_t rxCount;
    uint8_t rxBuff[BOARD_COMM_UART_RX_BUFF_SIZE];

    rxCount = UartReadBytes(BOARD_COMM_UART_PID, rxBuff);
    min_poll(&min_ctx, rxBuff, rxCount);
}

