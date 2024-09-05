#include <LD19.h>

LD19 lidar;

void setup() {
  Serial.begin(115200);
  lidar.begin(&Serial7, true);
}

void loop() {
  lidar.loop();
}
