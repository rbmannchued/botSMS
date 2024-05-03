#include "FS.h"
#include "SD.h"
#include "SPI.h"



bool temSMS = false;
bool temChamada = false;
String telefoneSMS;
String dataHoraSMS;
String mensagemSMS;
String comandoGSM = "";
String ultimoGSM = "";
String numeroChamada;
#define led  2
#define senhaGsm "1234"
//#define numeroCall "5542988673875" // numero p ligar

bool callStatus = false;
void leGSM();
void enviaSMS();


void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(led, OUTPUT);
  
 if(!SD.begin(5)){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

  Serial2.print("AT+CMGF=1\n;AT+CNMI=2,2,0,0,0\n;ATX4\n;AT+COLP=1\n;AT+CLIP=1\n");

}

void loop() {
  leGSM();
  if (comandoGSM != "") {
    Serial.println(comandoGSM);
    ultimoGSM = comandoGSM;
    comandoGSM = "";
  }
if (temChamada) {
    Serial.println("Chamada Recebida de: " + numeroChamada);
    Serial.println("Atendendo chamada...");
    writeFile(SD, "/info.txt", "chamada: " );
     writeFile(SD, "/info.txt",  "123" );
    // Atender chamada
    Serial2.println("ATA");
   digitalWrite(led, HIGH);
    delay(5000); // Manter a chamada ativa por 5 segundos

    // Desligar chamada
    Serial2.println("ATH");

    Serial.println("Chamada encerrada.");
   digitalWrite(led, LOW);
    temChamada = false;
  }

   
      
   

  if (temSMS) {

     Serial.println("Chegou Mensagem!!");
     Serial.println();
     digitalWrite(led,HIGH);
    
     Serial.print("Remetente: ");  
     Serial.println(telefoneSMS);
     Serial.println();
    
     Serial.print("Data/Hora: ");  
     Serial.println(dataHoraSMS);
     Serial.println();
    
     Serial.println("Mensagem:");  
     Serial.println(mensagemSMS);
     Serial.println();
      
     mensagemSMS.trim();      
     if ( mensagemSMS == senhaGsm ) {
        Serial.println("Enviando SMS de Resposta.");  
         enviaSMS(telefoneSMS, "eles estao di olho em vc da silva");
     }
     temSMS = false;
     delay(150);
     digitalWrite(led,LOW);

  }

}
