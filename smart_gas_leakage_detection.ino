
#define BLYNK_TEMPLATE_ID "TPLiP35VFG8cz"
#define BLYNK_TEMPLATE_NAME "Gas leakage notification"
#define BLYNK_AUTH_TOKEN "Your_Blynk_Auth_Token"

#define BLYNK_PRINT Serial
#include <WiFiClient.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// ----- WiFi Credentials -----
char auth[] = "Your_Blynk_Auth_Token";
char ssid[] = "Your_WiFi_SSID";
char pass[] = "Your_WiFi_Password";

// ----- Hardware Pins -----
Servo valveServo;
const int gasSensorPin = 34; // MQ-2 analog pin

const int buzzerPin  = 4;   // Active/Passive buzzer pin
const int motorPin   = 19;  // Fan or motor pin
const int relayPin   = 21;  // Relay control pin
const int ledPin     = 2;   // LED indicator pin
const int servoPin   = 18;  // Servo control pin

// ----- ADC Thresholds -----
const int THRESHOLD_1 = 1024;
const int THRESHOLD_2 = 1941;
const int THRESHOLD_3 = 2666;
const int THRESHOLD_4 = 3078;
const int THRESHOLD_5 = 4088;

void setup() {
  Serial.begin(115200);

  pinMode(buzzerPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  valveServo.attach(servoPin);
  valveServo.write(0);

  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();

  int rawGasLevel = analogRead(gasSensorPin);
  float voltage = (rawGasLevel / 4095.0) * 3.3;
  Blynk.virtualWrite(V1, rawGasLevel);

  Serial.print("Raw ADC: ");
  Serial.print(rawGasLevel);
  Serial.print("\tVoltage: ");
  Serial.println(voltage, 2);

  delay(500);

  if (rawGasLevel >= THRESHOLD_5) {
    tone(buzzerPin, 2000);
    digitalWrite(motorPin, HIGH);
    valveServo.write(90);
    digitalWrite(relayPin, LOW);
    digitalWrite(ledPin, HIGH);
    Blynk.logEvent("gas_alert", "High gas concentration detected!");
  }
  else if (rawGasLevel >= THRESHOLD_4) {
    tone(buzzerPin, 1500);
    digitalWrite(motorPin, HIGH);
    valveServo.write(90);
    digitalWrite(relayPin, HIGH);
    digitalWrite(ledPin, HIGH);
  }
  else if (rawGasLevel >= THRESHOLD_3) {
    tone(buzzerPin, 1000);
    digitalWrite(motorPin, HIGH);
    valveServo.write(90);
    digitalWrite(relayPin, HIGH);
    digitalWrite(ledPin, HIGH);
  }
  else if (rawGasLevel >= THRESHOLD_2) {
    tone(buzzerPin, 800);
    digitalWrite(motorPin, HIGH);
    valveServo.write(90);
    digitalWrite(relayPin, HIGH);
    digitalWrite(ledPin, LOW);
  }
  else if (rawGasLevel >= THRESHOLD_1) {
    noTone(buzzerPin);
    digitalWrite(motorPin, LOW);
    valveServo.write(90);
    digitalWrite(relayPin, HIGH);
    digitalWrite(ledPin, LOW);
  }
  else {
    noTone(buzzerPin);
    digitalWrite(motorPin, LOW);
    valveServo.write(0);
    digitalWrite(relayPin, HIGH);
    digitalWrite(ledPin, LOW);
  }

  delay(2000);
}
