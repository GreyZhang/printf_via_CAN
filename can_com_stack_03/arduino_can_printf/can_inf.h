#ifndef CAN_INF_H
#define CAN_INF_H

#include "stdint.h"
#include "can_message.h"

#define ERR_OK 0
#define ERR_QUEUE_FULL 1
#define ERR_QUEUE_EMPTY 2
#define ERR_QUEUE_POP 3

/* replace this ID according to your arduino design, here we use standar frame with ID 0x77U */
#define CAN_ID_PRINTF 0x77U

/* Send CAN message
* id: CAN iD
* ide: standard or extended frame, 0 for standard and 1 for extended
* dlc: length of data field
* data: pointer to data source of CAN message
*/
uint8_t can_inf_can_tx(uint32_t id, uint8_t ide, uint8_t dlc, uint8_t *data);
/* debug_lld_can_queue_pop can be called periodically such as 10ms, as long as the CAN
* Bus can process and no overload happens.
*/
uint8_t debug_lld_can_queue_pop(void);

#endif
