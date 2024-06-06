// all made with love by jairo gomez!

// librerias
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>
#include <DHT_U.h>
#include <CapacitiveSensor.h>
#define Type DHT11
int dhtPin = 2;
DHT HT (dhtPin, Type);
int humedad; //de el DHT11
float temperatura;// de el DHT11
int dlt1 = 3500;
int dlt2 = 500;
int dlt3 =1500;
//capacitivo
const int ValorSensorAire = 632; // Valor obtenido en aire
const int ValorSensorAgua = 322; // Valor obtenido sumergido en agua
LiquidCrystal_I2C lcd(0x27, 16, 2); //asignacion de las dimensiones
int contador = 1; //para el menu
const int lecturaCapacitiva = A0; //pin de lectura capacitiva
int humedad2; // humedad (mapeada con los valores de arriva ) de el capacitor
int lecturalluvia; //lectura de lluvia
int rainSensorNA = 625; //valor mas bajo de el sesnor para mapear (sin agua)
int rainSensorMAX = 100; //valor mas alto de el sesnor para mapear (mucha agua)
int RainSensorVal; //guardar el valor de lluvia
int sensorValue1; //para guardar el valor de el sensor capacitivo
int sensorValue2; //para guardar el valor de el sensor de lluvia
unsigned long lastPrintTime = 0; // Variable para almacenar el último tiempo de impresión
unsigned long printInterval = 1000; // Intervalo de impresión en milisegundos (1 segundo por defecto)
void setup() {

  Serial.begin(9600);

  pinMode(lecturaCapacitiva, INPUT);
  HT.begin();
  lcd.init();

  lcd.backlight();

  lcd.setCursor(2, 0);
  lcd.print("BIENVENIDOS");
  delay(dlt1);
  lcd.setCursor(2, 1);
  lcd.print("A");
  delay(dlt2);
  lcd.print(" LA");
  delay(dlt2);
  lcd.print(" FERIA");
  delay(dlt2);
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("STEAM");
  lcd.setCursor(0, 1);
  lcd.print("GCRB");
  lcd.setCursor(12, 1);
  lcd.print("2024");
  delay(dlt3);
  lcd.clear();

  lcd.setCursor(0, 1);
  lcd.print("CARGANDO DATA");
  delay(dlt2);
  lcd.print(".");
  delay(dlt2);
  lcd.print(".");
  delay(dlt2);
  lcd.print(".");
  delay(dlt2);
  lcd.clear(); //fase #1 saludo a el publico
}

void loop() {
  digitalWrite(13, HIGH);
  lecturalluvia = analogRead(A1);

  //dht reads...
  humedad = HT.readHumidity();
  temperatura = HT.readTemperature();

  //capacitiva lectura,mapeo,almacenamiento
  sensorValue1 = analogRead(lecturaCapacitiva);
  humedad2 = map(sensorValue1, ValorSensorAire, ValorSensorAgua, 0, 100);
  //lluvia lectura,mapeo,almacenamiento
  sensorValue2 = analogRead(lecturalluvia);
  RainSensorVal = map(sensorValue2, rainSensorNA, rainSensorMAX, 0, 100);

  lcd.setCursor(15, 1);
  lcd.print(contador); // el set de menu (el de abajo a la derecha)


  if (Serial.available() > 0) { // espera por la informacion sereal
    String input = Serial.readStringUntil('\n'); // Leer la entrada del monitor serie
    printInterval = input.toInt(); // Convertir la entrada a un entero y actualizar el intervalo
    Serial.print("Nuevo intervalo de impresión: ");
    Serial.print(printInterval);
    Serial.println(" ms");
  }
  unsigned long currentTime = millis();
  if (currentTime - lastPrintTime >= printInterval) { //se aplica a la informacion
    lastPrintTime = currentTime;
    Serial.print("dht11: Temperatura = "); //dht 11 serial
    Serial.print(temperatura);
    Serial.print(" Humedad = ");
    Serial.print(humedad);
    
  Serial.print("/");
  
    Serial.print("capacitive soil = "); //capacitive serial
    Serial.print(humedad2);

  Serial.print("/");

    Serial.print("MHDR raindrops = "); //rain sereal
    Serial.println(RainSensorVal);
  }


  if (digitalRead(12) == LOW) {
    contador++;
    delay(100);
  }
  if (contador == 9) { //no pase de 9 por que no hay mas
    contador = 1;
  }
 if(RainSensorVal < 0){ // no pasarse de 0 (valor erroneo o descontrolado)
  RainSensorVal = 1;
 }

  if (contador == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Temperatura=");
    lcd.print(temperatura);
    lcd.setCursor(0, 1);
    lcd.print("Humedad=");
    lcd.print(humedad);
    lcd.print("     ");
  }

  if (contador == 2) {
    lcd.setCursor(0, 0);
    lcd.print("     Humedad    ");
    lcd.setCursor(0, 1);
    lcd.print("de suelo = ");
    lcd.print(humedad2);
    lcd.print("%");
  }

  if (contador == 3) {
    lcd.setCursor(0, 0);
    lcd.print("     lluvia    ");
    lcd.setCursor(0, 1);
    lcd.print("de clima = ");
    lcd.print(RainSensorVal);
    lcd.print("% ");
  }

  if (contador == 4) {
    lcd.setCursor(0, 0);
    lcd.print("hecho por jairo");
    lcd.setCursor(0, 1);
    lcd.print("gomez 8C       ");
  }

  if (contador == 5) {
    lcd.setCursor(0, 0);
    lcd.print("   conexiones: ");
    lcd.setCursor(0, 1);
    lcd.print("DHT11 DAT -> D2");
  }

  if (contador == 6) {
    lcd.setCursor(0, 1);
    lcd.print("RAIN A0 -> A1  ");
  }

  if (contador == 7) {
    lcd.setCursor(0, 1);
    lcd.print("SOILCA-A0 -> A0");
  }
  
  if (contador == 8) {
    lcd.setCursor(0, 0);
    lcd.print("     LOVE TO        ");
    lcd.setCursor(0, 1);
    lcd.print("{*STAR QUEEN*} "); //verificacion de lcd,codigo,ram,almacenamiento completo...
  }

}