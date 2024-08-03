// all made with love by jairo gomez!

// Librerías
#include <SoftwareSerial.h> 
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>
#include <DHT_U.h>
#include <CapacitiveSensor.h>

SoftwareSerial BTserial(3, 4);
#define Type DHT11
int dhtPin = 2;
DHT HT(dhtPin, Type);
int humedad; // del DHT11
float temperatura; // del DHT11
int dlt1 = 3500;
int dlt2 = 500;
int dlt3 = 1500;
int dlt4 = 700;
int dlt5 = 1000;

// Capacitivo
const int ValorSensorAire = 632; // Valor obtenido en aire
const int ValorSensorAgua = 322; // Valor obtenido sumergido en agua
LiquidCrystal_I2C lcd(0x27, 16, 2); // Asignación de las dimensiones
int contador = 1; // Para el menú
const int lecturaCapacitiva = A0; // Pin de lectura capacitiva
int humedad2; // Humedad (mapeada con los valores de arriba) del sensor capacitivo
int lecturalluvia = A1; // Lectura de lluvia
int bombaPin = 5; //pin de la bomba
String estadobomba = "desactivada";
int rainSensorNA = 966; // Valor más bajo del sensor para mapear (sin agua)
int rainSensorMAX = 180; // Valor más alto del sensor para mapear (mucha agua)
int RainSensorVal; // Guardar el valor de lluvia
int sensorValue1; // Para guardar el valor del sensor capacitivo
int sensorValue2; // Para guardar el valor del sensor de lluvia
unsigned long lastPrintTime = 0; // Variable para almacenar el último tiempo de impresión
unsigned long printInterval = 1000; // Intervalo de impresión en milisegundos (1 segundo por defecto)
unsigned long BTprintInterVal = 1000; // Intervalo de impresión para Bluetooth

void setup() {
  Serial.begin(9600);
  BTserial.begin(9600);
  BTserial.write("conexion enlazada (HC-05)");
  BTserial.println("");
  BTserial.println("elige el estatus de la bomba:");
  BTserial.println("bomba Activa // bomba Inactivada");

  pinMode(lecturaCapacitiva, INPUT);
  pinMode(bombaPin,OUTPUT);
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
  lcd.clear(); // Fase #1 saludo al público
}

void loop() {
  digitalWrite(13, HIGH);

  // Lecturas del DHT
  humedad = HT.readHumidity();
  temperatura = HT.readTemperature();

  // Capacitiva lectura, mapeo, almacenamiento
  sensorValue1 = analogRead(lecturaCapacitiva);
  humedad2 = map(sensorValue1, ValorSensorAire, ValorSensorAgua, 0, 100);

  // Lluvia lectura, mapeo, almacenamiento
  sensorValue2 = analogRead(lecturalluvia);
  RainSensorVal = map(sensorValue2, rainSensorNA, rainSensorMAX, 0, 100);

if(contador == 1 || 9 ){
  lcd.setCursor(15, 1);
  lcd.print(contador); // el set de menú (abajo a la derecha)
}
if(contador == 10){
lcd.setCursor(14,0);
lcd.print(contador);
}
  if (Serial.available() > 0) { // Espera por la información serial
    String input = Serial.readStringUntil('\n'); // Leer la entrada del monitor serie
    printInterval = input.toInt(); // Convertir la entrada a un entero y actualizar el intervalo
    Serial.print("Nuevo intervalo de impresión: ");
    Serial.print(printInterval);
    Serial.println(" ms");
  }

  if (BTserial.available() > 0) { // Espera por la información del módulo Bluetooth
    String input = BTserial.readStringUntil('\n'); // Leer la entrada desde Bluetooth

    BTprintInterVal = input.toInt(); // Intentar convertir la entrada a un entero para actualizar el intervalo
    if (BTprintInterVal > 0) {
      BTserial.print("Nuevo intervalo de impresión: ");
      BTserial.print(BTprintInterVal);
      BTserial.println(" ms");
    }

    
    if (input.indexOf("bomba Activa") >= 0) { //activacion de la bomba
      digitalWrite(bombaPin,HIGH);
      estadobomba = "activada       ";
    }
      if (input.indexOf("bomba Inactiva") >= 0) { //deactivacion de la bomba
      digitalWrite(bombaPin,LOW);
      estadobomba = "desactivada    ";
    }
// no leer... 
    if (input.indexOf("amor") >= 0) {
      BTserial.println("");
      BTserial.println("Tus ojos brillan como perlas en el rio"); 
      delay(dlt4);
      BTserial.println("tu belleza unica a la luz de la luna");
      delay(dlt4);
      BTserial.println("que destella sobre calor de el lirio");
      delay(dlt4);
      BTserial.println("y rezo a tus ojos como a ninguna");
      delay(dlt5);
      BTserial.println("");
      BTserial.println("ya el sol y la estrellas no son bellas");
      delay(dlt4);
      BTserial.println("por que al al verte veo algo mas hermoso que ellas");
      delay(dlt5);
      BTserial.println("");
      BTserial.println("samantha deseo confesar...");
      delay(dlt5);
      BTserial.println("");
      BTserial.println("te amo");
    }
  }

  unsigned long currentTime = millis();
  if (currentTime - lastPrintTime >= printInterval) { // Se aplica a la información
    lastPrintTime = currentTime;
    Serial.print("dht11: Temperatura = "); // DHT11 serial
    Serial.print(temperatura);
    Serial.print(" Humedad = ");
    Serial.print(humedad);
    
    Serial.print("/");

    Serial.print("capacitive soil = "); // Capacitive serial
    Serial.print(humedad2);

    Serial.print("/");

    Serial.print("MHDR raindrops = "); // Rain serial
    Serial.println(RainSensorVal);
  }

  // Imprimir datos por Bluetooth si ha pasado el intervalo de tiempo correspondiente
  if (currentTime - lastPrintTime >= BTprintInterVal) {
    BTserial.print("dht11: Temperatura = ");
    BTserial.print(temperatura);
    BTserial.print(" Humedad = ");
    BTserial.print(humedad);

    BTserial.print("/");

    BTserial.print("capacitive soil = ");
    BTserial.print(humedad2);

    BTserial.print("/");

    BTserial.print("MHDR raindrops = ");
    BTserial.println(RainSensorVal);
  }

  if (digitalRead(12) == LOW) {
    contador++;
    delay(100);
  }

  if (contador == 11) { // No pase de 11 porque no hay más
    contador = 1;
  }

  if (RainSensorVal < 0) { // No pasarse de 0 (valor erróneo o descontrolado)
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

  if (contador == 8){
    lcd.setCursor(1, 0); //rx4 tx3
    lcd.print("bluetooth-hc05 ");
    lcd.setCursor(0, 1);
    lcd.print(" rx->4D tx->3D ");
  }

  if(contador == 9){
    lcd.setCursor(1,0);
    lcd.print("bomba -> 5D    ");
    lcd.setCursor(1,1);
    lcd.print(estadobomba);
    lcd.print("  ");
  }
  
  if (contador == 10) {
    lcd.setCursor(0, 0);
    lcd.print("     LOVE TO  ");
    lcd.setCursor(0, 1);
    lcd.print("{*STAR QUEEN*}"); // Verificación de LCD, código, RAM, almacenamiento completo...
  }
}
