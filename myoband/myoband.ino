#include <SoftwareSerial.h>

//Librerias del sensor GY
#include "Arduino.h"
#include "EMGFilters.h"

//constantes del sensor GY
#define TIMING_DEBUG 1
#define SensorInputPin A0

EMGFilters filtro;

int frecuenciaMAX = SAMPLE_FREQ_1000HZ;
int frecuenciaRuido = NOTCH_FREQ_50HZ;

// Variable LIMITE:
static int Limite = 0;

unsigned long escalaTiempo;
unsigned long tiempoMedio;

int sensorSelector = 0;
const int se = A1;
int menu = 0;
int sensorAccion = 0;
const int se2 = A5;
int prom = 0;

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

  // ------------------------------------------ CODIGO CALIBRACION ------------------------------
  if (isCalibrado())
  {
    calibrar();
    prom = calcularSignalProm();
  }

  sensorSelector = getSignal();

  enviarSignal(sensorSelector);

}  // END LOOP

int getSignal(){
  //escalaTiempo = micros();
  int Value = analogRead(SensorInputPin);
  // procesamos la filtracion
  int DataAfterFilter = filtro.update(Value);
  int valorObtenido = sq(DataAfterFilter);
  // any value under threshold will be set to zero
  valorObtenido = (valorObtenido > limite) ? valorObtenido : 0;
  // escalaTiempo = micros() - escalaTiempo;
  return valorObtenido;
}

void calibrar(){
  int i = 0, valorMax = 0;

  int signalTomada = 0;

  while (i < 500)
  {
    signalTomada = getsignal();
    if (signalTomada > valorMax)
    {
      valorMax = signalTomada;
    }
    i++;
  }
  Serial.print("Valor Limite Calibrado=");
  Serial.print(Limite);
  Limite = valorMax;
}

bool isCalibrado(){
  if (Limite == 0)
    return false;
  else
    return true;
}

void enviarSignal(int aSignal){
  if (aSignal >= prom)
    menu = 1;
}

int calcularSignalProm(){
  int i = 0, total = 0;
  while (i < 500)
  {
    total += getSignal();
    i++;
  }
  prom = total / 500;
  return prom;
}
