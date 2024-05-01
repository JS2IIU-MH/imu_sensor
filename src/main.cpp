// #include <M5Stack.h>
// #include <M5Core2.h>
#include <M5Unified.h>
#include <MahonyAHRS.h>

#define MAX_QUEUE_LENGTH 10
#define GYRO_CAL_THRESHOLD 20
#define CAL_COUNT 500
#define YAW_DELTA_LIM 200

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
float sum = 0.0F;
float ave_pitch = 0.0F;
float ave_roll = 0.0F;
float ave_yaw = 0.0F;

// calibration
uint8_t calibration = 0;
uint16_t calibrationCount = 0;
float pitchSum = 0.0F;
float rollSum = 0.0F;
float yawSum = 0.0F;
float pitchOffset = 0.0F;
float rollOffset = 0.0F;
float yawOffset = 0.0F;
float yawOld = 0.0F;
float yaw2 = 0.0F;

float temp = 0.0F;

// array for SMA calculate
float Qroll[MAX_QUEUE_LENGTH];
float Qpitch[MAX_QUEUE_LENGTH];
float Qyaw[MAX_QUEUE_LENGTH];


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

  // simple moving average process 
  for(int i = 0; i < MAX_QUEUE_LENGTH - 1; i++){
    Qroll[i] = Qroll[i+1];
    Qpitch[i] = Qpitch[i+1];
    Qyaw[i] = Qyaw[i+1];
  }
  // add latest data at the end of array
  Qroll[MAX_QUEUE_LENGTH - 1] = roll;
  Qpitch[MAX_QUEUE_LENGTH - 1] = pitch;
  Qyaw[MAX_QUEUE_LENGTH - 1] = yaw;

  // Calibration for roll, pitch, yaw
  if(calibration == 0){
    calibrationCount++;
    if(200 <= calibrationCount){
      calibration = 1;
      calibrationCount = 0;
      yawOld = yaw;
    }
  } else if(calibration == 1){
    float gyro = abs(gyroX) + abs(gyroY) + abs(gyroZ);
    if(GYRO_CAL_THRESHOLD < gyro){
      calibrationCount = 0;
      rollSum = 0;
      pitchSum = 0;
      yawSum = 0;
      Serial.println("##### Gyro cal");
    } else {
      rollSum += roll;
      pitchSum += pitch;
      yawSum += (yawOld - yaw);
      yawOld = yaw;
      calibrationCount++;
      if(CAL_COUNT <= calibrationCount){
        calibration = 2;
        rollOffset = rollSum / calibrationCount;
        pitchOffset = pitchSum / calibrationCount;
        yawOffset = yawSum / calibrationCount;
      }
    }
  } else {
    // adjust roll, pitch value by offset
    // yaw is not adjusted
    // TODO: substitute yaw by yaw2
    roll -= rollOffset;
    pitch -= pitchOffset;
    float yawDelta = yawOld - yaw;
    if (-YAW_DELTA_LIM < yawDelta && yawDelta < YAW_DELTA_LIM) {
      yaw2 -= yawDelta - yawOffset;
      yaw2 = yaw2 * 0.9;
    }
  }

  // calculate SMA respectively
  for(int i = 0; i < MAX_QUEUE_LENGTH - 1; i++) sum = sum + Qroll[i];
  ave_roll = sum / float(MAX_QUEUE_LENGTH);
  sum = 0.0F;
  for(int i = 0; i < MAX_QUEUE_LENGTH - 1; i++) sum = sum + Qpitch[i];
  ave_pitch = sum / float(MAX_QUEUE_LENGTH);
  sum = 0.0F;
  for(int i = 0; i < MAX_QUEUE_LENGTH - 1; i++) sum = sum + Qyaw[i];
  ave_yaw = sum / float(MAX_QUEUE_LENGTH);
  sum = 0.0F;

  Serial.printf("%6.2f %6.2f %6.2f\n", ave_yaw, ave_roll, ave_pitch);
  // Serial.printf("%6.2f %6.2f %6.2f\n", yaw, roll, pitch);
  
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
  M5.Display.printf("%6.2f%6.2f%6.2f deg", ave_yaw, ave_roll, ave_pitch);

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

  // queue array initialize
  for(int i = 0; i < MAX_QUEUE_LENGTH; i++){
    Qpitch[i] = 0.0F;
    Qroll[i] = 0.0F;
    Qyaw[i] = 0.0F;
  }

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
