
# Tutorial de Sistema de Irrigação Automatizado com Arduino

Este tutorial apresenta o desenvolvimento de um sistema de irrigação utilizando sensores ambientais e controle por relé, com base na plataforma Arduino.

## Componentes Necessários

- **Placa Arduino Uno (ou similar)**
- **Sensor de Umidade do Solo (analógico)**
- **Sensor de Temperatura (ex.: LM35)**
- **Relé de 5V**
- **Válvula Solenoide 12V ou 24V**
- **Botão Push-Button**
- **Resistor de 10kΩ (pull-down para botão)**
- **Jumpers e Protoboard**
- **Fonte de alimentação 9-12V para solenoide**

## Pinos Utilizados

| Componente              | Pino Arduino     |
|-------------------------|------------------|
| Relé (controle solenoide) | Digital 4        |
| Botão                   | Digital 8        |
| Sensor de Umidade       | Analógico A3     |
| Sensor de Temperatura   | Analógico A0     |

## Funcionalidades do Sistema

- Modo **Manual**: Acionamento do solenoide via botão físico.
- Modo **Automático**: Controle baseado em:
  - Temperatura < 35°C
  - Umidade do solo < 40%
- Leituras exibidas via **Monitor Serial**.

## Passo a Passo de Montagem

1. **Conecte o Relé ao pino digital 4**:
   - VCC e GND do módulo relé → alimentação 5V do Arduino.
   - IN → pino 4 do Arduino.

2. **Conecte o Botão ao pino digital 8**:
   - Use resistor de 10kΩ como pull-down entre botão e GND.

3. **Sensor de Umidade**:
   - Sinal → A3
   - Alimentação → 5V e GND

4. **Sensor de Temperatura (LM35)**:
   - Sinal → A0
   - Alimentação → 5V e GND

5. **Fonte de alimentação externa para solenoide**:
   - Conecte o relé entre a fonte e o solenoide conforme especificação.

## Lógica de Funcionamento

- O sistema inicia com o **modo manual ativado** (sem irrigação).
- Ao pressionar o botão, alterna-se para **modo automático**.
- No modo automático, se os critérios ambientais forem atendidos, a válvula é acionada por 10 segundos.
- Todas as leituras são mostradas via monitor serial.

## Upload do Código

1. Abra o Arduino IDE.
2. Selecione sua placa e porta (menu “Ferramentas”).
3. Copie o código para o editor.
4. Clique em **Verificar** e depois em **Carregar**.
5. Abra o Monitor Serial (9600 baud) para acompanhar o funcionamento.

## Observações

- Ajuste os valores-limite de temperatura e umidade conforme o tipo de cultura ou solo.
- O sistema é extensível: pode incluir sensores de luz, pluviômetro e conectividade WiFi para dados em nuvem.

## Créditos

Sistema inspirado em projetos de automação para agricultura urbana e permacultura.
Desenvolvido em atividades do projeto **Roçambola** da Casa de Cultura Tainã.
