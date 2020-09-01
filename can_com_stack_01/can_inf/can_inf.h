#ifndef CAN_INF_H
#define CAN_INF_H

#include "typedefs.h"
#include "can_message.h"
#include "FlexCAN.h"

#define ERR_OK 0
#define ERR_QUEUE_FULL 1
#define ERR_QUEUE_POP 2

uint8_t can_inf_can_tx(uint32_t id, uint8_t dlc, uint8_t *data);
void can_lld_printf_init(void);
uint8_t debug_lld_can_queue_pop(void);

#endif
