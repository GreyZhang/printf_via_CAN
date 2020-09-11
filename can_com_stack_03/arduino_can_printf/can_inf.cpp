#include "can_inf.h"

#define CAN_PRINTF_TX_QUEUE_LENGTH 128

uint8_t can_printf_chars_queue[CAN_PRINTF_TX_QUEUE_LENGTH];
uint8_t can_printf_queue_tail = 0x0U;
uint8_t can_printf_queue_head = 0x0U;
uint8_t can_printf_chars_num = 0U;


uint8_t can_printf_en_queue(uint8_t c)
{
    if(can_printf_chars_num >= CAN_PRINTF_TX_QUEUE_LENGTH)
    {
        return ERR_QUEUE_FULL;
    }

    can_printf_chars_queue[can_printf_queue_tail] = c;
    can_printf_queue_tail = (can_printf_queue_tail + 1) % CAN_PRINTF_TX_QUEUE_LENGTH;
    can_printf_chars_num += 1U;

    return ERR_OK;
}

uint8_t can_printf_de_queue(uint8_t *p_c)
{
    if(can_printf_chars_num == 0U)
    {
        return ERR_QUEUE_EMPTY;
    }

    *p_c = can_printf_chars_queue[can_printf_queue_head];
    can_printf_queue_head = (can_printf_queue_head + 1U) % CAN_PRINTF_TX_QUEUE_LENGTH;
    can_printf_chars_num -= 1U;

    return ERR_OK;
}

uint8_t debug_lld_can_queue_pop(void)
{
    uint8_t can_data_idx = 0U;
    uint8_t c = 0U;
    uint8_t data[8];

    if(can_printf_chars_num == 0U)
    {
        return ERR_OK;
    }

    while((can_data_idx < 8) && (ERR_OK == can_printf_de_queue(&c)))
    {
        data[can_data_idx] = c;
        can_data_idx += 1U;
    }

    can_inf_can_tx(CAN_ID_PRINTF, 0, can_data_idx, data);

    return ERR_OK;
}

