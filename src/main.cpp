// #include <M5Stack.h>
// #include <M5Core2.h>
#include <M5Unified.h>
#include <MahonyAHRS.h>

Mahony filter;

hw_timer_t *timer = NULL;

volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

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

void sensorUpdate(){
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
  M5.Display.printf("gyroX, gyroY, gyroZ");
  M5.Display.setCursor(0, 42);
  M5.Display.printf("%6.2f%6.2f%6.2f o/s ", gyroX, gyroY, gyroZ);

  M5.Display.setCursor(0, 64);
  M5.Display.printf("accX, accY, accZ");
  M5.Display.setCursor(0, 86);
  M5.Display.printf("%6.2f%6.2f%6.2f m/s^2", accX, accY, accZ);

  M5.Display.setCursor(0, 118);
  M5.Display.printf("yaw, roll, pitch");
  M5.Display.setCursor(0, 140);
  M5.Display.printf("%6.2f%6.2f%6.2f deg", yaw, roll, pitch);

  M5.Display.setCursor(0, 162);
  M5.Display.printf("temp");
  M5.Display.setCursor(0, 184);
  M5.Display.printf("%6.2f degreeC", temp);
}

void IRAM_ATTR onTimer(){
  // run onTimer at timer interaption
  portENTER_CRITICAL_ISR(&timerMux);

  portEXIT_CRITICAL_ISR(&timerMux);
  // Give a semaphore that we can check in the loop
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
  // It is safe to use digitalRead/Write here if you want to toggle an output
}

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
  M5.Display.setTextColor(WHITE, BLACK);
  M5.Display.setTextSize(2);

  // Mahony filter
  filter.begin(100);
  // filter to expect 100 measurements per second
  // see interapt timing setting below

  // timer semaphore
  timerSemaphore = xSemaphoreCreateBinary();

  // Timer setting 1 count = 1 MHz
  timer = timerBegin(0, 80, true);
  // timer interaption func call
  timerAttachInterrupt(timer, &onTimer, true);
  // interapt timing = 1 MHz(1/1000000) * 10000 = 1/100 sec
  timerAlarmWrite(timer, 10000, true);
  // enable timer
  timerAlarmEnable(timer);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(xSemaphoreTake(timerSemaphore, 0) == pdTRUE){
    sensorUpdate();
  }

}
