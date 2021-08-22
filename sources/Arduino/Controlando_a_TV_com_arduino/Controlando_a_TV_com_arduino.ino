// Links úteis
/*
 * (1)Documentação oficial: https://github.com/Arduino-IRremote/Arduino-IRremote
 * (2)Botões do controle da TV LG em hexadecimal já mapeado: https://gist.github.com/francis2110/f04237b04c94218028e3    
 * 
 */
 
// Lembretes importantes
/* -> Versao da biblioteca: 2.0.1
 * -> A porta que liga o LED Infravermelho geralmente é o pino digital 3, mas pode variar 
 * conforme a placa que estiver usando, consulte a documentação oficial(1);
 * -> Este codigo está em versão inicial, por isso pode apresentar alguns bugs
 * -> Fique a vontade para contribuir com o repositorio!
 * 
 */

// Onde colocar os pinos(Arduino UNO)
/* IR: 3
 * RX: 4
 * TX: 5
 */

#include <IRremote.h>
#include <IRremoteInt.h>
#include <SoftwareSerial.h> 

SoftwareSerial bluethooth(5, 4); // RX | TX 

//------------ CONSTANTES -----------------//
#define CIMA 0
#define BAIXO 1
#define ESQUERDA 2
#define DIREITA 3
#define OK 4

String mensagem = "";
bool recebiMensagem = false;

 
//------------ ESTRUTURAS -----------------//
typedef struct{
  int x,y;
}position;


//Esta parte abaixo foi copiada do codigo de exemplo do emissor IR
/*
 * Responsavel pela parte de transformar o Hexadecimail em RAW code, formato no qual
 * é entendido pela biblioteca
 */
IRrecv irrecv(0);
decode_results results;


class IRsendDummy : 
public IRsend
{
public:
  // For testing, just log the marks/spaces
#define SENDLOG_LEN 128
  int sendlog[SENDLOG_LEN];
  int sendlogcnt;
  IRsendDummy() : 
  IRsend() {
  }
  void reset() {
    sendlogcnt = 0;
  }
  void mark(int time) {
    sendlog[sendlogcnt] = time;
    if (sendlogcnt < SENDLOG_LEN) sendlogcnt++;
  }
  void space(int time) {
    sendlog[sendlogcnt] = -time;
    if (sendlogcnt < SENDLOG_LEN) sendlogcnt++;
  }
  // Copies the dummy buf into the interrupt buf
  void useDummyBuf() {
    int last = SPACE;
    irparams.rcvstate = STATE_STOP;
    irparams.rawlen = 1; // Skip the gap
    for (int i = 0 ; i < sendlogcnt; i++) {
      if (sendlog[i] < 0) {
        if (last == MARK) {
          // New space
          irparams.rawbuf[irparams.rawlen++] = (-sendlog[i] - MARK_EXCESS) / USECPERTICK;
          last = SPACE;
        } 
        else {
          // More space
          irparams.rawbuf[irparams.rawlen - 1] += -sendlog[i] / USECPERTICK;
        }
      } 
      else if (sendlog[i] > 0) {
        if (last == SPACE) {
          // New mark
          irparams.rawbuf[irparams.rawlen++] = (sendlog[i] + MARK_EXCESS) / USECPERTICK;
          last = MARK;
        } 
        else {
          // More mark
          irparams.rawbuf[irparams.rawlen - 1] += sendlog[i] / USECPERTICK;
        }
      }
    }
    if (irparams.rawlen % 2) {
      irparams.rawlen--; // Remove trailing space
    }
  }
};

IRsendDummy IRemissor;

void testNEC(unsigned long val, int bits) {
  IRemissor.reset();
  IRemissor.sendNEC(val, bits);
}

//termina a parte que copie do exemplo

void digitar(String frase){
  char escolhida = '0';
  int posLetraAtual = 0;
  int x = 0,y = 0;
  position cordenadaAtual = {1,0}, cordenadaAnterior = {1,0};
  
  char teclado[7][6]
  { // Padrao do teclado da Netflix
    {' ',' ',' ','<','<','<'},
    {'a','b','c','d','e','f'},
    {'g','h','i','j','k','l'},
    {'m','n','o','p','q','r'},
    {'s','t','u','v','w','x'},
    {'y','z','1','2','3','4'},
    {'5','6','7','8','9','0'},
  };
  
  
  Serial.print("\n  |--------------------------------------------------------------------------\n");
  do
  {
    for(x = 0; x < 7; x++)
    {
      for(y = 0; y < 6; y++)
        {
          
          if(teclado[x][y] == frase[posLetraAtual])
          {
          
              cordenadaAtual.x = x;
              cordenadaAtual.y = y;
              break; 
          }
        }
    }
    
//----------- refatorar isso com mais calma ------------------------//

    if(frase[posLetraAtual] == ' '){
      cordenadaAtual.y = cordenadaAnterior.y;
    }
    
    if(cordenadaAtual.x > cordenadaAnterior.x)
    {
      for(x = cordenadaAnterior.x; x < cordenadaAtual.x; x++){
       Serial.print("Baixo | ");
       apertarTecla(BAIXO);
        
      }
    }
    else if(cordenadaAtual.x < cordenadaAnterior.x)
    {
      for(x = cordenadaAnterior.x; x > cordenadaAtual.x; x--){
        Serial.print("Cima | ");
        apertarTecla(CIMA);
      }
    }
    
      
//-------------------------------------------------------------------//
    if(cordenadaAtual.y > cordenadaAnterior.y)
    {
      for(y = cordenadaAnterior.y; y <= cordenadaAtual.y; y++){
        for(y = cordenadaAnterior.y; y < cordenadaAtual.y; y++){
         Serial.print("direita | ");
         apertarTecla(DIREITA);
        }
      }
    }
    else if(cordenadaAtual.y < cordenadaAnterior.y)
    {
      for(y = cordenadaAnterior.y; y > cordenadaAtual.y; y--){
        Serial.print("Esquerda | ");
        apertarTecla(ESQUERDA);
      }
    }
    Serial.println("OK");
    apertarTecla(OK);
    
//-------------------------------------------------------------------//
    cordenadaAnterior.x = cordenadaAtual.x;
    cordenadaAnterior.y = cordenadaAtual.y;
//-------------------------------------------------------------------//
    posLetraAtual++;

  }while(frase[posLetraAtual] != '\0');
  
}

void apertarTecla(int tecla){
  switch(tecla){
    case CIMA:
      testNEC(0x20DF02FD, 32);//cima
    break;
    case BAIXO:
      testNEC(0x20DF827D, 32);//baixo
    break;
    case ESQUERDA:
      testNEC(0x20DFE01F, 32);
    break;
    case DIREITA:
      testNEC(0x20DF609F, 32);//direita
    break;
    case OK:
      testNEC(0x20DF22DD, 32);//OK
    break;
  }
  delay(150);
}

void lerBluetoothSerial(){
  if (bluethooth.available()){
    
    String value = bluethooth.readStringUntil('#');
    
    if(value.substring(0, 1) == "T")
    {
      mensagem = value.substring(2, value.length());
      Serial.flush();
      value="";
      recebiMensagem = true;
    }
  }
}
void setup() {
  Serial.begin(9600);
  bluethooth.begin(9600); 
}

void loop() {

  lerBluetoothSerial();
  
  while(recebiMensagem)
  {
    digitar(mensagem);
    recebiMensagem = false;
  }
  
}
