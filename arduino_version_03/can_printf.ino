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

void loop(void)
{
    if (Serial.available() > 0)
    {
        String can_config_str = Serial.readString();

        if (can_config_str == "250\n")
        {
            if (CAN.begin(CAN_250KBPS) == CAN_OK)
            {
                Serial.println("can baudrate is set as 250kps");
            }
        }
        else if (can_config_str == "500\n")
        {
            if (CAN.begin(CAN_500KBPS) == CAN_OK)
            {
                Serial.println("can baudrate is set as 500kps");
            }
        }
        else if (can_config_str == "1000\n")
        {
            if (CAN.begin(CAN_1000KBPS) == CAN_OK)
            {
                Serial.println("can baudrate is set as 1000kps");
            }
        }
        else
        {
            Serial.println("wrong configureation was input!");
        }
    }
    delay(100); // send data per 100ms
}

void MCP2515_ISR(void)
{
    unsigned char len = 0;
    unsigned char buf[8];
    INT32U id = 0U;

    CAN.readMsgBuf(&len, buf); // read data,  len: data length, buf: data buf
    id = CAN.getCanId();

    switch (id)
    {
    case 0x77:
        for (int i = 0; i < len; i++)
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
