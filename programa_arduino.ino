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
int preMenu=-1;
bool selectedMenu=true;
bool enterMenu0= true;
bool enterMenu1= true;
bool enterMenu2= true;
bool enterMenu3= true;
                
int timeSecuence;
int libSounds;
int opcionlib;
unsigned long start = 0;
bool replay = true;

bool PiezoPrevio[8] = {0};

int recordMode;
int vol_BPM;
bool show =true;
String Lectura[TAM];
String Escritura[TAM];

char cmd[2];

LiquidCrystal_PCF8574 lcd(0x27);

void setup() {

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
    //----------------------------------------------------- Protocolo DAM [NO TOCAR] --------------------------------------------------------//

    if (Serial.available()) {
        Serial.readBytes(cmd, 2);
        switch (cmd[0]) {
          
            case 'R':
                for (int i = 0; i < TAM; i++) {
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

    //-------------------------------------------------------- Protocolo DAM [NO TOCAR] --------------------------------------------------------//

    menuMode = 1;

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


    int PiezoVal[8] = {
        Piezo0Val,
        Piezo1Val,
        Piezo2Val,
        Piezo3Val,
        Piezo4Val,
        Piezo5Val,
        Piezo6Val,
        Piezo7Val
    };

    if (menuMode) {

        for (int i = 0; i < 8; i++) {

            if (PiezoVal[i] > 300) {

                PiezoVal[i] = 1;
                Lectura[i] = PiezoVal[i];
              
            } else {
              
                PiezoVal[i] = 0;
                Lectura[i] = PiezoVal[i];
            }
        }

  if (selectedMenu){
        lcd.setCursor(0, 0);
        lcd.print("(1)Loop (2)Met ");

        lcd.setCursor(0, 1);
        lcd.print("(3)Lib  (4)Game");

  for(int i=0; i<7; i++){  
      if (PiezoVal[i]){
        switch (i) {
        case 0: 
        selectedMenu=false;
        menu=0;
        break;

        case 4:
        selectedMenu=false;
        menu=0;
        break;
        
        case 1:
        selectedMenu=false;
        menu=1;
        break;

        case 5:
        selectedMenu=false;
        menu=1;
        break;

        case 2:
        selectedMenu=false;
        menu=2;
        break;

        case 6:
        selectedMenu=false;
        menu=2;
        break;

        case 3:
        menu=3;
        replay=true;
        selectedMenu=false;
        break;

        case 7:
        replay=true;
        menu=3;
        selectedMenu=false;
        break;

        default:
        break;
      }
    }
  }
}
else {
  
  for(int i=0; i<7; i++) {
    if(PiezoVal[i]){
    selectedMenu = 1;
    break;
    }
  }
}

 if (menu != -1) {

        switch (menu) {
            case 0: {
              if (enterMenu0) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Loop");
              enterMenu0 = false;
              }
              
                Lectura[9] = 1;
            }
            break;

            case 1: {
                if (enterMenu1) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Tempo (BPM)");
                enterMenu1 = false;
                }
              //corregir esto
                lcd.setCursor(0, 1);
                long int BPM = map(vol_BPM, 0, 4095, 0, 300);
              
                lcd.print(BPM);
            }
            break;

            case 2: {
              if(enterMenu2) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Seleccione");

                lcd.setCursor(0, 1);
                lcd.print("una libreria");
                enterMenu2= false;
                }
                
              libSounds = Escritura[1].toInt();

                for (int i = 0; i < 8; i++) {
                    if (PiezoVal[i] && !PiezoPrevio[i]) {
                        opcionlib = i;
                    }
                    PiezoPrevio[i] = PiezoVal[i];
                }
                Lectura[10] = opcionlib;
            }
            break;

            case 3: {
                if (replay) {
                  if (enterMenu3) {
                    lcd.clear();

                    lcd.setCursor(0, 0);
                    lcd.print("Escucha la");

                    lcd.setCursor(0, 1);
                    lcd.print("secuencia!");
                    enterMenu3 = false;
                  }
                    Lectura[11] = 1;

                    timeSecuence = Escritura[2].toInt();

                    start = millis();

                    replay = false;
                }

                if (millis() - start < (unsigned long)(timeSecuence + 1000)) {

                    Lectura[12] = '1';
                } else {
                    Lectura[12] = '0';
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Tiempo terminado");

                    lcd.setCursor(0, 1);
                    lcd.print("Punt:");

                    lcd.print(Lectura[13]);       
                  for (int i = 0; i < 7; i++) {
                    if (PiezoVal[i]) {
                      break;
                    }
                  }
                }
              }
            default:
            menu = -1;
                break;
        }
      }
    }

    else {

        for (int i = 0; i < 8; i++) {
            Lectura[i] = PiezoVal[i];
        }

        if (recordMode) {
            Lectura[8] = recordMode;
            digitalWrite(ledRecord, HIGH);
        } else {
            Lectura[8] = recordMode;
            digitalWrite(ledRecord, LOW);
        }
    }
}
