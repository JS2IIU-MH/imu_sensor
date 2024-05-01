#include <M5Unified.h>
#include "view0.h"

void View0::show(float roll, float pitch, float yaw){
    const int init_x = 0;
    const int init_y = 20;
    const int title_width = 40;
    const int interval_y = 44;

    // M5.Display.fillScreen(BLACK);
    // M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.setTextSize(4);

    M5.Display.setCursor(init_x, init_y);
    M5.Display.printf("R: %6.2f", roll);

    M5.Display.setCursor(init_x, init_y + interval_y);
    M5.Display.printf("P: %6.2f", pitch);

    M5.Display.setCursor(init_x, init_y + interval_y * 2);
    M5.Display.printf("Y: %6.2f", yaw);

}

void View0::showDetail(float gyroX, float gyroY, float gyroZ,
                        float accX, float accY, float accZ,
                        float ave_roll, float ave_pitch, float ave_yaw,
                        float temp){
    // Display
    // M5.Display.fillScreen(BLACK);
    // M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.setTextSize(2);

    M5.Display.setCursor(0, 20);
    M5.Display.printf("gyroX, gyroY, gyroZ");
    M5.Display.setCursor(0, 42);
    M5.Display.printf("%6.2f%6.2f%6.2f o/s", gyroX, gyroY, gyroZ);

    M5.Display.setCursor(0, 64);
    M5.Display.printf("accX, accY, accZ");
    M5.Display.setCursor(0, 86);
    M5.Display.printf("%6.2f%6.2f%6.2f m/s^2", accX, accY, accZ);

    M5.Display.setCursor(0, 118);
    M5.Display.printf("yaw, roll, pitch");
    M5.Display.setCursor(0, 140);
    M5.Display.printf("%6.2f%6.2f%6.2f deg", ave_yaw, ave_roll, ave_pitch);

    M5.Display.setCursor(0, 162);
    M5.Display.printf("temp");
    M5.Display.setCursor(0, 184);
    M5.Display.printf("%6.2f degreeC", temp);

}


void View0::flushView(){
    M5.Display.fillScreen(BLACK);
}
