#include <ACAN2515.h>
#include <ACAN2515Settings.h>
#include <ACANBuffer.h>
#include <ACANBuffer16.h>
#include <CANMessage.h>
#include <MCP2515ReceiveFilters.h>

//——————————————————————————————————————————————————————————————————————————————
//  ACAN2515 Demo in loopback mode
//——————————————————————————————————————————————————————————————————————————————

#include <ACAN2515.h>

#define DEBUG_MODE 1

//——————————————————————————————————————————————————————————————————————————————
//  MCP2515 connections:
//    - standard SPI pins for SCK, MOSI and MISO
//    - a digital output for CS
//    - interrupt input pin for INT
//——————————————————————————————————————————————————————————————————————————————
// If you use CAN-BUS shield (http://wiki.seeedstudio.com/CAN-BUS_Shield_V2.0/) with Arduino Uno,
// use B connections for MISO, MOSI, SCK, #9 or #10 for CS (as you want),
// #2 or #3 for INT (as you want).
//——————————————————————————————————————————————————————————————————————————————

static const byte MCP2515_CS = 10; // CS input of MCP2515 (adapt to your design)
static const byte MCP2515_INT = 3; // INT output of MCP2515 (adapt to your design)

//——————————————————————————————————————————————————————————————————————————————
//  MCP2515 Driver object
//——————————————————————————————————————————————————————————————————————————————

ACAN2515 can(MCP2515_CS, SPI, MCP2515_INT);

//——————————————————————————————————————————————————————————————————————————————
//  MCP2515 Quartz: adapt to your design
//——————————————————————————————————————————————————————————————————————————————

static const uint32_t QUARTZ_FREQUENCY = 8UL * 1000UL * 1000UL; // 8 MHz

//——————————————————————————————————————————————————————————————————————————————
//   SETUP
//——————————————————————————————————————————————————————————————————————————————

void setup()
{
    //--- Switch on builtin led
    //  pinMode (LED_BUILTIN, OUTPUT) ;
    //  digitalWrite (LED_BUILTIN, HIGH) ;
    //--- Start serial
    Serial.begin(2000000);
    //--- Wait for serial (blink led at 10 Hz during waiting)
    while (!Serial)
    {
        delay(50);
        //    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    }
    //--- Begin SPI
    SPI.begin();
    //--- Configure ACAN2515
    Serial.println("Configure ACAN2515");
    ACAN2515Settings settings(QUARTZ_FREQUENCY, 500UL * 1000UL); // CAN bit rate 500 kb/s
    settings.mRequestedMode = ACAN2515Settings::NormalMode;      // Select loopback mode
    const uint16_t errorCode = can.begin(settings, [] { can.isr(); });
    if (errorCode == 0)
    {
        Serial.print("Bit Rate prescaler: ");
        Serial.println(settings.mBitRatePrescaler);
        Serial.print("Propagation Segment: ");
        Serial.println(settings.mPropagationSegment);
        Serial.print("Phase segment 1: ");
        Serial.println(settings.mPhaseSegment1);
        Serial.print("Phase segment 2: ");
        Serial.println(settings.mPhaseSegment2);
        Serial.print("SJW: ");
        Serial.println(settings.mSJW);
        Serial.print("Triple Sampling: ");
        Serial.println(settings.mTripleSampling ? "yes" : "no");
        Serial.print("Actual bit rate: ");
        Serial.print(settings.actualBitRate());
        Serial.println(" bit/s");
        Serial.print("Exact bit rate ? ");
        Serial.println(settings.exactBitRate() ? "yes" : "no");
        Serial.print("Sample point: ");
        Serial.print(settings.samplePointFromBitStart());
        Serial.println("%");
    }
    else
    {
        Serial.print("Configuration error 0x");
        Serial.println(errorCode, HEX);
    }
}

//------------------------------------------------------------------------------

static uint32_t gBlinkLedDate = 0;
static uint32_t gReceivedFrameCount = 0;
static uint32_t gSentFrameCount = 0;

//——————————————————————————————————————————————————————————————————————————————
CANMessage frame;
char putc_char;
uint32_t counter = 0U;

void loop()
{
#if DEBUG_MODE
    frame.id = 0x76;
    frame.len = 8;
    frame.ext = false;
    frame.rtr = false;
    frame.idx = 0x0;
    if (can.tryToSend(frame))
    {
        Serial.println(counter++);
    }
    delay(100);
#endif
    if (can.available())
    {
        can.receive(frame);
        if (0x77 == frame.id)
        {
            memcpy(&putc_char, frame.data, 1);
            Serial.print(putc_char);
        }
    }
}
