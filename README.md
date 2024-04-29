# imu_sensor
IMU sensor on M5Stack Core2 for AWS


## メモ
### M5Stack Core2 for AWS
- [M5Stack Core2 for AWS - ESP32 IoT開発キット — スイッチサイエンス](https://www.switch-science.com/products/6784)
- [開始方法 :: AWS IoT Kit](https://aws-iot-kit-docs.m5stack.com/ja/getting-started/)

### M5Unified.h
- [m5-docs M5Unifiedへの移植のポイント](https://docs.m5stack.com/ja/arduino/m5unified/migration)

### IMU
- MPU6886
  - Core2 for AWSにはボトムUNITにMPU6886（6軸のIMU）が搭載されている。温度センサも付属している。
  - `M5.IMU.Init()`で初期化する→`M5.Unified.h`を使う場合は`cfg.internal_imu = true`とする。以降、`M5.Imu`大文字小文字に注意。
  - `M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ)`, `M5.IMU.getAccelData(&accX, &accY, &accZ)`, `M5.getAhrsData(&pitch, &roll, &yaw)`, `M5.IMU.getTempData(&temp)`
  - `M5Unified.h`の場合は`M5.Imu.getGyroData(&gyroX, &gyroY, &gyroZ)`, `M5.Imu.getAccelData(&accX, &accY, &accZ)`, `M5.Imu.getTempData(&temp)`となる。`M5.getAhrsData(&pitch, &roll, &yaw)`の代わりがない。
- [Mahony Orientation Filter — AHRS 0.3.1 documentation](https://ahrs.readthedocs.io/en/latest/filters/mahony.html)

### PlatformIO
- [マイコン開発するならPlatformIOがめちゃくちゃいいぞ #C++ - Qiita](https://qiita.com/JotaroS/items/1930f156aab953194c9a)
- [VSCodeとPlatformIOでM5Stack Core2開発 #VSCode - Qiita](https://qiita.com/desertfox_i/items/a6ff7deaa0a0b3802bcd)
- [VScode+PlatformIOでポートを固定する | 株式会社マグノリア](https://magnolia-tech.jp/vscode-platformio-comport-set/)
