void enviaSMS(String telefone, String mensagem) {
  Serial2.print("AT+CMGS=\"" + telefone + "\"\n");
  Serial2.print(mensagem + "\n");
  Serial2.print((char)26); 
}
void leGSM()
{
  static String textoRec = "";
  static unsigned long delay1 = 0;
  static int count = 0;
  static unsigned char buffer[64];

  if (Serial2.available()) {

    while (Serial2.available()) {

      buffer[count++] = Serial2.read();
      if (count == 64)break;
    }

    textoRec += (char*)buffer;
    delay1   = millis();

    for (int i = 0; i < count; i++) {
      buffer[i] = NULL;
    }
    count = 0;
    
  }
  


  if ( ((millis() - delay1) > 100) && textoRec != "" ) {

    if (textoRec.indexOf("RING") != -1) {
      temChamada = true;
    }     

    

    if ( textoRec.substring(2, 7) == "+CMT:" ) {
      temSMS = true;
    }

    if (temSMS) {

      telefoneSMS = "";
      dataHoraSMS = "";
      mensagemSMS = "";

      byte linha = 0;
      byte aspas = 0;
      for (int nL = 1; nL < textoRec.length(); nL++) {

        if (textoRec.charAt(nL) == '"') {
          aspas++;
          continue;
        }

        if ( (linha == 1) && (aspas == 1) ) {
          telefoneSMS += textoRec.charAt(nL);
        }

        if ( (linha == 1) && (aspas == 5) ) {
          dataHoraSMS += textoRec.charAt(nL);
        }

        if ( linha == 2 ) {
          mensagemSMS += textoRec.charAt(nL);
        }

        if (textoRec.substring(nL - 1, nL + 1) == "\r\n") {
          linha++;
        }
      }
    } else {
      comandoGSM = textoRec;
    }

    textoRec = "";
  }
}
