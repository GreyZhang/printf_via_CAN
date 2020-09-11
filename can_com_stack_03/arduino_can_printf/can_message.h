#ifndef CAN_MESSAGE_H
#define CAN_MESSAGE_H

#include "stdint.h"

typedef struct
{
    uint32_t  ID;
    uint8_t   Length;
    uint8_t   Extended;
    uint8_t   Remote;
    uint8_t   Data[8];
} CAN_DATATYPE;


#endif /* _CAN_MESSAGE_H_ */

