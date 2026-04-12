# ☀️ Projeto: Rastreador Solar Automatizado com comunicação LoRa

Este projeto consiste no desenvolvimento de um sistema de rastreamento solar (Solar Tracker) focado em eficiência energética. O sistema utiliza um ESP32 para processar dados de posicionamento e sensores de potência, transmitindo as informações via rádio LoRa para uma base de monitoramento.

## 📡 Comunicação e Protocolo (LoRa)
Nesta etapa inicial, estabelecemos a comunicação entre o **Nó Emissor (Tracker)** e o **Nó Receptor (Base)**.
* **Transmissão:** Empacotamento de dados (Tensão, Corrente, LDRs) em Strings com delimitadores.
* **Recepção:** Lógica de *parsing* para fatiamento de dados e conversão de tipos.
* **Hardware:** Módulos SX1278 operando em 433MHz.

## 📂 Organização do Firmware
* `01_Comunicacao_LoRa`: Códigos de validação de TX (Transmissão) e RX (Recepção).
* `02_Sensores_LDR`: Lógica de comparação de luminosidade para orientação dos motores.
* `03_Sensores_Energia`: Monitoramento de performance via sensor INA3221.
* `04_Motores`: Algoritmos de controle para movimentação do painel.

## 🛠️ Especificações Técnicas
* **Microcontrolador:** ESP32 DevKit V1
* **Sensores de Luz:** LDRs (Resistores Dependentes de Luz)
* **Sensores de Energia:** Monitor de corrente/tensão I2C
* **Rádio:** Tecnologia LoRa para telemetria de longo alcance

## 📋 Status do Cronograma
- [x] Arquitetura de comunicação LoRa concluída.
- [x] Lógica de fatiamento de dados validada (pendente teste com os microcontrolaodres).
- [ ] Integração física dos sensores e motores (Em andamento).
- [ ] Testes de campo e medição de sinal (RSSI).
