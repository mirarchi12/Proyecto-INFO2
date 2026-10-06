#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define Piezo0 36
#define Piezo1 39
#define Piezo2 34
#define Piezo3 35
#define Piezo4 32
#define Piezo5 33
#define Piezo6 25
#define Piezo7 26
#define switchBPM 1
#define poteBPM 12
#define switchRecord 23
#define TAM 20

#define SCL 22
#define SDA 21

int Piezo0Val;
int Piezo1Val;
int Piezo2Val;
int Piezo3Val;
int Piezo4Val;
int Piezo5Val;
int Piezo6Val;
int Piezo7Val;
int setBPM;
int vol_BPM; 
//int recordMode;
String Lectura[TAM];
//String Escritura[TAM];

Adafruit_SSD1306 oled(128, 64, &Wire, 4);

void setup() {
  
  Wire.begin(SDA, SCL);
  oled.begin(SSD1306_SWITCHCAPVCC, 0x27);

  Serial.begin(115200);
  pinMode(switchBPM, INPUT_PULLUP);
  pinMode(switchRecord, INPUT);
}

void loop() {

//--------------------------------------------------------Protocolo DAM [NO TOCAR] --------------------------------------------------------------------//
  if(Serial.available()) {
        for (int i=0; i<TAM; i++) {
          Serial.print(Lectura[i]);
          Serial.print("-");
      }
      Serial.print("\n");

      /* -Escritura-
        String sI = Serial.readStringUntil('-');
        int i = sI.toInt();
        Escritura[i] = Serial.readStringUntil('\n');
        Lectura[i] = Escritura[i];
        */

      }
  //--------------------------------------------------------Protocolo DAM [NO TOCAR] --------------------------------------------------------------------//

  //recordMode = digitalRead(switchRecord);
  Piezo0Val = analogRead(Piezo0);
  Piezo1Val = analogRead(Piezo1);
  Piezo2Val = analogRead(Piezo2);
  Piezo3Val = analogRead(Piezo3);
  Piezo4Val = analogRead(Piezo4);
  Piezo5Val = analogRead(Piezo5);
  Piezo6Val = analogRead(Piezo6);
  Piezo7Val = analogRead(Piezo7);
  vol_BPM = analogRead(poteBPM);
  setBPM = digitalRead(switchBPM);
  int PiezoVal[8] = {Piezo0Val, Piezo1Val, Piezo2Val, Piezo3Val, Piezo4Val, Piezo5Val, Piezo6Val, Piezo7Val};
  
    for(int i=0; i<8; i++) {
      Lectura[i] = PiezoVal[i]; 
      }
bool enterBPM = true;
bool menuBPM = false;

          if(!setBPM) {
            menuBPM = !menuBPM;
          }

              if (menuBPM) { 
                if (enterBPM) {
                  oled.clearDisplay();
                  oled.setTextColor(WHITE);
                  oled.setTextSize(1);

                  oled.setCursor(0, 0);
                  oled.print("Tempo (BPM)");
                  oled.display();
                  enterBPM = false;
                  }
                  oled.setTextColor(WHITE);
                  oled.setTextSize(2);

                  oled.setCursor(30, 16);
                  unsigned long int BPM = map(vol_BPM, 0, 4095, 0, 300);
              
                  oled.print(BPM);
                  oled.display();
            }
}