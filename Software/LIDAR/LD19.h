/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Updated by Jason A. Cox, February 18, 2023.
  Released into the public domain.
*/

#ifndef LD19_h
#define LD19_h

#include "Arduino.h"
#include "CRCTable.h"

class LD19
{
  private:
    static constexpr int BUFFER_SIZE = 46;
    static constexpr int POINTCLOUD_SIZE = 400;
    static constexpr int startchar = 0x54;
    static constexpr char endChar = startchar;
    static constexpr int speedLSB = 1;
    static constexpr int speedMSB = 2;
    static constexpr int startAngleLSB = 3;
    static constexpr int startAngleMSB = 4;
    static constexpr int endAngleLSB = 41;
    static constexpr int endAngleMSB = 42;
  public:
    LD19();
    void begin(HardwareSerial* _port);
    void begin(HardwareSerial* _port, bool _debug);
    void loop();

    struct lidarPointVec {
      int16_t x;
      int16_t y;
      int16_t intensity;
    };
    
    lidarPointVec lidar_points[POINTCLOUD_SIZE];

  private:
    HardwareSerial* serialPort;
    bool debug;
    unsigned char buf[BUFFER_SIZE];
    uint16_t current_point_cloud_position = 0;
    int count_print = 0;
    uint16_t AddLSB(uint8_t Low, uint8_t High);
    uint8_t CalCRC8(uint8_t *p, uint8_t len);
};



#endif

