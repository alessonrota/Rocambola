#include <Wire.h> 
#include <RTClib.h> 
#include <U8g2lib.h> 
#include <WiFiEspAT.h> 
#include <PubSubClient.h> 
#include <SparkFunBME280.h> 
#include "wiring_private.h" 
#include "private_settings.h" 

#define PIN_SERIAL2_RX   (15ul)  
#define PIN_SERIAL2_TX   (14ul)  
#define PAD_SERIAL2_TX   (UART_TX_PAD_2) 
#define PAD_SERIAL2_RX   (SERCOM_RX_PAD_3) 

#define MQTT_KEEPALIVE   15000
#define SENSOR2_TOPIC1   "/tc/sensor_2/umidade"      
#define SENSOR2_TOPIC2   "/tc/sensor_2/temperatura"  
#define SENSOR2_TOPIC3   "/tc/sensor_2/pressao"      
#define SENSOR3_TOPIC1   "/tc/sensor_3/umidade_solo" 

BME280 bme280;
float humidity = 0, pressure = 0, temp = 0;
String temp_var;

const int soil_sensor = A4; 
int soil_read = 0;

RTC_PCF8523 rtc;

int status = WL_IDLE_STATUS;
WiFiClient ESPclient;
PubSubClient client(ESPclient);

U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, D8, D7, U8X8_PIN_NONE);
Uart Serial2(&sercom2, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX, PAD_SERIAL2_TX);

void setup()
{
  Serial.begin(115200);
  Serial.println("[BigDot] Saida serial USB ativada.");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Wire.begin();
  display.begin();
  display.clearBuffer();
  display.setFont(u8g_font_profont11);
  display.setCursor(2,10); display.print("Monitor Ambiental, v1");
  display.setCursor(2,50); display.print("CASA DE CULTURA TAINã");
  display.sendBuffer();
  Serial.println("[OLED] Tela inicial configurada.");

  bme280.setI2CAddress(0x77); 
  bme280.beginI2C();
  Serial.println("[BME280] Sensor de temperatura, umidade e pressão inicializado.");

  if (!rtc.begin()) {
    Serial.println("[RTC] Relógio não encontrado.");
  }

  pinPeripheral(15, PIO_SERCOM); 
  pinPeripheral(16, PIO_SERCOM);
  Serial2.begin(115200);         
  WiFi.init(&Serial2);           

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("[ESP-12F] Modulo WiFi não encontrado.");
  } else {
    wifi_connect();              
    client.setServer(MQTT_SERVER, MQTT_PORT); 
    client.setKeepAlive(MQTT_KEEPALIVE);
    mqtt_connect();              
  }
}

void loop() 
{
  Serial.println("");
  Serial.print("[Big Dot] Identificação da mucua: ");
  Serial.println(MQTT_CLIENT_ID); 
  Serial.println("[Big Dot] Iniciando o ciclo de leitura e envio de dados.");

  wifi_connect();

  if (!client.connected()) {
    mqtt_reconnect();
  } else {
    client.loop(); 
  }

  display.clearBuffer();

  if (rtc.begin()) {
    DateTime now = rtc.now(); 
    display.setCursor(1, 10);
    display.print(" ");
    display.print(now.day());
    display.print("/");
    display.print(now.month());
    display.print("/");
    display.print(now.year());
    display.print(" ");
    display.print(now.hour());
    display.print(":");
    display.print(now.minute());
  }

  humidity = bme280.readFloatHumidity();
  Serial.print("[BME280] Umidade (%): ");
  Serial.println(humidity);
  display.setCursor(8, 20); display.print("Umidade (%): ");
  display.setCursor(86, 20); display.print(humidity, 0);
  temp_var = String(humidity);
  client.publish(SENSOR2_TOPIC1, temp_var.c_str());
  delay(1000);

  temp = bme280.readTempC();
  Serial.print("[BME280] Temperatura (C): ");
  Serial.println(temp);
  display.setCursor(8, 30); display.print("Temp. (C): ");
  display.setCursor(86, 30); display.print(temp, 2);
  temp_var = String(temp, 2);
  client.publish(SENSOR2_TOPIC2, temp_var.c_str());
  delay(1000);

  pressure = (bme280.readFloatPressure() / 100.0F);
  Serial.print("[BME280] Pressão (hPa): ");
  Serial.println(pressure);
  display.setCursor(8, 40); display.print("Press.(hPa): ");
  display.setCursor(86, 40); display.print(pressure, 2);
  temp_var = String(pressure, 2);
  client.publish(SENSOR2_TOPIC3, temp_var.c_str());
  delay(1000);

  Serial.println("[HD-38] Lendo sensor de umidade do solo.");
  soil_read = analogRead(soil_sensor);
  Serial.print("[HD-38] Valor de leitura: ");
  Serial.println(soil_read);
  temp_var = String(soil_read);
  client.publish(SENSOR3_TOPIC1, temp_var.c_str());
  delay(1000);
}

void SERCOM2_Handler()
{
  Serial2.IrqHandler();
}

void wifi_connect()
{
  if (status != WL_CONNECTED) {
    Serial.print("[ESP-12F] Tentando conectar ao SSID: ");
    Serial.println(WLAN_SSID);
    status = WiFi.begin(WLAN_SSID, WLAN_PASS);
  } else {
    Serial.println("[ESP-12F] Conexão WiFi ativa.");
    Serial.print("[ESP-12F] Força do signal WiFi (RSSI): ");
    Serial.println(WiFi.RSSI());
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void mqtt_connect()
{
  Serial.print("[MQTT] Conectando ao broker MQTT: ");
  Serial.println(MQTT_SERVER);
  if (client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWD)) {
    Serial.println("[MQTT] Conectado ao servidor MQTT!");
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    Serial.print("[MQTT] Conexão falhou, código de status = ");
    Serial.println(client.state());
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void mqtt_reconnect()
{
  for (int i = 1; i <= 4; i++) {
    Serial.print("[MQTT] Tentando conexão... tentativa ");
    Serial.println(i);
    if (client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWD)) {
      Serial.println("[MQTT] Conectado ao servidor MQTT!");
      digitalWrite(LED_BUILTIN, HIGH);
      break;
    } else {
      Serial.println("[MQTT] Conexão falhou, tentando novamente.");
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}
