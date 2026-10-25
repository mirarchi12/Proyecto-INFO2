#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

  #define Piezo1 36
  #define Piezo2 39
  #define Piezo3 34
  #define Piezo4 35
  #define Piezo5 32
  #define Piezo6 33
  #define Piezo7 25
  #define Piezo8 26

 
  #define ANCHO_PANTALLA 128
  #define ALTO_PANTALLA 64
  #define OLED_RESET -1

  #define OLED_SDA 21
  #define OLED_SCL 22

  #define record 23
  #define ledRec 15
//defino pin para potenciometro y buzzer
  #define potPin 27
  #define buzzerPin 14

// crea un objeto de la clase Adafruit_SSD1306 llamado display
// los primeros dos parametros indican las dimensiones del display
// el tercer parametro indica la interfaz de comunicacion
// el ultimo parametro indica que pin resetea la pantalla
// es necesario asignarlo en constructor aunque no lo vayamos a usar
// pq si no es un error de compilacion.
Adafruit_SSD1306 display(
  ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, OLED_RESET
);

// valores para la logica
unsigned long ultimoPulso = 0;
int bpm = 120;
int cuenta = 0;
bool recording = false;
bool prevrecordMode = false;

void setup() {
  // Se inicia la comunicacion serie entre la computadora y el microcontrolador

  Serial.begin(921600);
  
  pinMode(buzzerPin, OUTPUT);
  pinMode(record, INPUT_PULLDOWN);
  pinMode(ledRec, OUTPUT);

  // Establece la resolucion de lectura para todos los valores 
  // analogicos en 12 bits.

  analogReadResolution(12);

// Inicializar I2C: primero SDA, después SCL
  // los valores fueron creados al principio del programa
  Wire.begin(OLED_SDA, OLED_SCL);

// intenta inicializar el display y si falla te manda error
// funciona llamando al metodo begin del objeto display  y dandole
// valor de la biblioteca de alimentacion y la direccion de i2c de la pantalla
// no me preguntes bien como funciona pq no estoy seguro
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Error al inicializar OLED");
  }
// Pone una pantallita al encender que dice drumkit v1
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("DRUMKIT V1");
  display.display();

  delay(4000);
}



void loop() {
// Seccion de lectura 
  unsigned int valor1 = analogRead(Piezo1);
  unsigned int valor2 = analogRead(Piezo2);
  unsigned int valor3 = analogRead(Piezo3);
  unsigned int valor4 = analogRead(Piezo4);
  unsigned int valor5 = analogRead(Piezo5);
  unsigned int valor6 = analogRead(Piezo6);
  unsigned int valor7 = analogRead(Piezo7);
  unsigned int valor8 = analogRead(Piezo8);

  int PiezoVal[8] = {valor1, valor2, valor3, valor4, valor5, valor6, valor7, valor8};

// Boton de grabacion y Potenciometro de BPM
  bool recordMode = digitalRead(record);
  float valorPot = analogRead(potPin);

  bpm = map(valorPot, 0, 4095, 40, 240);

// Envio de senal de los datos por puerto serie
if (Serial.available()){
for (int i = 0; i < 8; i++) {
  Serial.print(PiezoVal[i]);
  Serial.print("-");
  }

  if (recordMode == 1 && prevrecordMode == 0) {
    recording = !(recording);
  }
  
  
  prevrecordMode = recordMode;
  Serial.print(recording);
  Serial.print("-");
  Serial.print(bpm);
  Serial.print("\n");
}


// En intervalo, dividimos un minuto (60.000UNSIGED LONG) entre el bpm para indicar cuanto ms hay entre cada beep del metronomo.
// En ahora, se indica el tiempo transcurrido desde que se inicio el programa.
  unsigned long intervalo = 60000UL / bpm;
  unsigned long ahora = millis();

//Si la diferencia entre ahora y ultimoPulso es mayor a intervalo, suena el buzzer y asigna ahora a ultimoPulso.
//Cada vez que pase el intervalo va haciendo una cuenta del 1 al 4 (este metronomo solo cuenta en 4).
//  **El beat 1 suena mas fuerte que los demas.
  if (ahora - ultimoPulso >= intervalo) {
    ultimoPulso = ahora;

    cuenta++;

    if (cuenta > 4) {
      cuenta = 1;
    }

    if(cuenta == 1) {
    tone(buzzerPin, 2000, 40);
    }
    else 
      tone(buzzerPin,1000,40);
  }

//limpia el display en cada vuelta y muestra:

//bpm: (bpm)
//pulso:(cuenta) (1,2,3,4)
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("METRONOMO");
  display.drawLine(0, 12, 127, 12, SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print("BPM: ");
  display.println(bpm);

  display.setTextSize(1);
  display.setCursor(0, 46);
  display.println("PULSO");

  display.setTextSize(2);
  display.setCursor(75, 42);
  display.print(cuenta);

  display.display();


  }

