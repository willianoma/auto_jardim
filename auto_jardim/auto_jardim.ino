#include <LiquidCrystal.h>
#include <Servo.h>
 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Servo motor;

int intervaloInicio = 10;//segundos
int intervaloFim = 5;//segundos

void setup() {
  lcd.begin(16, 2);
  motor.attach(10);
  Serial.begin(9600);
}

void loop() {
  msgAlerta("Mudou de ciclo");
  //inicioLCD();
  tempoInicio(intervaloInicio*1000); 
  motor.write(180);
  msgAlerta("Mudou de ciclo");
  fimLCD();
  tempoFim(intervaloFim*1000); 
  motor.write(90);

  
} //Fim do loop
void inicioLCD(){
//lcd.clear();  
lcd.setCursor(0,0);
lcd.print("Tempo p/ Inicio");
}

void fimLCD(){
lcd.clear(); 
lcd.setCursor(0,0);
lcd.print("Tempo p/ Fim");
}


void tempoInicio(int tempo){
      tempo=tempo/1000;
      for (int i=0; i<=(tempo); i++){
      lcd.setCursor(0,1);
      lcd.print((tempo)-i); 
      inicioLCD();
      delay(1000);
      lcd.clear();  
  }
}

void tempoFim(int tempo){
      for (int i=0; i<=(tempo/1000); i++){
      lcd.setCursor(0,1);
      lcd.print((tempo/1000)-i); 
      delay(1000);
    }
}

void ligarAgua(){
  motor.write(90); 
}

void desligarAgua(){
  motor.write(180);
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
  
  
  void msgAlerta(String msg){
    lcd.clear();
    int tamanho = msg.length();
    String texto = "";
    int spaco = (16-tamanho)/2;
    
    for (int i=0; i<spaco;i++){
    texto += " ";
        
    efeitoDuasLinhas(texto+=msg);
    delay(200);
    lcd.clear();
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
  
