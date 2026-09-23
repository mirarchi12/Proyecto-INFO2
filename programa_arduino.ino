#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#define Piezo0 36
#define Piezo1 39
#define Piezo2 34
#define Piezo3 35
#define Piezo4 32
#define Piezo5 33
#define Piezo6 25
#define Piezo7 26
#define bootMode 0
#define switchMenu 23
#define switchRecord 3
#define ledRecord 1
#define poteVol 12
#define SCL 22
#define SDA 21
#define TAM 20

int Piezo0Val;
int Piezo1Val;
int Piezo2Val;
int Piezo3Val;
int Piezo4Val;
int Piezo5Val;
int Piezo6Val;
int Piezo7Val;
int menuMode;
bool pressPrevio0_4;
bool pressPrevio1_5;
bool pressPrevio2_6;
bool pressPrevio3_7;
int menu;
int preMenu = -1;
bool pressMenu=false;
bool selectedMenu=false;
int timeSecuence;
bool replay = true;
unsigned long int start = 0;
int libSounds;
int opcionlib;
bool PiezoPrevio[8] = {0};
int recordMode;
int vol_BPM;
String Lectura[TAM];
String Escritura[TAM];
char cmd[2];

LiquidCrystal_PCF8574 lcd(0x27);

void setup() {
  // put your setup code here, to run once:
  Wire.begin(SDA, SCL);
  lcd.begin(168,64);
  lcd.setBacklight(1);
  Serial.begin(115200);
  pinMode(switchMenu, INPUT_PULLDOWN);
  pinMode(switchRecord, INPUT_PULLDOWN);
  pinMode(bootMode, INPUT_PULLDOWN);
  pinMode(ledRecord, OUTPUT);
}

void loop() {

//--------------------------------------------------------Protocolo DAM [NO TOCAR] --------------------------------------------------------------------//
  if(Serial.available()) {
    Serial.readBytes(cmd, 2);
    switch(cmd[0]) {
      case 'R': 
        for (int i=0; i<TAM; i++) {
          Serial.print(Lectura[i]);
          Serial.print("-");
      }
      Serial.print("\n");
      break;
      case 'E': {
        String sI = Serial.readStringUntil('-');
        int i = sI.toInt();
        Escritura[i] = Serial.readStringUntil('\n');
      break;
      }

      default:
      break;
      }
    }
  //--------------------------------------------------------Protocolo DAM [NO TOCAR] --------------------------------------------------------------------//

  menuMode = digitalRead(switchMenu);
  recordMode = digitalRead(switchRecord);
  Piezo0Val = analogRead(Piezo0);
  Piezo1Val = analogRead(Piezo1);
  Piezo2Val = analogRead(Piezo2);
  Piezo3Val = analogRead(Piezo3);
  Piezo4Val = analogRead(Piezo4);
  Piezo5Val = analogRead(Piezo5);
  Piezo6Val = analogRead(Piezo6);
  Piezo7Val = analogRead(Piezo7);
  vol_BPM = analogRead(poteVol);
  lcd.setCursor(0,0);
  int PiezoVal[8] = {Piezo0Val, Piezo1Val, Piezo2Val, Piezo3Val, Piezo4Val, Piezo5Val, Piezo6Val, Piezo7Val};
  
  if (menuMode) {
      for(int i=0; i<8; i++)
       if(PiezoVal[i] > 300){
         PiezoVal[i] = 1;
         Lectura[i] = PiezoVal[i]; }
       else {
         PiezoVal[i] = 0;
         Lectura[i] = PiezoVal[i]; 
         }

    if (!selectedMenu) {
      lcd.print("(1).Loop (2).Met");
      lcd.setCursor(0,1);
      lcd.print("(3).Lib (4).Game");

    for(int i=0; i<7; i++){  
        if (PiezoVal[i]){
         switch (i) {
          case 0: 
          case 4:
         selectedMenu=true;
          menu=0;
         break;
        
          case 1:
          case 5:
          selectedMenu=true;
          menu=1;
          break;

          case 2:
          case 6:
          selectedMenu=true;
          menu=2;
          break;

          case 3:
          case 7:
          menu=3;
          selectedMenu=true;
          break;

          default:
          break;
        }
       }
      }
    }
    else {
    if(menu != preMenu) {
      lcd.clear();
      switch (menu) {

      case 0: {
        lcd.setCursor(0,0);
        lcd.print(" Loop ");
        Lectura[9] = 1;
      }
      break;

     case 1: {
       lcd.setCursor(0,0);
       lcd.print("    Tempo(BPM)    ");
       lcd.setCursor(6,1);
       int BPM = (vol_BPM); //Calcular Relacion vol_BPM->BPM
       lcd.print(BPM);
      }
      break;

      case 2: {
       lcd.setCursor(0,0);
       lcd.print("Seleccione su libreria");
       lcd.setCursor(0,1);
       lcd.print("Librerias disponibles");
       libSounds = Escritura[1].toInt(); //Revisar entrada de cantidad de librerias enviadas por la pc.
       for (int i=0; i<libSounds; i++){
       lcd.setCursor((2*i),3);
       }
       for(int i=0; i<8; i++) {

        if(PiezoVal[i] && !PiezoPrevio[i])
        opcionlib = i;
       PiezoPrevio[i] = PiezoVal[i];
       }
       Lectura[10] = opcionlib;
     }
      break;

     case 3: {
        
        if(replay) {
        lcd.setCursor(0,0);
        lcd.print("¡Escucha la secuencia!");
        Lectura[11] = 1;
        timeSecuence = Escritura[2].toInt();
        delay((timeSecuence + 1000));
    
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Preparate para reproducir la secuencia");
        lcd.setCursor(0,1);
        for(int i=0; i<3; i++){
          lcd.print((i+1));
          delay(500);
        }
        delay(500);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Reproduce la secuencia");

        replay = false;
        start = millis();
        }

        if ((millis() - start) > (timeSecuence+1000))
        Lectura[12] = '1';
        else {
        Lectura[12]= '0' ;
        }
        if(Lectura[12].toInt() == 0){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Se acabo el tiempo");
        lcd.setCursor(0,1);
        lcd.print("Su puntuacion es:");
        lcd.setCursor(0,2);
        lcd.print(Lectura[13]);
        lcd.setCursor(0,5);
        lcd.print("Toque cualquier boton para salir");
        }
        for(int i=0; i<7; i++) {
          if (PiezoVal[i]) {
            break;
          }
        }
      }
      default:
      break;
      menu = preMenu;
      }
     }
    }
   }
   else {
      for (int i=0; i<8; i++) {
        Lectura[i] = PiezoVal[i]; 
      }

    if (recordMode) {
      Lectura[8] = recordMode;
      digitalWrite(ledRecord, HIGH);
    } else { 
      Lectura[8] = recordMode;
    }
  }
}




