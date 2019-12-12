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
static int Limite_selector = 600;
static int Limite_accion = 800;

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

     Serial.print("Limite_selector ");
  Serial.println(Limite_selector);
  delay(2000);
     Serial.print("Limite_accion");
  Serial.println(Limite_accion);
delay(2000);
     Serial.print("calculo promedio ");
  Serial.println(Limite_selector);
delay(2000);
  if (!isCalibrado())
  {
<<<<<<< HEAD
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
=======
    Serial.print("aqui empieza la calibracion");
 
    Limite_selector = calibrar(SensorInputSelectorPin, Limite_selector);
   // Limite_accion = calibrar(SensorInputAccionPin, Limite_accion);
   Serial.print("calculo Limite_selector ");
  Serial.print(Limite_selector);
  delay(5000);
    prom1 = calcularSignalProm(SensorInputSelectorPin,Limite_selector);
    Serial.print("calculo Limite_selector ");
  Serial.print(prom1);
   // prom2 = calcularSignalProm(SensorInputAccionPin, Limite_accion);
  }
  // ------------------------------------------ ENVIO DE INFORMACION ------------------------
  int i=0;
   while(i==0){
    control(prom1);
    activar(prom2);
   }
>>>>>>> 6d57cb70631736a23572476aaa8738ff3de04ccc

} // END LOOP


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ OBTENER SIGNAL++++++++++++++++++
int getSignal(int pinSeleccionado, int limite)
{
<<<<<<< HEAD
  Serial.println("================metodo obtener senal============");
  Serial.print("limite: ");
  Serial.println(limite);
  Serial.print("pin: ");
  Serial.println(pinSeleccionado);
  delay(500);
  
=======

>>>>>>> 6d57cb70631736a23572476aaa8738ff3de04ccc
  //escalaTiempo = micros();
  int Value = analogRead(pinSeleccionado);
  // procesamos la filtracion
  int DataAfterFilter = filtro.update(Value);
  int valorObtenido = sq(DataAfterFilter);
<<<<<<< HEAD
    // any value under threshold will be set to zero
    valorObtenido = (valorObtenido > limite) ? valorObtenido : 0;
    // escalaTiempo = micros() - escalaTiempo;  
    return valorObtenido;
=======
  // any value under threshold will be set to zero
  valorObtenido = (valorObtenido > limite) ? valorObtenido : 0;
  // escalaTiempo = micros() - escalaTiempo;
  
    Serial.print("lectura de senal ");
    Serial.println(valorObtenido);

  return valorObtenido;
>>>>>>> 6d57cb70631736a23572476aaa8738ff3de04ccc
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ CALIBRAR ++++++++++++++++++
int calibrar(int pinSensor, int limite)
{
<<<<<<< HEAD
//  Serial.println("Calibrar:");
=======
  
  delay(1500);
>>>>>>> 6d57cb70631736a23572476aaa8738ff3de04ccc
  int i = 0, valorMax = 0;

  int signalTomada = 0;

  while (i < 100)
  { Serial.println("cant mediciones");
  Serial.println(i);
    signalTomada = getSignal(pinSensor, limite);
<<<<<<< HEAD
    Serial.print("senal tomada: ");
    Serial.println(signalTomada);
//    Serial.print("valor maximo: ");
//    Serial.println(valorMax);
//    delay(500);
    if (signalTomada > valorMax)
    {
=======
    if (signalTomada > valorMax and signalTomada<1000)
>>>>>>> 6d57cb70631736a23572476aaa8738ff3de04ccc
      valorMax = signalTomada;
    if(signalTomada>100)
    i++;
  }
<<<<<<< HEAD
//  Serial.print("========================valor senal: ");
//  Serial.println(valorMax);
//  delay(5000);

=======
  Serial.print("LIMITE ESTABLECIDO ");
  Serial.print(valorMax);
  delay(1500);
>>>>>>> 6d57cb70631736a23572476aaa8738ff3de04ccc
  return valorMax;
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ IS CALIBRADO ++++++++++++++++++
bool isCalibrado()
{
<<<<<<< HEAD
  if (/*Limite_accion == 0 && */Limite_selector == 0)
    return false;
=======

  
  Serial.println("isCalibrado");
  delay(1500);
  Serial.println(Limite_accion);
  delay(2500);
  Serial.println(Limite_selector);
  delay(2500);
  if (Limite_accion == 0)
     return false;
>>>>>>> 6d57cb70631736a23572476aaa8738ff3de04ccc
  else
     { 
      if (Limite_selector == 0)
        return false;
      else
        return true;
     }
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ CALCULAR PROMEDIO SIGNAL ++++++++++++++++++
int calcularSignalProm(int pinSensor, int limite)
{
  int i = 0, total = 0, prom=0;
  while (i < 100)
  {
    total += getSignal(pinSensor, limite);
    i++; 
  }
  prom = total / 100;
  return prom;
}


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ CONTROL ++++++++++++++++++
void control(int valorPromedio){
  int valorSelector = getSignal(SensorInputSelectorPin, Limite_selector);
  if(valorSelector > valorPromedio){
    menu = cambiarDireccion();
    
  }
  imprimirSeleccion();
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
