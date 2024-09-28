#include <LD19.h>

LD19 lidar;

int lidar_x_max = 0;
int lidar_x_min = 0;
int lidar_y_max = 0;
int lidar_y_min = 0;

int lidar_feldx = 0;
int lidar_feldy = 0;
int lidar_differenz = 100;


int feldx = 1820;
int feldy = 2430;
int botx = 0;
int boty = 0;


void setup() {
  Serial.begin(115200);
  lidar.begin(&Serial1, true);
}

void loop() {
  lidar_auslesen();
}




void lidar_auslesen() {
  lidar.loop();
  for (int i = 0; i < 450; i++) {
    if (lidar.lidar_points[i].x > lidar_x_max) {
      lidar_x_max = lidar.lidar_points[i].x;
    }
    if (lidar.lidar_points[i].x < lidar_x_min) {
      lidar_x_min = lidar.lidar_points[i].x;
    }
    if (lidar.lidar_points[i].y > lidar_y_max) {
      lidar_y_max = lidar.lidar_points[i].y;
    }
    if (lidar.lidar_points[i].y < lidar_y_min) {
      lidar_y_min = lidar.lidar_points[i].y;
    }
  }

  lidar_feldx = lidar_x_max - lidar_x_min;
  lidar_feldy = lidar_y_max - lidar_y_min;
  if (lidar_feldx < feldx - lidar_differenz || lidar_feldx > feldx + lidar_differenz) {
    lidar_feldx = 0;
  }
  if (lidar_feldy < feldy - lidar_differenz || lidar_feldy > feldy + lidar_differenz) {
    lidar_feldy = 0;  // 0 bedeutet hier ein Fehlerwert
  }

  botx = lidar_x_min * (-1);
  boty = lidar_y_min * (-1);

  Serial.print("Botposition:  x ");
  Serial.print(botx);
  Serial.print("  y ");
  Serial.println(boty);

  lidar_x_max = 0;
  lidar_x_min = 0;
  lidar_y_max = 0;
  lidar_y_min = 0;
}
