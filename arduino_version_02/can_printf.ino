// demo: CAN-BUS Shield, send data
#include "mcp_can.h"
#include <SPI.h>

void MCP2515_ISR(void);

void setup()
{
    Serial.begin(2000000);
    // init can bus, baudrate: 500k
    if (CAN.begin(CAN_500KBPS) == CAN_OK)
    {
        Serial.print("can init ok!!\r\n");
    }
    else
    {
        Serial.print("Can init fail!!\r\n");
    }
    attachInterrupt(0, MCP2515_ISR, FALLING); // start interrupt
}

unsigned char stmp[8] = {0, 1, 2, 3, 4, 5, 6, 7};
void loop()
{
    // send data:  id = 0x00, standrad flame, data len = 8, stmp: data buf
    // CAN.sendMsgBuf(0x00, 0, 8, stmp);
    delay(100); // send data per 100ms
}

void MCP2515_ISR(void)
{
    unsigned char len = 0;
    unsigned char buf[8];
    INT32U id = 0U;

    CAN.readMsgBuf(&len, buf); // read data,  len: data length, buf: data buf
    id = CAN.getCanId();

    switch(id)
    {
    case 0x77:
        for(int i = 0; i < len; i++)
        {
            Serial.print(char(buf[i]));
        }
        break;
    default:
        break;
    }
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
