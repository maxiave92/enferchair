#include <EMGFilters.h>

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
    Serial.print("CALIBRADO: ");
    Serial.println(!isCalibrado());
    Limite_selector = calibrar(SensorInputSelectorPin, Limite_selector);
    //Limite_accion = calibrar(SensorInputAccionPin, Limite_accion);
    prom1 = calcularSignalProm(SensorInputSelectorPin, Limite_selector);
    //prom2 = calcularSignalProm(SensorInputAccionPin, Limite_accion);
  }
  // ------------------------------------------ ENVIO DE INFORMACION ------------------------
    //Serial.println("OBTENER SENAL:");
    //Serial.println(prom1);
    while(true){
      control(prom1);
    //activar(prom2);
    }

} // END LOOP


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ OBTENER SIGNAL++++++++++++++++++
int getSignal(int pinSeleccionado, int limite)
{
  Serial.println("================metodo obtener senal============");
  Serial.print("limite: ");
  Serial.println(limite);
  Serial.print("pin: ");
  Serial.println(pinSeleccionado);
  delay(500);
  
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

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ CALIBRAR ++++++++++++++++++
int calibrar(int pinSensor, int limite)
{
//  Serial.println("Calibrar:");
  int i = 0, valorMax = 0;

  int signalTomada = 0;

  while (i < 500)
  {
    signalTomada = getSignal(pinSensor, limite);
    Serial.print("senal tomada: ");
    Serial.println(signalTomada);
//    Serial.print("valor maximo: ");
//    Serial.println(valorMax);
//    delay(500);
    if (signalTomada > valorMax)
    {
      valorMax = signalTomada;
    }
    i++;
  }
//  Serial.print("========================valor senal: ");
//  Serial.println(valorMax);
//  delay(5000);

  return valorMax;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ IS CALIBRADO ++++++++++++++++++
bool isCalibrado()
{
  if (/*Limite_accion == 0 && */Limite_selector == 0)
    return false;
  else
    return true;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ CALCULAR PROMEDIO SIGNAL ++++++++++++++++++
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


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ CONTROL ++++++++++++++++++
void control(int valorPromedio){
  int valorSelector = getSignal(SensorInputSelectorPin, Limite_selector);
  if(valorSelector > valorPromedio){
    menu = cambiarDireccion();
    imprimirSeleccion();
  }
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ ACTIVAR ++++++++++++++++++
void activar(int valorPromedio){
   int valorAccion = getSignal(SensorInputAccionPin, Limite_accion);
  if(valorAccion > valorPromedio){
     mySerial.write(menu);
  }
}


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ CAMBIAR DIRECCION ++++++++++++++++++
int cambiarDireccion(){
  if(menu > 4){
    menu = 1;
  } else {
    menu++;
  }
  return menu;
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
