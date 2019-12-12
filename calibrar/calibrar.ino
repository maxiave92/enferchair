#include <SoftwareSerial.h>
//Librerias del sensor GY
#include "Arduino.h"
#include "EMGFilters.h"

//constantes del sensor GY
#define TIMING_DEBUG 1
#define SensorInputSelectorPin A0
#define SensorInputAccionPin A5

EMGFilters filtro;

int frecuenciaMAX   = SAMPLE_FREQ_1000HZ;
int frecuenciaRuido = NOTCH_FREQ_50HZ;

// Variable LIMITE:
static int Limite_selector = 0;
static int Limite_accion   = 0;

int menu = 0;
int promedioControlar = 0;
int promedioActivar = 0;

SoftwareSerial mySerial(10, 11); //rx tx

void setup()
{
  Serial.begin(9450);
  mySerial.begin(9450);
  // inicializamos el filtro poniendo:
  //           frecuenciaMaxima,
  //           frecuenciaRuido, filtro de ruido activado, filtro de paso bajo activado, filtro de paso alto activado
  filtro.init(SAMPLE_FREQ_1000HZ, NOTCH_FREQ_50HZ, true, true, true);
  Limite_selector = calibrar(SensorInputSelectorPin, Limite_selector);
  Limite_accion = calibrar(SensorInputAccionPin, Limite_accion);
  promedioControlar = calcularSignalProm(SensorInputSelectorPin, Limite_selector);
  promedioActivar = calcularSignalProm(SensorInputAccionPin, Limite_accion);
}

void loop()
{Serial.println(Limite_selector);
Serial.println(Limite_accion);
Serial.println(promedioControlar);
Serial.println(promedioActivar);
    controlar();
    activar();
} // END LOOP

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ CONTROL ++++++++++++++++++
void controlar(){
  int valorSelector = getSignal(SensorInputSelectorPin, Limite_selector);
  if(valorSelector > promedioControlar){
    menu = cambiarDireccion();
  }
  imprimirSeleccion();
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ACTIVAR ++++++++++++++++++
void activar(){
  if(getSignal(SensorInputAccionPin, Limite_accion) > promedioActivar) {
     mySerial.write(menu);
  }
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ CAMBIAR DIRECCION ++++++++++++++++++
int cambiarDireccion(){
  return (++menu > 4) ? 1 : menu;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ IMPRIMIR SELECCION ++++++++++++++++++
void imprimirSeleccion(){
  switch (menu)
  {
  case 1:
    Serial.println("ADELANTE");
    break;
  case 2:
    Serial.println("DERECHA");
    break;
  case 3:
    Serial.println("IZQUIERDA");
    break;
  case 4:
    Serial.println("ATRAS");
    break;
  }
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ OBTENER SIGNAL++++++++++++++++++
int getSignal(int pinSeleccionado, int limite)
{
  int value = analogRead(pinSeleccionado);
  // procesamos la filtracion
  int valorObtenido = sq(filtro.update(value));
  // any value under threshold will be set to zero
  return (valorObtenido > limite) ? valorObtenido : 0;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ CALCULAR PROMEDIO SIGNAL ++++++++++++++++++
int calcularSignalProm(int pinSensor, int limite)
{
  int i = 0;
  int total = 0;
  for (i = 0; i < 100; i++)
  {
    total += getSignal(pinSensor, limite);
  }
  return total / 100;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ CALIBRAR ++++++++++++++++++
int calibrar(int pinSensor, int limite)
{
  delay(1500);
  int i = 0;
  int valorMax = 0;
  int entrada = 0;
  while (i < 100)
  {
    entrada = getSignal(pinSensor, limite);
    if (entrada > valorMax and entrada < 1000)
      valorMax = entrada;
    if(entrada > 100) i++;
  }
  return valorMax;
}
