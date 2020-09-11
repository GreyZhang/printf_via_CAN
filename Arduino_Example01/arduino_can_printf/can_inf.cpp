#include "can_inf.h"

#define DEBUG_CAN_ENALE 1
#define DEBUG_BUFFER_SIZE 128

#if DEBUG_CAN_ENALE
uint8_t can_message_queue[DEBUG_BUFFER_SIZE];
CAN_DATATYPE debug_lld_msg;
#endif


uint8_t debug_lld_can_queue_append(uint8_t c)
{
#if DEBUG_CAN_ENALE
    uint32_t i = 0U;

    for (i = 0; i < DEBUG_BUFFER_SIZE; i++)
    {
        if (can_message_queue[i] == 0xFFU)
        {
            if (i == (DEBUG_BUFFER_SIZE - 1))
            {
                /* buffer is full, report an error */
                return ERR_QUEUE_FULL;
            }
            else
            {
                can_message_queue[i] = c;
                can_message_queue[i + 1] = 0xFFU;
                return ERR_OK;
            }
        }
        else
        {
            /* check the next element */
        }
    }

#endif
    /* should not come here */
    return ERR_QUEUE_FULL;
}

uint8_t debug_lld_can_queue_pop(void)
{
    uint32_t i = 0U;
    uint32_t j = 0U;
    /* uint8_t tx_c = 0U; */

    if (can_message_queue[0] != 0xFF)
    {
        /* data is valid, tx a frame after prepare the new queue. */
        for(i = 1U; i < DEBUG_BUFFER_SIZE; i++)
        {
            if(can_message_queue[i] == 0xFFU)
            {
                /* this is then end idx and it is also the length of the characters need to be sent */
                if(i <= 8U)
                {
                    can_inf_can_tx(CAN_ID_PRINTF, 0U, i, can_message_queue);

                    for(j = 0U; j < i; j++)
                    {
                        can_message_queue[j] = 0xFFU;
                    }
                }
                else
                {
                    can_inf_can_tx(CAN_ID_PRINTF, 0U, 8U, can_message_queue);
                    for(j = 0; j < i - 8U; j++)
                    {
                        can_message_queue[j] = can_message_queue[j + 8U];
                    }

                    for(j = 0U; j < 8U; j++)
                    {
                        can_message_queue[j + i - 8U] = 0xFFU;
                    }
                }
                break;
            }
        }
    }

    return ERR_OK;
}

void can_lld_printf_init(void)
{
    uint32_t i = 0U;

    for(i = 0U; i < DEBUG_BUFFER_SIZE; i++)
    {
        can_message_queue[i] = 0xFFU;
    }

    debug_lld_msg.ID = CAN_ID_PRINTF;
    debug_lld_msg.Extended = 0U;
    debug_lld_msg.Length = 0U;
    debug_lld_msg.Remote = 0U;
}

