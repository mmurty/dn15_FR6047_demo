/*
 * port_min.h
 *
 *  Created on: 10-Sep-2022
 *      Author: Mahesh Murty
 */

#ifndef __PORT_MIN_H__
#define __PORT_MIN_H__              1

#include "min.h"

typedef void (*min_rx_callback_t)(uint8_t min_id, const uint8_t *min_payload, uint8_t len_payload, uint8_t port);

void min_register_rx_callback(uint8_t port, min_rx_callback_t callback);
void min_deregister_rx_callback(uint8_t port);

#endif /* __PORT_MIN_H__ */
