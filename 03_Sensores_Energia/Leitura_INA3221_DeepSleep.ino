#include <Wire.h>
#include <Adafruit_INA3221.h>

// Instancia o sensor usando a classe da Adafruit
Adafruit_INA3221 ina3221;

// --- CONFIGURAÇÃO DO DEEP SLEEP ---
#define uS_TO_S_FACTOR 1000000ULL  // Fator de conversão para microssegundos
#define TIME_TO_SLEEP  300         // Tempo de sono: 300 segundos (5 minutos)

//Utilização da mémoria RTC (teste) para armazenar os valores no modo Deep Sleep
RTC_DATA_ATTR float bufferCorrente[12];
RTC_DATA_ATTR float bufferTensaoPainel[12];
RTC_DATA_ATTR int indiceAmostra = 0;

void setup() {
  Serial.begin(115200);
  // Pequeno atraso para garantir que a porta serial abriu a tempo
  while (!Serial) { delay(10); } 
  
  Wire.begin();
  
  // Inicialização dos ensor INA3221 utilizando o padrão da biblioteca Adafruit
  if (!ina3221.begin()) {
    Serial.println("Erro: INA3221 não encontrado! Verifique as conexões.");
    while (1); // Laço de segurança para travar caso tenha falha no hardware
  }
  
  // Definição da média para 1024 amostras (testar com outros valores)
  ina3221.setAveragingMode(INA3221_AVG_1024_SAMPLES);

  // Rotina de leitura que ée xecutada a cada 5 minutos (tempo ainda não definido)
float tensaoPainel = ina3221.getBusVoltage(0); 
  
  float correntePainel = ina3221.getCurrentAmps(0) * 1000.0;

  // Laço para guardar na memória RTC
  if (indiceAmostra < 12) {
    bufferCorrente[indiceAmostra] = correntePainel;
    bufferTensaoPainel[indiceAmostra] = tensaoPainel;
    
    Serial.print("Amostra");
    Serial.print(indiceAmostra + 1);
    Serial.println(" salva na memória RTC.");
    
    indiceAmostra++; // Avança para o próximo ciclo de 5 minutos
  }

  // ========================================================
  // Início do ciclo de 1h
  // ========================================================
  if (indiceAmostra >= 12) {
    Serial.println("Ciclo de 1 Hora atingido. Iniciando envio...");

    // 1. Lê a tensão instantânea da Bateria no Canal 2
float tensaoBateria = ina3221.getBusVoltage(1);

    // 2. Constrói o Payload compactado
    String payload = "VBAT:" + String(tensaoBateria, 2) + "|I:";
    for (int i = 0; i < 12; i++) {
      payload += String(bufferCorrente[i], 0); // Sem casas decimais para poupar bytes
      if (i < 11) payload += ","; 
    }

    Serial.println("Payload formatado: " + payload);
    
    //adicionar aqui o código do motor, LDRS e envio Lora

    // 3. Reseta o contador para zerar a memória para a próxima hora
    indiceAmostra = 0; 
  }

 // Entrando em Deep Sleep novamente
  Serial.println("Entrando em Deep Sleep por 5 minutos...");
  Serial.flush(); // Aguarda o fim da impressão no Monitor Serial
  
  // Ativa o timer e desliga a CPU
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void loop() {
  // O código nunca chega aqui em arquiteturas de Deep Sleep.
}
