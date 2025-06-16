#include <Wire.h>

#define RELAY_PIN 4
#define BUTTON_PIN 8
#define HUMIDITY_SENSOR_PIN A3
#define TEMPERATURE_SENSOR_PIN A0

unsigned long irrigationStartMillis = 0;
const unsigned long irrigationDuration = 10000;
bool manualOverride = true;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(RELAY_PIN, LOW);
  Serial.begin(9600);
  Serial.println("Sistema de Irrigação Iniciado");
}

void loop() {
  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(BUTTON_PIN);

  if (currentButtonState == LOW && lastButtonState == HIGH) {
    manualOverride = !manualOverride;
    digitalWrite(RELAY_PIN, manualOverride ? HIGH : LOW);
    Serial.println(manualOverride ? "Modo Manual: ON" : "Modo Manual: OFF");
    delay(2000);
  }
  lastButtonState = currentButtonState;

  if (!manualOverride) {
    float humidityPercentage = convertToRelativeHumidity(analogRead(HUMIDITY_SENSOR_PIN));
    float temperature = readTemperature();

    if (humidityPercentage < 40 && temperature < 35.0) {
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println("Irrigação Automática: Ativada");
    } else {
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("Irrigação Automática: Desativada");
    }

    Serial.print("Umidade: ");
    Serial.print(humidityPercentage);
    Serial.println("%");
    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.println("C");

    delay(1000);
  }
}

float convertToRelativeHumidity(int sensorValue) {
  float humidityPercentage = map(sensorValue, 1023, 0, 0, 100);
  return humidityPercentage;
}

float readTemperature() {
  int reading = analogRead(TEMPERATURE_SENSOR_PIN);
  float voltage = reading * 0.5 / 1150.0;
  float temperature = voltage * 100.0;
  return temperature;
}
