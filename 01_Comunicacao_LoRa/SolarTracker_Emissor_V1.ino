// Teste de comunicação LoRa 1.0

#include <SPI.h>
#include <LoRa.h>
// Mapeamento dos pinos SPI
#define SCK_PIN 5
#define MISO_PIN  19
#define MOSI_PIN  27
#define SS_PIN    18
#define RST_PIN   14
#define DIO0_PIN  26
// Frequência do SX1278
#define LORA_FREQ 433E6

void setup() {

  Serial.begin(115200);
  while (!Serial);

  Serial.println("Inicializando Nó Emissor (Tracker)...");

  // Configuração do barramento SPI e pinos
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN); //Define os pinos utilizados
  LoRa.setPins(SS_PIN, RST_PIN, DIO0_PIN); //Define os pinos utilizados

  //Inicialização do rádio - teste de fhalhas no hardware
  if (!LoRa.begin(LORA_FREQ)) {
    Serial.println("ERRO: Falha ao iniciar o módulo SX1278!");
    while (1); 
  }

  LoRa.setTxPower(2); //Define a potência de trabalho do amplificador, máximo 20dB, utilizando 2 para não saturar o receptor pela distância. 
}

void loop() {
  
  float tensao_V = 19.5; 
  float corrente_A = 1.15; 
  int ldr_esq = 24500; 
  int ldr_dir = 24100;

  EnviarDadosLoRa(tensao_V, corrente_A, ldr_esq, ldr_dir); //chamando a função de envio e passando as variáveis atuais

  delay(3000);
}
//Empacota e transmite os dados via LoRa
void EnviarDadosLoRa(float tensao, float corrente, int ldrEsq, int ldrDir) {

  String pacoteDados = String(tensao) + "|" +
                       String(corrente) + "|" +
                       String(ldrEsq) + "|" +
                       String(ldrDir);
  
  Serial.print("Transmitindo pacote:");
  Serial.println(pacoteDados);

  //Envio dos dados de rádio
  LoRa.beginPacket();
  LoRa.print(pacoteDados);
  LoRa.endPacket();


}
