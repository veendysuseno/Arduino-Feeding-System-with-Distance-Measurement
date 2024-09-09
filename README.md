# Arduino Feeding System with Distance Measurement

This Arduino project utilizes an HC-SR04 ultrasonic sensor, a DS3231 RTC, a servo motor, an LCD, and a buzzer to create an automated feeding system. The system measures the distance to determine the amount of feed remaining and performs feeding actions at scheduled times.

## Components Required

- Arduino board (e.g., Arduino Uno)
- HC-SR04 Ultrasonic Sensor
- DS3231 RTC Module
- 16x2 LCD with I2C Interface
- Servo Motor
- Buzzer
- Jumper Wires

## Libraries Used:

- LiquidCrystal_I2C: To control the LCD via I2C.
- RTClib: To interface with the DS3231 real-time clock (RTC).
- Servo: To control a servo motor.
- SPI and Wire: For I2C communication.

## Wiring

- **HC-SR04 Sensor**:

  - **Trig** to Arduino pin D7
  - **Echo** to Arduino pin D6
  - **VCC** to Arduino 5V
  - **GND** to Arduino GND

- **DS3231 RTC**:

  - **SDA** to Arduino A4 (or appropriate SDA pin)
  - **SCL** to Arduino A5 (or appropriate SCL pin)
  - **VCC** to Arduino 5V
  - **GND** to Arduino GND

- **LCD**:

  - **SDA** to Arduino A4 (or appropriate SDA pin)
  - **SCL** to Arduino A5 (or appropriate SCL pin)
  - **VCC** to Arduino 5V
  - **GND** to Arduino GND

- **Servo Motor**:

  - **Control** to Arduino pin D8
  - **VCC** to Arduino 5V
  - **GND** to Arduino GND

- **Buzzer**:
  - **Positive** to Arduino pin D0
  - **Negative** to Arduino GND

## Key Points to Verify

1. Initialization and Setup:

   - The LCD, RTC, and Servo are correctly initialized.
   - Ensure the RTC.adjust(DateTime(**DATE**, **TIME**)); line is uncommented if you need to set the RTC to the compile time. Otherwise, the RTC should already be set.

2. Distance Measurement:

   - The baca_sensor function uses the HC-SR04 ultrasonic sensor to measure distance. The calculation of distance as distance = duration \* 0.034 / 2; seems correct.

3. Data Display:

   - The get_data function updates the LCD with the current date, time, and distance measurement. It also triggers the servo and buzzer based on conditions.

4. Scheduled Feeding:
   - The code includes checks for specific times to activate the feeding mechanism. Ensure the RTC time is set correctly.

## Code

```cpp
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 RTC;
Servo myservo;

const int Buzer = D0;
const byte Echo = D6;
const byte Trig = D7;

long duration;
int distance, jarak, tinggipakan;
int max_tinggi = 17; // Dalam satuan CM
String line;
unsigned long old_time;

void setup() {
  Serial.begin(115200);
  lcd.begin();
  Wire.begin();
  RTC.begin();
  myservo.attach(D8);
  myservo.write(0);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  pinMode(Buzer, OUTPUT);
}

void loop() {
  if (millis() - old_time >= 1000) {
    get_data();
    old_time = millis();
  }
}

void baca_sensor(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
}

void get_data() {
  DateTime now = RTC.now();
  baca_sensor(Trig, Echo);
  jarak = distance;
  tinggipakan = (max_tinggi - jarak);
  line = String("T:") + tinggipakan;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(now.day(), DEC);
  lcd.print("/");
  lcd.print(now.month(), DEC);
  lcd.print("/");
  lcd.print(now.year(), DEC);
  lcd.setCursor(11, 0);
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);
  lcd.print(":");
  lcd.print(now.second(), DEC);
  lcd.setCursor(0, 1);
  lcd.print(line);
  Serial.println(line);

  if (tinggipakan <= 3) {
    lcd.setCursor(4, 1);
    lcd.print("cm,Habis");
    bunyi();
  } else {
    lcd.setCursor(4, 1);
    lcd.print("cm,Tersisa");
  }

  if (now.hour() == 13 && now.minute() == 40 && now.second() <= 05) {
    kasih_pakan(3);
    Serial.println("Makan ke-1");
  } else if (now.hour() == 13 && now.minute() == 41 && now.second() <= 05) {
    kasih_pakan(3);
    Serial.println("Makan ke-2");
  } else if (now.hour() == 13 && now.minute() == 42 && now.second() <= 05) {
    kasih_pakan(3);
    Serial.println("Makan ke-3");
  }
  else {
    myservo.write(0);
  }
}

void bunyi() {
  digitalWrite(Buzer, HIGH);
  delay(1000);
  digitalWrite(Buzer, LOW);
}
```

## Usage

1. Connect the components to the Arduino as per the wiring instructions.
2. Upload the code to your Arduino board.
3. Monitor the LCD and Serial Monitor for updates on feed levels and feeding actions.

## License

- This project is licensed under the MIT License - see the LICENSE file for details.
