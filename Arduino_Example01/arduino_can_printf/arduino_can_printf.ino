/*
  MCP2515 CAN Interface Using SPI

  Author: David Harding

  Created: 11/08/2010
  Modified: 6/26/12 by RechargeCar Inc.

  For further information see:

  http://ww1.microchip.com/downloads/en/DeviceDoc/21801e.pdf
  http://en.wikipedia.org/wiki/CAN_bus

  The MCP2515 Library files also contain important information.

  This sketch is configured to work with the 'Macchina' Automotive Interface board
  manufactured by RechargeCar Inc. CS_PIN and INT_PIN are specific to this board.

  This sketch shows the most basic of steps to send and receive CAN messages.

  NOTE!!!  If you use this sketch to test on a live system I suggest that you comment out the
  send messages lines unless you are certain that they will have no detrimental effect!


  This example code is in the public domain.

*/

#include <SPI.h>
#include "MCP2515_defs.h"
#include "MCP2515.h"
#include "can_inf.h"
#include "printf.h"

// Pin definitions specific to how the MCP2515 is wired up.

#define CS_PIN 10
#define INT_PIN 4
// Create CAN object with pins as defined
MCP2515 CAN(CS_PIN, INT_PIN);

// CAN message frame (actually just the parts that are exposed by the MCP2515 RX/TX buffers)


void CANHandler()
{
    CAN_FRAME message;
    uint8_t interruptFlags = CAN.Read(CANINTF);

    CAN.intHandler();
    if (CAN.GetRXFrame(message))
    {
        if(message.extended != 0)
        {
            message.extended = 1;
        }

        if (interruptFlags & RX0IF)
        {
        }

        if (interruptFlags & RX1IF)
        {
            // standard frame
        }
    }

    if (interruptFlags & TX0IF)
    {
    }

    if (interruptFlags & TX1IF)
    {
    }

    if (interruptFlags & TX2IF)
    {
    }

    if (interruptFlags & ERRIF)
    {
    }
}

void setup()
{
    Serial.begin(2000000);

    Serial.println("Initializing ...");

    // Set up SPI Communication
    // dataMode can be SPI_MODE0 or SPI_MODE3 only for MCP2515
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.begin();

    // Initialize MCP2515 CAN controller at the specified speed and clock frequency
    // (Note:  This is the oscillator attached to the MCP2515, not the Arduino oscillator)
    //speed in KHz, clock in MHz
    if (CAN.Init(500, 8))
    {
        Serial.println("MCP2515 Init OK ...");
    }
    else
    {
        Serial.println("MCP2515 Init Failed ...");
    }

    attachInterrupt(0, CANHandler, FALLING);

    Serial.println("Ready ...");

    // Application initialization
    Serial.println("");
    can_lld_printf_init();
    printf("This is printf function via CAN!\n");
}

void loop()
{
    debug_lld_can_queue_pop();
    delay(10);
}

uint8_t can_inf_can_tx(uint32_t id, uint8_t ide, uint8_t dlc, uint8_t *data)
{
    /* provide your own design to send a CAN message */
    CAN_FRAME msg;
    uint8_t i = 0U;

    msg.id = id;
    msg.extended = ide;
    msg.length = dlc;

    for(i = 0U; i < dlc; i++)
    {
        msg.data.uint8[i] = data[i];
    }

    CAN.EnqueueTX(msg);

    return 1;
}
