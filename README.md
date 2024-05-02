# imu_sensor
`M5Stack Core2 for AWS`に搭載されたIMUを動作させる
- `M5Unified.h`を使用する
- IMUのyaw, roll, pitch の計算は`Mahony Orientation Filter`を使用する
- センサ値はキャリブレーション、オフセット処理を行う

開発環境は`VSCode + Platform.io`です。

## アプリの概要
- M5Stack内蔵IMUデータを画面表示する
  - 100Hzでセンサデータを読み出して、10Hzでデータ表示を更新する
  - roll, pitch, yawの値は100ms区間の平均値を表示・出力する
- 表示画面が2種類、詳細表示、簡易表示があり、それらをボタンAをタッチすることで切り替える
- roll, pitch, yawの値はUARTで出力する

<img src="doc\imu_sensor_img.png" width=600>

| release ver. | comment | date |
|---|---|---|
| [v0.0](releases/tag/v0.0) | Initial release. | 2024-05-01 |

## メモ
### M5Stack Core2 for AWS
- [M5Stack Core2 for AWS - ESP32 IoT開発キット — スイッチサイエンス](https://www.switch-science.com/products/6784)
- [開始方法 :: AWS IoT Kit](https://aws-iot-kit-docs.m5stack.com/ja/getting-started/)

### M5Unified.h
- [m5-docs M5Unifiedへの移植のポイント](https://docs.m5stack.com/ja/arduino/m5unified/migration)
- 基本設定を`platformio.ini`に書く必要がある。ArduinoIDEの時にはGUIで設定していた内容。以下のページに詳細あり。
  - [Espressif 32 — PlatformIO v6.1 documentation](https://docs.platformio.org/en/latest/platforms/espressif32.html)
- timer関連
  - `M5.config()`のところでは設定する必要なし。`hw_timer_t * timer = NULL;`の設定を行う。
    - ESP32でタイマー割り込みを使うには、次の4つのポイントを押さえておく必要があります。
      - グローバル変数の準備
      - セットアップで行う初期化処理
      - 割込処理（ISR: Interrupt Service Routin）関数の構成
      - メインループでの割込検知処理
  - こちらのページが詳しく書かれていて参考になる：[ESP32でのタイマー割り込みによる自動計測](http://marchan.e5.valueserver.jp/cabin/comp/jbox/arc202/doc21105.html)
  - [m5-docs](https://docs.m5stack.com/en/arduino/m5unified/m5unified_appendix)
  - [ESP32の高精度タイマー割り込みを調べる | Lang-ship](https://lang-ship.com/blog/work/esp32-timer/)
  - [・ESP32 リセットを繰り返す rst:0x3 (SW_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT) : Skyzoo ヨッシーの備忘録](http://gijin77.blog.jp/archives/33372499.html)
  - [Arduino IDEでESP-WROOM-02のボード設定について #Arduino - Qiita](https://qiita.com/umi_kappa/items/ced90c258a14be165291)
- [【推奨】lib_depsを指定してProject毎にインストールする](https://qiita.com/watarukato/items/455c74c625e178de385d#%E6%8E%A8%E5%A5%A8lib_deps%E3%82%92%E6%8C%87%E5%AE%9A%E3%81%97%E3%81%A6project%E6%AF%8E%E3%81%AB%E3%82%A4%E3%83%B3%E3%82%B9%E3%83%88%E3%83%BC%E3%83%AB%E3%81%99%E3%82%8B)

- ボタン
  - [M5Unified入門 その4 ボタン操作 | Lang-ship](https://lang-ship.com/blog/work/m5unified-4-button/#toc3)

- timer化した時のトラブル
  - timer割り込みするたびにSW_CPU_RESETがかかってしまう。
  - flash frequencyあたりが怪しそう？？→関係なし。
    - [ESP32-CAM rst:0xc (SW_CPU_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT) fault · Issue #3510 · espressif/arduino-esp32](https://github.com/espressif/arduino-esp32/issues/3510)
    - [Espressif 32 — PlatformIO v6.1 documentation](https://docs.platformio.org/en/latest/platforms/espressif32.html)
  - いろいろ試した結論
    - 割り込みする場合は排他処理を入れなければならない。以下のページを参考にセマフォを導入することで解決した。
    - [ESP32の高精度タイマー割り込みを調べる | Lang-ship](https://lang-ship.com/blog/work/esp32-timer/)


### IMU
- MPU6886
  - Core2 for AWSにはボトムUNITにMPU6886（6軸のIMU）が搭載されている。温度センサも付属している。
  - `M5.IMU.Init()`で初期化する→`M5.Unified.h`を使う場合は`cfg.internal_imu = true`とする。以降、`M5.Imu`大文字小文字に注意。
  - `M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ)`, `M5.IMU.getAccelData(&accX, &accY, &accZ)`, `M5.getAhrsData(&pitch, &roll, &yaw)`, `M5.IMU.getTempData(&temp)`
  - `M5Unified.h`の場合は`M5.Imu.getGyroData(&gyroX, &gyroY, &gyroZ)`, `M5.Imu.getAccelData(&accX, &accY, &accZ)`, `M5.Imu.getTempData(&temp)`となる。`M5.getAhrsData(&pitch, &roll, &yaw)`の代わりがない。
- [Mahony Orientation Filter — AHRS 0.3.1 documentation](https://ahrs.readthedocs.io/en/latest/filters/mahony.html)
- [M5StickCのIMU(AHRS)研究 その1 標準ライブラリ | Lang-ship](https://lang-ship.com/blog/work/m5stickc-imu-ahrs/)
- [IMU Mahony filter explanation. Mahony filter is proposed by Robert… | by Teddy Lai | Apr, 2024 | Medium](https://medium.com/@k66115704/imu-mahony-filter-explanation-1ae75bf033ab)

### PlatformIO
- [マイコン開発するならPlatformIOがめちゃくちゃいいぞ #C++ - Qiita](https://qiita.com/JotaroS/items/1930f156aab953194c9a)
- [VSCodeとPlatformIOでM5Stack Core2開発 #VSCode - Qiita](https://qiita.com/desertfox_i/items/a6ff7deaa0a0b3802bcd)
- [VScode+PlatformIOでポートを固定する | 株式会社マグノリア](https://magnolia-tech.jp/vscode-platformio-comport-set/)
