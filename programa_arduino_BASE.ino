
  #define Piezo1 36
  #define Piezo2 39
  #define Piezo3 34
  #define Piezo4 35
  #define Piezo5 32
  #define Piezo6 33
  #define Piezo7 25
  #define Piezo8 26

  String Lectura[9];
 
void setup() {

  Serial.begin(115200);

}

void loop() {

  unsigned int PiezoVal1 = analogRead(Piezo1);
  unsigned int PiezoVal2 = analogRead(Piezo2);
  unsigned int PiezoVal3 = analogRead(Piezo3);
  unsigned int PiezoVal4 = analogRead(Piezo4);
  unsigned int PiezoVal5 = analogRead(Piezo5);
  unsigned int PiezoVal6 = analogRead(Piezo6);
  unsigned int PiezoVal7 = analogRead(Piezo7);
  unsigned int PiezoVal8 = analogRead(Piezo8);

  int PiezoVal[8] = {PiezoVal1, PiezoVal2, PiezoVal3, PiezoVal4, PiezoVal5, PiezoVal6, PiezoVal7, PiezoVal8};

if(Serial.available()) {
  for(int i=0; i<8; i++) {
    Lectura[i] = PiezoVal[i];
    Serial.print(Lectura[i]);
    Serial.print("-");
    }
  Serial.println("\n");
  }
  delay(50);
  }
