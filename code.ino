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
int max_tinggi = 17; //Dalam satuan CM
String line;
unsigned long old_time;

void setup() {
  Serial.begin(115200);
  lcd.begin();
  Wire.begin();
  RTC.begin();
  myservo.attach(D8);
  myservo.write(0);
  //  RTC.adjust(DateTime(__DATE__, __TIME__));
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  pinMode(Buzer, OUTPUT);
}

void loop() {
  if ( millis() - old_time >= 1000 ) {
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
  lcd.print("/");//  lcd.print("22 "); //
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
