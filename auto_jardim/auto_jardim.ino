#include <LiquidCrystal.h>
#include <Servo.h>
 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Servo motor;

int tempoDeEspera = 15;//segundos
int tempoDoCiclo = 25;//Segundos

void setup() {
  lcd.begin(16, 2);
  motor.attach(10);
  Serial.begin(9600);
  motor.write(0);
}

void loop() {
  
  cicloemEspera(tempoDeEspera*1000); 
  iniciarCiclo(tempoDoCiclo*1000); 


  

} //Fim do loop


void iniciarCiclo(int tempo){
      lcd.clear();
      Alerta("Iniciar ciclo");
      lcd.clear();
      motor.write(0);
      tempo=tempo/1000;
      lcd.setCursor(0,0);
      lcd.print("Tempo p/ Fim");
      for (int i=0; i<=(tempo); i++){
        lcd.setCursor(0,1);
        lcd.print((tempo)-i); 
        delay(1000); 
      }  
}

void cicloemEspera(int tempo){ 
      lcd.clear();
      Alerta("Terminar ciclo");
      lcd.clear();
      motor.write(35);
      tempo=tempo/1000;         
      lcd.setCursor(0,0);
      lcd.print("Tempo p/ Inicio");
      for (int i=0; i<=(tempo); i++){
      lcd.setCursor(0,1);
      lcd.print((tempo)-i); 
      delay(1000);
    }
}



  void transicao(){  
   for (int i=0; i<=16; i++){
        for (int j=0; j<=1;j++){
        lcd.setCursor(i,j);
        lcd.print("*"); 
        delay(75);
        }
 delay(5);
    }
    lcd.clear();
   
  } //fim transicao
  
   void Alerta(String msg){
     
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(msg); 
      delay(2000);
   }
  
  
  
  void msgAlerta(String msg){
    int tamanho = msg.length();
    String texto = "";
    int spaco = (16-tamanho)/2;
    
    for (int i=0; i<spaco;i++){
      texto += " ";
      String valor =  texto+=msg;   
    Serial.println(valor);
    efeitoDuasLinhas(valor);
    delay(200);
    }
   } //fim msgAlenta
 
  void efeitoDuasLinhas(String valor){
  for (int i=0; i<=2; i++){
        for (int j=0; j<=1;j++){
          lcd.setCursor(0,j);
          lcd.print(valor); 
        delay(300);
        lcd.clear(); 
        }
     }
  } //fim efeitoDuasLinhas
