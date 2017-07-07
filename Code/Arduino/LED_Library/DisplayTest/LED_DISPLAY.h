#ifndef LED_DISPLAY_H
#define LED_DISPLAY_H

#include <SPI.h>

class LED_DISPLAY
{
public:
  // constructors
  LED_DISPLAY();
  LED_DISPLAY(long SPI_Frequency);

  // initialization method
  bool makeDerricksBlinkyBlinky();

  // methods for reading and processing SPI data
  void  readToSPIBuffer();
  int   processSPIBuffer();
  
  void  printSPI_DATA();

  //methods to retrieve RPM and Gear number
  int   getRPMData();
  int   getCurrentGearState();

  // methods for 7 segment display
  void  displayGear(int num);
  void  displayNeutral();
  void  displayDecimalPoint();
  void  segOff();

  // methods to set pins values [not important right now]
  void  setRPM_PIN(int num);
  void  setDisplayTypePin(int num);
private:
  int   RPM_PIN;     // RPM pin number
  int   displayType; // Display pin number 
  
  int   rpmData;     // current RPM value
  int   gear;        // current gear value

  volatile int bufferPosition; // used as index for the 'spiDataPakcet' array
  uint8_t spiDataPacket[3];    // SPI data packet
  bool  isrFlag;                // bool used to detect new interrupts

  int   segArr[8] = {40, 42, 32, 34, 36, 38, 44, 30};
  //                A,  B,  C,  D,  E,  F,  G, DP

  //                      0,1,2,3,4,5,6,7    <= indeces of 'segCode'
  int   segCode[9][8] = {  {1,1,1,1,1,1,1,1},    /* Turns display off      */
                           {1,1,0,1,1,1,0,1},    /* Displays one           */
                           {1,0,1,0,0,0,0,1},    /* Displays two           */
                           {1,0,0,0,1,0,0,1},    /* Displays three         */
                           {0,0,0,1,1,1,0,1},    /* Displays four          */
                           {0,0,0,0,1,0,1,1},    /* Displays five          */
                           {0,0,0,0,0,0,1,1},    /* Displays six           */
                           {1,0,0,1,0,1,1,1},    /* Displays neutral       */
                           {1,1,1,1,1,1,1,0}  }; /* Displays decimal point */

  // Declaration of common anodes on LED Bar
  int   comAnodeArr[16] = {46,48,49,47,45,43,41,39,37, 33, 35, 31, 28, 26, 24, 22};
  //                       C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11,C12,C13,C14,C15,C16

  // Declaration of selection array
  int   color[3][3] = { { 2,3,4},      /* Red1, Red2, Red3       */
                        {10,9,8},      /* Green1, Green2, Green3 */
                        { 7,6,5} };    /* Blue1, Blue2, Blue3    */ 
};

#endif