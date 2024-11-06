#include <Wire.h>
#include "TouchSensorClass.hpp"
#include <FastLED.h>
#include "VibrationMotorClass.hpp"

static constexpr int MOTOR_F_PIN = D3;
static constexpr int MOTOR_R_PIN = D2;
static constexpr int SLED_PIN = D8;
static constexpr int SLED_NUMBER = 5;

static constexpr int SENSOR_ADDRESS = 0x37;

static const unsigned char SENSOR_CONFIG[128] = {
  0xE3u, 0x01u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x80u, 0x80u, 0x7Fu, 0x7Fu,
  0x7Fu, 0x80u, 0x80u, 0x80u, 0x80u, 0x7Fu, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x03u, 0x00u, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x80u,
  0x05u, 0x00u, 0x00u, 0x02u, 0x00u, 0x02u, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Eu, 0x1Eu, 0x00u,
  0x00u, 0x1Eu, 0x1Eu, 0x00u, 0x00u, 0x00u, 0x01u, 0x01u,
  0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x03u, 0x01u, 0x58u,
  0x00u, 0x37u, 0x06u, 0x00u, 0x00u, 0x0Au, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x25u, 0xDFu
};

static TouchSensorClass TouchSensor{ SENSOR_ADDRESS };
static CRGB Leds[SLED_NUMBER];
static VibrationMotorClass VibrationMotor{ MOTOR_F_PIN, MOTOR_R_PIN };

static int LedPattern = -1;
static long LedPatternStartTime = 0;
static int VibrationPattern = -1;
static long VibrationPatternStartTime = 0;

static std::vector<String> split(const String& str, char separator) {
  std::vector<String> array;

  if (str.length() <= 0) return array;

  int beginIndex = 0;
  while (true) {
    const int endIndex = str.indexOf(separator, beginIndex);
    if (endIndex < 0) break;

    array.push_back(str.substring(beginIndex, endIndex));

    beginIndex = endIndex + 1;
  }
  array.push_back(str.substring(beginIndex));

  return array;
}

static void captureCommand(const std::vector<String>& args) {
  TouchSensor.capture();
  Serial.printf("%3u %3u %3u %3u\n", TouchSensor.getValue(0), TouchSensor.getValue(1), TouchSensor.getValue(2), TouchSensor.getValue(3));

  Serial.println("ok");
}

static void setLedCommand(const std::vector<String>& args) {
  if (args.size() < 2) return;
  LedPattern = args[1].toInt();
  LedPatternStartTime = millis();

  Serial.println("ok");
}

static void setAllLeds(const CRGB& color) {
  for (int i = 0; i < std::size(Leds); i++) Leds[i] = color;
  FastLED.show();
}

static void ledWork(void) {
  switch (LedPattern) {
    case -1:
      break;
    case 0:  // 消灯
      setAllLeds(0x000000);
      LedPattern = -1;
      break;
    case 1:  // 赤点灯
      setAllLeds(0x100000);
      LedPattern = -1;
      break;
    case 2:  // 青点滅
      {
        const auto elapsed = millis() - LedPatternStartTime;
        setAllLeds(elapsed % 2000 < 1000 ? 0x000010 : 0x000000);
      }
      break;
    case 3:  // 緑ワンショット
      {
        const auto elapsed = millis() - LedPatternStartTime;
        if (elapsed < 2000) {
          setAllLeds(0x001000);
        } else {
          setAllLeds(0x000000);
          LedPattern = -1;
        }
      }
      break;
    case 4:  // 青フワフワ
      {
        const auto elapsed = (millis() - LedPatternStartTime) % 5000;
        if (elapsed < 1000) {
          setAllLeds(elapsed * 0xff / 1000);
        } else if (elapsed < 1500) {
          setAllLeds(0xff);
        } else if (elapsed < 2500) {
          setAllLeds((3500 - elapsed) * 0xff / 1000);
        } else {
          setAllLeds(0);
        }
      }
      break;
  }
}

static void setVibrationCommand(const std::vector<String>& args) {
  if (args.size() < 2) return;
  VibrationPattern = args[1].toInt();
  VibrationPatternStartTime = millis();

  Serial.println("ok");
}

static void vibrationWork(void) {
  switch (VibrationPattern) {
    case -1:
      break;
    case 0:  // 停止
      VibrationMotor.turnOff();
      VibrationPattern = -1;
      break;
    case 1:  // 常時
      VibrationMotor.turnOn();
      VibrationPattern = -1;
      break;
    case 2:  // ワンショット
      {
        const auto elapsed = millis() - VibrationPatternStartTime;
        if (elapsed < 2000) {
          VibrationMotor.turnOn();
        } else {
          VibrationMotor.turnOff();
          VibrationPattern = -1;
        }
      }
      break;
  }
}

void setup(void) {
  Serial.begin(115200);
  Serial.setTimeout(10);
  Wire.begin();

  delay(1000);

  TouchSensor.begin(SENSOR_CONFIG);
  FastLED.addLeds<SK6812, SLED_PIN, GRB>(Leds, std::size(Leds));
  FastLED.show();
  VibrationMotor.begin();
}

void loop(void) {
  const auto commandLine = Serial.readStringUntil('\r');
  if (commandLine.length() >= 1) {
    const auto args = split(commandLine, ' ');
    if (args.size() >= 1) {
      if (args[0].compareTo("capture") == 0) {
        captureCommand(args);
      } else if (args[0].compareTo("setLed") == 0) {
        setLedCommand(args);
      } else if (args[0].compareTo("setVibration") == 0) {
        setVibrationCommand(args);
      }
    }
  }

  ledWork();

  vibrationWork();
}
