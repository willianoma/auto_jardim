#include "DHT.h"
#define DHTPIN A1
#define DHTTYPE DHT11  
#include <MeetAndroid.h>
MeetAndroid meetAndroid;
DHT dht(DHTPIN, DHTTYPE);
 
//OUTRAS CONFIGURAÃ‡Ã•ES
    float umidadeSensorChuva = 100; //valor minimo para atrazar o ciclo
    long tempoCortesia = 1000; //Tempo entre trocas de solenoides  CRIAR METODO PARECIDO AO SETAR TEMPO DE ESPERA.
     
// CONFIGURAR PORTAS DO ARDUINO 

    int PsolenoidA = 4;
    int NsolenoidA = 5;
    int bomba = NULL;
    float sensorChuva;
   
//APENAS PARA TESTES 
long tempoEsperaGlobal = 43200;//12 horas

void setTempoEspera(long tempo){
tempoEsperaGlobal = tempo;
};

long getTempoEspera(){
return tempoEsperaGlobal;
}

  
void setup() {
  
 Serial.begin(9600);
 dht.begin();
 meetAndroid.registerFunction(receberTempoEspera, 'A');
// meetAndroid.registerFunction(receberTempoEspera, 'B');
 
 //ATIVAR PORTAS DO ARDUINO
  pinMode(PsolenoidA, OUTPUT);
  pinMode(NsolenoidA, OUTPUT);
  
}

void loop() { 
  meetAndroid.send("");

 Serial.println("");
 
      chuveu();
      iniciarCiclo();
      cicloemEspera(getTempoEspera());
  
//  meetAndroid.receive(); 
    
  Serial.println("");
  
} //Fim do loop


   void ativaSolenoid(){
       Serial.println("ATIVANDO SOLENOIDE!");
   digitalWrite(PsolenoidA, HIGH);
   digitalWrite(NsolenoidA, LOW);
   delay(100);
   digitalWrite(PsolenoidA, LOW);
   digitalWrite(NsolenoidA, LOW);
  }
  
  void desativaSolenoid(){
    Serial.println("DESATIVANDO SOLENOIDE!");
   digitalWrite(PsolenoidA, LOW);
   digitalWrite(NsolenoidA, HIGH);
   delay(100);
   digitalWrite(PsolenoidA, LOW);
   digitalWrite(NsolenoidA, LOW);
  }

 
 void chuveu(){
        
// Se chuver por 15~30 min adiciona 12h em tempo de espera 
// Se chuver por 60 min adiciona 24h em tempo de espera 
    
      long novoTempo = 0; 
      long tempoDeChuva = verificaTempoChuva();

  if (tempoDeChuva>9 && tempoDeChuva<1800){//chuva 15~30m
     novoTempo = 43200;//43200;//espera 12 horas
     Serial.print("Tempo extra com qtd de chuva 15ma30m: ");
     Serial.println(novoTempo);
     cicloemEspera(novoTempo); 
     }

  if (tempoDeChuva>1800){//chuva 30m~
     novoTempo = 86400;//espera 24 horas
     Serial.print("Tempo extra com qtd de chuva 30m~: ");
     Serial.println(novoTempo);
     cicloemEspera(novoTempo); 
     }
     
       meetAndroid.send("");
     
 }
  


long verificaTempoChuva(){
  
  sensorChuva = analogRead(A0);
  long tempoChuva = 0;
  
  if (sensorChuva<umidadeSensorChuva){ // NÃƒO CHUVEU     
     tempoChuva = 0;
     return tempoChuva;
   } 
    desativaSolenoid();
     Serial.print("Chuva Detectada, Iniciando contagem para acrescimo de tempo: ");  
 
  meetAndroid.send("");
  while (sensorChuva>umidadeSensorChuva){ //CHUVEU
//  meetAndroid.send("");
   
       sensorChuva = analogRead(A0);
     
       delay(1000);
       tempoChuva++;
       Serial.print(tempoChuva); 
       Serial.print(" "); 
      }
      
       Serial.print("Tempo de chuva: ");
       Serial.println(tempoChuva);
      return tempoChuva;
       
      
  }
        
void cicloemEspera(long tempo){ // Metodo responsÃ¡vel pelo tempo de espera quando o sistema nÃ£o estÃ¡ operante

     Serial.print("Tempo em espera ATUAL: "); 
      Serial.println(tempo);
      Serial.print("Contagem: "); 
      meetAndroid.send("");
       for (int i=0; i<=(tempo); i++){    
        Serial.print(i); 
         Serial.print(" "); 
//         meetAndroid.send("");
        delay(1000);
       }
        Serial.println("");
}

long verificaClima(){
  long tempo = 0;
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.println(" %");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.println(" *C ");

  Serial.print("limite Sensor de Chuva: ");
  Serial.println(umidadeSensorChuva);
  Serial.print("Leitura Sensor de Chuva: ");
  Serial.println(sensorChuva);
 
 if (t<20){tempo = 60;} //600
 if (t>=20 && t<=25){tempo = 600;} //1500
 if (t>=25 && t<=30){tempo = 900;} //2100
 if (t>=35 && t<=40){tempo = 1200;} //2700
 if (t>40){tempo = 1800;} //3600
 
  Serial.print("Tempo setado automaticamente de acordo com o clima: ");
  Serial.print(tempo);
  Serial.println(" Segundos");
   meetAndroid.send("");
 return tempo;  
}


void iniciarCiclo(){
  
  Serial.println("Inicio do ciclo!");
  meetAndroid.send("");
       ativaSolenoid();
  
     cronometro(verificaClima());
   
        desativaSolenoid();
 
    Serial.println("Fim do ciclo!");
      
      } 
 
void ativarPorta(long x){ //Metodo para ativar uma solenoid
     if (x!=NULL){
        digitalWrite(x, HIGH);
        Serial.print("Porta Ativada: ");
        Serial.println(x);   
     } 
}

void desativarPorta(long x){  //Metodo para desativar uma solenoide
     if (x!=NULL){
        digitalWrite(x, LOW);
        Serial.print("Porta Desativada: ");
        Serial.println(x);
      } 
}

void cronometro(long tempo){       
    Serial.print("Cronometro: ");
    for (long i=0; i<=(tempo); i++){   
     chuveu(); //AQUI TEM UM BUG, quanto dermina o ciclo de espera de chuva ele conta o resto do tempo que falta do ciclo de irrigaÃ§Ã£o anterior com tudo parado.
     delay(1000); 
        Serial.print(i); 
        Serial.print(" "); 
//         meetAndroid.send("");
    }
    Serial.println(""); 
}
         
//void receberTempoCiclo(byte flag, byte numOfValues){
//     int TempoCicloAndroid = meetAndroid.getint();
//     Serial.print("Tempo de Ciclo Android: ");
//     Serial.println(TempoCicloAndroid);  
//     tempoDoCiclo = TempoCicloAndroid;  
//     }
       

void receberTempoEspera(byte flag, byte numOfValues){
 //    int TempoEsperaAndroid = meetAndroid.getint();
    // Serial.print("Tempo de Espera Android: ");
     Serial.write(flag);  
    Serial.write(numOfValues);
    // setTempoDeEspera(TempoEsperaAndroid);    
    }
  
 
 
 
 //      int length = meetAndroid.stringLength();
//      char data[length];
//      meetAndroid.getString(data);
//      String TempoCicloAndroid = data;
 
 
