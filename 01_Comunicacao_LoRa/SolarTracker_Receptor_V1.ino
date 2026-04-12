// Teste comunicação - Receptor LoRa SX1278

#include <SPI.h>
#include <LoRa.h>

// Definição dos pinos utilizados na esp32
#define SCK_PIN  5
#define MISO_PIN 19
#define MOSI_PIN 27
#define SS_PIN   18
#define RST_PIN  14
#define DIO0_PIN 26
#define LORA_FREQ 433E6 // Definiç~~ão da frequencia

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Iniciando Receptor SX1278");
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
  LoRa.setPins(SS_PIN, RST_PIN, DIO0_PIN);

  // Tenntativa de comunicacao com o SX1278
  if (!LoRa.begin(LORA_FREQ)) {
    Serial.println("Falha ao detectar o modulo Lora SX1278");
    while (1); 
  }

  Serial.println("LoRa SX1278 pronto para receber pacotes..p");
}

void loop() {
  // Verifica se chegou um novo pacote de dados
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    String payload = ""; //limpa a string payload
    
    // Lê a mensagem completa caractere por caractere
    while (LoRa.available()) {
      payload += (char)LoRa.read();
    }

    Serial.print("Mensagem recebida");
    Serial.println(payload);

    //Teste de lógica para separar as grandezas recebidas
    
    int pos1 = payload.indexOf('|');           // Localiza a 1ª barra
    int pos2 = payload.indexOf('|', pos1 + 1); // Localiza a 2ª barra
    int pos3 = payload.indexOf('|', pos2 + 1); // Localiza a 3ª barra

    //Validação para nenhuma posição estar vaziaa
    if (pos1 != -1 && pos2 != -1 && pos3 != -1) {
      
      // Extração das Substrings (O "Corte")
      String sTensao   = payload.substring(0, pos1);
      String sCorrente = payload.substring(pos1 + 1, pos2);
      String sLdrEsq   = payload.substring(pos2 + 1, pos3);
      String sLdrDir   = payload.substring(pos3 + 1);

      // Conversão de Texto de string para float e int
      float tensao   = sTensao.toFloat();
      float corrente = sCorrente.toFloat();
      int ldrEsq     = sLdrEsq.toInt();
      int ldrDir     = sLdrDir.toInt();

      Serial.print("Tensão: ");
      Serial.print(tensao); 
      Serial.println("V");
      Serial.print("Corrente");
      Serial.print(corrente); 
      Serial.println("A");
      
      Serial.print("SENSORES: ");
      Serial.print("LDR Esq: "); 
      Serial.print(ldrEsq);
      Serial.print(" | LDR Dir: "); 
      Serial.println(ldrDir);
      
      Serial.print("SINAL: ");
      Serial.print(LoRa.packetRssi()); Serial.println(" dBm");
    } else {
      Serial.println("Pacote recebido com formato inválido");
    }
  }
}