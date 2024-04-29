// #include <M5Stack.h>
// #include <M5Core2.h>
#include <M5Unified.h>
#include <MahonyAHRS.h>

Mahony filter;

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

float pitch = 0.0F;
float roll = 0.0F;
float yaw = 0.0F;

float temp = 0.0F;

void setup() {
  // put your setup code here, to run once:
  // Init M5Core
  auto cfg = M5.config();
  cfg.serial_baudrate = 115200;
  cfg.output_power = true;
  cfg.internal_imu = true;

  M5.begin(cfg);

  // LCD Screen setting
  M5.Display.fillScreen(BLACK);
  M5.Display.setTextColor(WHITE, RED);
  M5.Display.setTextSize(2);

  // Mahony filter
  filter.begin(100); // filter to expect 100 measurements per second

}

void loop() {
  // put your main code here, to run repeatedly:
  M5.update();

  // Stores the triaxial gyroscope data of the inertial sensor to the relevant
  M5.Imu.getGyroData(&gyroX, &gyroY, &gyroZ);
  // Stores the triaxial accelerometer
  M5.Imu.getAccelData(&accX, &accY, &accZ);
  // Stores the inertial sensor attitude
  filter.updateIMU(gyroX, gyroY, gyroZ, accX, accY, accZ);
  roll = filter.getRoll();
  pitch = filter.getPitch();
  yaw = filter.getYaw();

  Serial.printf("%6.2f %6.2f %6.2f\n", yaw, roll, pitch);
  
  // M5.Imu.getAhrsData(&pitch, &roll, &yaw);
  // Stores the inertial sensor temperature
  M5.Imu.getTemp(&temp);

  // Display
  M5.Display.setCursor(0, 20);
  M5.Display.printf("gyroX,  gyroY, gyroZ");
  M5.Display.setCursor(0, 42);
  M5.Display.printf("%6.2f%6.2f%6.2f o/s", gyroX, gyroY, gyroZ);

  delay(10);

}
