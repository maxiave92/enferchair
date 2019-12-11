#include <SoftwareSerial.h>

//Librerias del sensor GY
#include "Arduino.h"
#include "EMGFilters.h"

//constantes del sensor GY
#define TIMING_DEBUG 1
#define SensorInputSelectorPin A0
#define SensorInputAccionPin A5

EMGFilters filtro;

int frecuenciaMAX = SAMPLE_FREQ_1000HZ;
int frecuenciaRuido = NOTCH_FREQ_50HZ;

// Variable LIMITE:
static int Limite_selector = 0;
static int Limite_accion = 0;

unsigned long escalaTiempo;
unsigned long tiempoMedio;


int menu = 0;
int prom1 = 0;
int prom2 = 0;

SoftwareSerial mySerial(10, 11); //rx tx


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9450);
  mySerial.begin(9450);

  // inicializamos el filtro poniendo:
  //           frecuenciaMaxima,
  //           frecuenciaRuido, filtro de ruido activado, filtro de paso bajo activado, filtro de paso alto activado
  filtro.init(frecuenciaMAX, frecuenciaRuido, true, true, true);
  // calculamos el medio de tiempo
  tiempoMedio = 1e6 / frecuenciaMAX;
}

void loop()
{

  // ------------------------------------------ CALIBRACION ------------------------------
  if (!isCalibrado())
  {
    Limite_selector = calibrar(SensorInputSelectorPin, Limite_selector);
    Limite_accion = calibrar(SensorInputAccionPin, Limite_accion);
    prom1 = calcularSignalProm(SensorInputSelectorPin, Limite_accion);
    prom2 = calcularSignalProm(SensorInputAccionPin, Limite_accion);
  }
  // ------------------------------------------ ENVIO DE INFORMACION ------------------------
  while (true)
  {
    control(prom1);
    activar(prom2);
  }

} // END LOOP

int getSignal(int pinSeleccionado, int limite)
{
  //escalaTiempo = micros();
  int Value = analogRead(pinSeleccionado);
  // procesamos la filtracion
  int DataAfterFilter = filtro.update(Value);
  int valorObtenido = sq(DataAfterFilter);
  // any value under threshold will be set to zero
  valorObtenido = (valorObtenido > limite) ? valorObtenido : 0;
  // escalaTiempo = micros() - escalaTiempo;
  return valorObtenido;
}
int calibrar(int pinSensor, int limite)
{
  int i = 0, valorMax = 0;

  int signalTomada = 0;

  while (i < 500)
  {
    signalTomada = getSignal(pinSensor, limite);
    if (signalTomada > valorMax)
    {
      valorMax = signalTomada;
    }
    i++;
  }
  return valorMax;
}
bool isCalibrado()
{
  if (Limite_accion == 0 && Limite_selector == 0)
    return false;
  else
    return true;
}
int calcularSignalProm(int pinSensor, int limite)
{
  int i = 0, total = 0, prom=0;
  while (i < 500)
  {
    total += getSignal(pinSensor, limite);
    i++;
  }
  prom = total / 500;
  return prom;
}

void control(int valorPromedio){
  int valorSelector = getSignal(SensorInputSelectorPin, Limite_selector);
  if(valorSelector > valorPromedio){
    menu = cambiarDireccion();
    imprimirSeleccion();
  }
}


void activar(int valorPromedio){
   int valorAccion = getSignal(SensorInputAccionPin, Limite_accion);
  if(valorAccion > valorPromedio){
     mySerial.write(menu);
  }
}

int cambiarDireccion(){
  if(menu > 4){
    menu = 1;
  } else {
    menu++;
  }
  return menu;
}

void imprimirSeleccion(){
  switch (menu)
  {
  case 1:
      Serial.print("ADELANTE");
    break;
  case 2:
      Serial.print("DERECHA");
    break;
  case 3:
      Serial.print("IZQUIERDA");
    break;
  case 4:
      Serial.print("ATRAS");
    break;
  }
}
