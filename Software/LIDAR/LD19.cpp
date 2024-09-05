/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Updated by Jason A. Cox, February 18, 2023.
  Released into the public domain.
*/

#include "Arduino.h"
#include "LD19.h"

LD19::LD19()
{
}
void LD19::begin(HardwareSerial *_port)
{
  begin(_port, false);
}

void LD19::begin(HardwareSerial *_port, bool _debug)
{
  serialPort = _port;
  serialPort->begin(230400, SERIAL_8N1);
  debug = _debug;
}

void LD19::loop()
{
  if (!(serialPort->available() == 0))
  {
    // read the incoming bytes:
    uint8_t header = serialPort->read();
    if (header == startchar)
    {
      serialPort->readBytes(buf, BUFFER_SIZE);

      uint8_t crcbuffer[46];
      crcbuffer[0] = 0x54;
      for (int i = 0; i < 45; i++)
      {
        crcbuffer[i + 1] = buf[i];
      }
      uint8_t crcc = CalCRC8(crcbuffer, 46);

      if (crcc == buf[45])
      {
        uint16_t startAngle = (AddLSB(buf[startAngleLSB], buf[startAngleMSB]));
        uint16_t endAngle = (AddLSB(buf[endAngleLSB], buf[endAngleMSB]));

        uint16_t angleDiff;
        if (startAngle < endAngle)
          angleDiff = (endAngle - startAngle) / 12;
        else
          angleDiff = ((endAngle + 36000) - startAngle) / 12;

        for (int i = 0; i < 12; i++)
        {
          current_point_cloud_position++;
          if (current_point_cloud_position >= POINTCLOUD_SIZE)
            current_point_cloud_position = 0;

          uint16_t distance_data[12];
          uint8_t intensity_data[12];
          distance_data[i] = AddLSB(buf[5 + 3 * i], buf[6 + 3 * i]);
          intensity_data[i] = buf[7 + 3 * i];

          uint16_t angle_data = startAngle + angleDiff * i;
          double radiant_data = (double)angle_data / 406800.0 * 71;

          lidar_points[current_point_cloud_position].x = sin(radiant_data) * distance_data[i];
          lidar_points[current_point_cloud_position].y = cos(radiant_data) * distance_data[i];
          lidar_points[current_point_cloud_position].intensity = intensity_data[i];
        }
      }
    }
    if (debug)
    {

      // // only for printing
      // if (count_print >= 20)
      // {
      //   for (int i = 0; i < POINTCLOUD_SIZE - 1; i++)
      //   {
      //     Serial.print(String(int(lidar_points[i].x)) + "p" + String(int(lidar_points[i].y)) + "p" + String(int(0)) + "q");
      //   }
      //   Serial.println(String(int(lidar_points[POINTCLOUD_SIZE - 1].x)) + "p" + String(int(lidar_points[POINTCLOUD_SIZE - 1].y)) + "p" + String(int(0)) + "q");
      //   count_print = 0;
      // }
      // else
      //   count_print++;
      // send start bytes
      Serial.write(0);
      Serial.write(0);
      // send x coords
      for (int i = 0; i < POINTCLOUD_SIZE; i++){
        if(lidar_points[i].x == 0)lidar_points[i].x = 1;
        Serial.write(highByte(lidar_points[i].x));
        Serial.write(lowByte(lidar_points[i].x));
      }
      // send y coords

      for (int i = 0; i < POINTCLOUD_SIZE; i++){
        if(lidar_points[i].y == 0)lidar_points[i].y = 1;
        Serial.write(highByte(lidar_points[i].y));
        Serial.write(lowByte(lidar_points[i].y));
      }
    }
  }
}

uint16_t LD19::AddLSB(uint8_t Low, uint8_t High)
{
  return High * 256 + Low;
}

uint8_t LD19::CalCRC8(uint8_t *p, uint8_t len)
{
  uint8_t crc = 0;
  uint16_t i;
  for (i = 0; i < len; i++)
  {
    crc = CrcTable[(crc ^ *p++) & 0xff];
  }
  return crc;
}
