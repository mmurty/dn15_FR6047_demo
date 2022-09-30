/*
 * port_min.c
 *
 *  Created on: 10-Sep-2022
 *      Author: Mahesh Murty
 */

#include "port_min.h"
#include "drivers/uart.h"
#include <stdio.h>

static min_rx_callback_t rxCallback[MAX_UARTS];

/* Tell MIN how much space there is to write to the serial port. This is used
 * inside MIN to decide whether to bother sending a frame or not.
 */
uint16_t min_tx_space(uint8_t port)
{
    /* Avoid unused argument warning. */
    (void)(port);

    /* We fake this as we are working with blocking tx routines. */
    return MAX_PAYLOAD;
}

/* Dummy implementation. */
void min_tx_start(uint8_t port)
{
    /* Avoid unused argument warning. */
    (void)(port);
}

/* Dummy implementation. */
void min_tx_finished(uint8_t port)
{
    /* Avoid unused argument warning. */
    (void)(port);
}

/* Send a character on the designated port. */
void min_tx_byte(uint8_t port, uint8_t byte)
{
    UartSend(port, &byte, 1);
}

/* This function is called when a valid frame is successfully received by the min stack. */
void min_application_handler(uint8_t min_id, const uint8_t *min_payload, uint8_t len_payload, uint8_t port)
{
    /* If an application rx callback is registered. */
    if(rxCallback[port] != NULL)
    {
        /* Pass received packet to the application callback function. */
        rxCallback[port](min_id, min_payload, len_payload, port);
    }
}

void min_register_rx_callback(uint8_t port, min_rx_callback_t callback)
{
    if(port < MAX_UARTS)
    {
        rxCallback[port] = callback;
    }
}

void min_deregister_rx_callback(uint8_t port)
{
    if(port < MAX_UARTS)
    {
        rxCallback[port] = NULL;
    }
}
