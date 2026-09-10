
#define Piezo0 36
#define Piezo1 39
#define Piezo2 34
#define Piezo3 35
#define Piezo4 32
#define Piezo5 33
#define Piezo6 25
#define Piezo7 26
#define switchMenu 23
#define switchRecord 22
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
int recordMode;
String Lectura[TAM];
String Escritura[TAM];
char cmd[2];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(switchMenu, INPUT);
  pinMode(switchRecord, INPUT);
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
        Lectura[i] = Escritura[i];
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
  int PiezoVal[8] = {Piezo0Val, Piezo1Val, Piezo2Val, Piezo3Val, Piezo4Val, Piezo5Val, Piezo6Val, Piezo7Val};
  
  if (menuMode) {    
    for(int i=0; i<8; i++)
      if(PiezoVal[i] > 30){
        PiezoVal[i] = 1;
        Lectura[i] = PiezoVal[i]; }
      else {
        PiezoVal[i] = 0;
        Lectura[i] = PiezoVal[i]; }
      }
    else {
      for (int i=0; i<8; i++) {
        Lectura[i] = PiezoVal[i]; 
      }
        Lectura[8] = recordMode;
    }
  }





