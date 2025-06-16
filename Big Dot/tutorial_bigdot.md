
# Tutorial de Monitoramento Ambiental com Big Dot

Este tutorial orienta o uso da placa Big Dot no monitoramento ambiental da Casa de Cultura Tainã.

## Componentes Necessários

- **Placa Big Dot**
- **Sensor BME280**: Temperatura, umidade e pressão atmosférica
- **Sensor de Umidade do Solo**: HD-38 ou equivalente
- **Tela OLED 128x64**: Interface visual
- **Módulo WiFi ESP-12F**
- **RTC PCF8523**
- **Protoboard e Jumpers**
- **Fonte de alimentação 5V**
- **Resistores de pull-up/pull-down (se necessário)**

## Funcionalidades

- Leitura e exibição em tempo real de:
  - Umidade do ar
  - Temperatura
  - Pressão atmosférica
  - Umidade do solo
  - Data e hora (RTC)
- Envio de dados via MQTT para um servidor remoto
- Visualização local em OLED
- Comunicação via WiFi com ESP-12F

## Bibliotecas Necessárias

- Wire.h
- RTClib.h
- U8g2lib.h
- WiFiEspAT.h
- PubSubClient.h
- SparkFunBME280.h
- wiring_private.h
- private_settings.h (WiFi e MQTT)

## Conexões Principais

| Componente           | Big Dot Pin     |
|----------------------|------------------|
| BME280 (I2C)         | SDA/SCL (Wire)   |
| Sensor de Solo       | A4               |
| Tela OLED (I2C)      | SDA/SCL (Wire)   |
| RTC PCF8523          | SDA/SCL (Wire)   |
| ESP-12F (Serial2)    | RX=15, TX=14     |
| LED Indicador        | LED_BUILTIN      |

## Passo a Passo

1. **Montagem do Circuito**:
   - Monte os sensores em uma protoboard e conecte à Big Dot.
   - Verifique as polaridades e o uso de resistores.

2. **Instalação das Bibliotecas**:
   - Use o Arduino IDE ou PlatformIO para instalar as bibliotecas listadas.

3. **Configuração do Código**:
   - Edite o arquivo `private_settings.h` com suas credenciais WiFi e MQTT.
   - Compile e carregue o código na Big Dot.

4. **Execução**:
   - Abra o monitor serial (115200 baud).
   - Acompanhe leituras e status da conexão WiFi/MQTT.
   - Verifique os dados sendo exibidos na tela OLED e enviados para o broker MQTT.

## Observações

- O sistema é ideal para oficinas de extensão e experimentação com sensores ambientais.
- O uso do Big Dot permite maior controle sobre consumo energético e personalização da placa.

## Créditos

Desenvolvido por Casa de Cultura Tainã em parceria com oficinas comunitárias.
Versão 1.0 — 2024 — Aniversário de 35 anos.
