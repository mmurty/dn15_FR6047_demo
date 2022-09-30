/*
 * uart.c
 *
 *  Created on: 20-Nov-2021
 *      Author: Mahesh Murty
 */

#include <msp430.h>
#include "uart.h"
#include "assert.h"
#include <stdio.h>
#include <string.h>
#include "driverlib.h"

static uart_config_t *perConfig[MAX_UARTS] = { NULL };

/* ISRs.
 * Optimizing ISRs for speed as the MCU is operating @ 1MHz.
 * NOTE: Disable FIFO when operating @ 1MHz otherwise the driver
 * may not work properly at higher baud rates. */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=EUSCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(EUSCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    uart_config_t *config = perConfig[0];
    unsigned int eventId;

    eventId = __even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG);

    if(eventId == USCI_UART_UCRXIFG)
    {
            /* Add received byte to ring buffer. */
            #if defined (UART_LIB_USE_FIFO)
                FIFO_Put(&config->fifo, UCA0RXBUF);
            #else
                if(config->index < config->buffLen)
                {
                   config->buffer[config->index++] = UCA0RXBUF;
                }
            #endif
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=EUSCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(EUSCI_A3_VECTOR))) USCI_A3_ISR (void)
#else
#error Compiler not supported!
#endif
{
    uart_config_t *config = perConfig[3];
    unsigned int eventId;

    eventId = __even_in_range(UCA3IV, USCI_UART_UCTXCPTIFG);

    if(eventId == USCI_UART_UCRXIFG)
    {
            /* Add received byte to ring buffer. */
            #if defined (UART_LIB_USE_FIFO)
                FIFO_Put(&config->fifo, UCA3RXBUF);
            #else
                if(config->index < config->buffLen)
                {
                   config->buffer[config->index++] = UCA3RXBUF;
                }
            #endif
    }
}

/* Global Functions. */

void InitUart(uint8_t pid, uart_config_t *config)
{
    if(pid == 0)
    {
        /* UART0 pin initializations.
         * P2.1 RX, P2.0 TX
         */
        GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P2,
            GPIO_PIN0 + GPIO_PIN1,
            GPIO_SECONDARY_MODULE_FUNCTION
        );

        config->UCAxCTLW0 = &UCA0CTLW0;
        config->UCAxBRW = &UCA0BRW;
        config->UCAxMCTLW = &UCA0MCTLW;
        config->UCAxIE = &UCA0IE;
        config->UCAxRXBUF = &UCA0RXBUF;
        config->UCAxTXBUF = &UCA0TXBUF;
        config->UCAxIFG = &UCA0IFG;
    }
    else if(pid == 3)
    {
        /* UART3 pin initializations.
         * P8.2 RX, P8.3 TX
         */
        GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P8,
            GPIO_PIN2 + GPIO_PIN3,
            GPIO_PRIMARY_MODULE_FUNCTION
        );

        config->UCAxCTLW0 = &UCA3CTLW0;
        config->UCAxBRW = &UCA3BRW;
        config->UCAxMCTLW = &UCA3MCTLW;
        config->UCAxIE = &UCA3IE;
        config->UCAxRXBUF = &UCA3RXBUF;
        config->UCAxTXBUF = &UCA3TXBUF;
        config->UCAxIFG = &UCA3IFG;
    }
    else
    {
        /* Port not supported by this driver. */
        assert(0);
    }

    /* Do other error checks. */
    assert(config->UCAxBRW != NULL);
    assert(config->UCAxCTLW0 != NULL);
    assert(config->UCAxIE != NULL);
    assert(config->UCAxMCTLW != NULL);
    assert(config->UCAxRXBUF != NULL);
    assert(config->UCAxIFG != NULL);
    assert(config->UCAxTXBUF != NULL);
    assert(config->baudRate != 0);
    assert(config->buffer != NULL);
    assert(config->buffLen != 0);

    /* Save config variable for future reference. */
    perConfig[pid] = config;

    #if defined (UART_LIB_USE_FIFO)
        /* Init FIFO. */
        FIFO_Init(&config->fifo, config->buffer, config->buffLen);
    #else
        config->index = 0;
    #endif

    /* Reset UART H/W so that configuration can be changed. */
    *config->UCAxCTLW0 = UCSWRST;
    switch(config->baudRate)
    {
        case 9600:
            /* Setup UART clock source. */
            *config->UCAxCTLW0 |= UCSSEL__ACLK;
            *config->UCAxBRW = 3;
            /* 32768/9600 - INT(32768/9600)=0.41
             * UCBRSx value = 0x53.
             */
            *config->UCAxMCTLW |= 0x5300;
            break;
        default:
            /* Baudrate not supported by this driver.
             * Flag an error. */
            assert(0);
            break;
    }

    /* Clear Reset bit to init eUSCI. */
    *config->UCAxCTLW0 &= ~UCSWRST;
    /* Enable RX interrupt */
    *config->UCAxIE |= UCRXIE;
}

void UartEnableRx(uint8_t pid, uint8_t en)
{
    /* Check if the UART peripheral ID does not exceed the
     * max allowed peripherals.
     */
    assert(pid < MAX_UARTS);

    if(en)
    {
        (*perConfig[pid]->UCAxIE) |= UCRXIE;
    }
    else
    {
        (*perConfig[pid]->UCAxIE) &= ~UCRXIE;
    }
}

uint16_t UartGetRxByteCount(uint8_t pid)
{
    /* Check if the UART peripheral ID does not exceed the
     * max allowed peripherals.
     */
    assert(pid < MAX_UARTS);
    return perConfig[pid]->index;
}

uint16_t UartReadBytes(uint8_t pid, uint8_t *dataBuff)
{
    /* Check if the UART peripheral ID does not exceed the
     * max allowed peripherals.
     */
    assert(pid < MAX_UARTS);

    uint16_t retVal = 0;
    /* If any data available in rx buff. */
    if(perConfig[pid]->index > 0)
    {
        /* Read all the data from the rxbuff. */
        memcpy(dataBuff, (uint8_t *)perConfig[pid]->buffer, perConfig[pid]->index);
        retVal = perConfig[pid]->index;
        perConfig[pid]->index = 0;
    }

    return retVal;
}

uart_config_t *UartGetConfigPtr(uint8_t pid)
{
    /* Check if the UART peripheral ID does not exceed the
     * max allowed peripherals.
     */
    assert(pid < MAX_UARTS);

    return perConfig[pid];
}

void UartSend(uint8_t pid, uint8_t *buff, uint16_t len)
{
    uint16_t i;
    /* Check if the UART peripheral ID does not exceed the
     * max allowed peripherals.
     */
    assert(pid < MAX_UARTS);

    if(perConfig[pid]->controlDE)
    {
        /* TODO: Set DE pin. */
        /* 1000 for 1MHz and 16000 for 16MHz*/
        __delay_cycles(1000);
    }

    for(i = 0; i < len; i++)
    {
        /* Loop until tx operation is complete. */
        while(!((*perConfig[pid]->UCAxIFG) & UCTXIFG));
        /* Write 1 byte to tx register. */
        (*perConfig[pid]->UCAxTXBUF) = buff[i];
    }

    if(perConfig[pid]->controlDE)
    {
        /* TODO: Clear DE pin. */
        /* 1000 for 1MHz and 16000 for 16MHz*/
        __delay_cycles(1000);
    }
}

void UartSendStr(uint8_t pid, char *buff)
{
    UartSend(pid, (uint8_t *)buff, strlen(buff));
}
