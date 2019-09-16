/**
   This code is for program the myochair's funcionality

      code by Isaias
*/

#include <Fuzzy.h>
#include <FuzzyComposition.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzyOutput.h>
#include <FuzzyRule.h>
#include <FuzzyRuleAntecedent.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzySet.h>

int izqA = 5;  //motor izquierdo adelante
int izqB = 6;  //motor izquierdo Atras
int derA = 9;  //moto derecho adelanta
int derB = 10; //motor derecho atras
float vel = 0;            // Velocidad de los motores (Difusa)
int estado = 0;         // inicia detenido
int cont = 0;
int pecho = 2;            // define el pin 2 como (pecho) para el Ultrasonido
int ptrig = 3;            // define el pin 3 como (ptrig) para el Ultrasonido
int duracion, distancia;  // para Calcular distacia

int ledUp = 8;
int ledDown = 13;
int ledLeft = 12;
int ledRight = 11;

// paso 1 -  Instanciando un objeto de la libreria
Fuzzy* fuzzy = new Fuzzy();
FuzzySet* grande = new FuzzySet(60, 80, 150, 250); // distancia grande
FuzzySet* segura = new FuzzySet(30, 50, 50, 70); // distancia segura
FuzzySet* corta = new FuzzySet(0, 20, 20, 40); // corta distancia
FuzzySet* rapido = new FuzzySet(180, 240, 240, 240); // velocidad rapido // velocidad baja(30, 40, 40, 50);
FuzzySet* promedio = new FuzzySet(90, 150, 150, 210); // velocidad promedio
FuzzySet* baja = new FuzzySet(0, 60, 60, 120);

void setup() {
  // inicia el puerto serial para comunicacion
  Serial.begin(9600);
  // definicion de pines de motor como salida
  pinMode(derA, OUTPUT);
  pinMode(derB, OUTPUT);
  pinMode(izqA, OUTPUT);
  pinMode(izqB, OUTPUT);

  pinMode(ledUp, OUTPUT);
  pinMode(ledDown, OUTPUT);
  pinMode(ledLeft, OUTPUT);
  pinMode(ledRight, OUTPUT);

  pinMode(pecho, INPUT);   // define el pin 2 como entrada (pecho)
  pinMode(ptrig, OUTPUT);  // define el pin 3 como salida  (ptrig)


  // paso 2 - Creando una distancia FuzzyInput
  FuzzyInput* distancia = new FuzzyInput(1);// With its ID in param

  // Creando el FuzzySet para componer la distancia FuzzyInput
  fuzzy->addFuzzyInput(distancia); // agrega el FuzzyInput al Fuzzy object

  distancia->addFuzzySet(corta); // agrega FuzzySet corta a la distancia
  distancia->addFuzzySet(segura); // agrega FuzzySet segura a la distance
  distancia->addFuzzySet(grande);

  fuzzy->addFuzzyInput(distancia);
  // paso 3 - creando FuzzyOutput velocidad


  FuzzyOutput* velocidad = new FuzzyOutput(1);// con su ID de parametro
  // creando FuzzySet para componer el FuzzyOutput velocidad
  velocidad->addFuzzySet(baja); // agrega FuzzySet baja a la velocidad
  velocidad->addFuzzySet(promedio); // agrega FuzzySet promedio a la velocidad
  velocidad->addFuzzySet(rapido); // agrega FuzzySet rapido a la velocidad

  fuzzy->addFuzzyOutput(velocidad); // agregar FuzzyOutput a Fuzzy object

  //ensamblar las reglas difusas
  // FuzzyRule "IF distancia = corta THEN velocidad = baja";
  FuzzyRuleAntecedent* SiDistanciaEsCorta = new FuzzyRuleAntecedent(); // instanciando el antecedente de la expresion
  SiDistanciaEsCorta->joinSingle(corta); // agregando el correspondiente FuzzySet al antecedente creado
  FuzzyRuleConsequent* EntoncesVelocidadEsBaja = new FuzzyRuleConsequent(); // instanciando el consecuente de la expresion
  EntoncesVelocidadEsBaja->addOutput(baja);// agregando el correspondiente FuzzySet al consecuente creado
  // instanciando una FuzzyRule
  FuzzyRule* reglaDifusa1 = new FuzzyRule(1, SiDistanciaEsCorta, EntoncesVelocidadEsBaja); // pasando el antecedente y el consecuente de la expresion
  fuzzy->addFuzzyRule(reglaDifusa1); // agrega FuzzyRule al Fuzzy object

  // FuzzyRule "IF distancia = segura THEN velocidad = promedio"
  FuzzyRuleAntecedent* SiDistanciaEsSegura = new FuzzyRuleAntecedent(); // instanciando el antecedente de la expresion
  SiDistanciaEsSegura->joinSingle(segura); // agregando el correspondiente FuzzySet al antecedente creado
  FuzzyRuleConsequent* EntoncesVelocidadEsPromedio = new FuzzyRuleConsequent(); // instanciando el consecuente de la expresion
  EntoncesVelocidadEsPromedio->addOutput(promedio); // agregando el correspondiente FuzzySet al consecuente creado
  // instancianto una FuzzyRule
  FuzzyRule* reglaDifusa2 = new FuzzyRule(2, SiDistanciaEsSegura, EntoncesVelocidadEsPromedio); // pasando el antecedente y el consecuente de la expresion
  fuzzy->addFuzzyRule(reglaDifusa2); // agrega FuzzyRule al Fuzzy object

  // FuzzyRule "IF distancia = grande THEN velocidad = rapido"
  FuzzyRuleAntecedent* SiDistanciaEsGrande = new FuzzyRuleAntecedent(); // instanciando el antecedente de la expresion
  SiDistanciaEsGrande->joinSingle(grande); // agregando el correspondiente FuzzySet al antecedente creado
  FuzzyRuleConsequent* EntoncesVelocidadEsRapido = new FuzzyRuleConsequent(); // instanciando el consecuente de la expresion
  EntoncesVelocidadEsRapido->addOutput(rapido);// agregando el correspondiente FuzzySet al consecuente creado
  // instancianto una FuzzyRule
  FuzzyRule* reglaDifusa3 = new FuzzyRule(3, SiDistanciaEsGrande, EntoncesVelocidadEsRapido); // pasando el antecedente y el consecuente de la expresion
  fuzzy->addFuzzyRule(reglaDifusa3); // agrega FuzzyRule al Fuzzy object
} // END SETUP

/**
   LOOP METHOD START HERE
      by MyoChair
*/
void loop()  {

  estado = Serial.read();
  

  message("Estado", estado);

  /**
     00 Parar
     49 Adelante
     51 Izquierda
     50 Derecha
     52 Atras
  */
  switch (estado) {
    case 0:
      //Parar
      movimiento(0, 0, 0, 0);
      apagarTodo();
      break;


    case 1:
      //MOVIMIENTO ADELANTE
      apagarTodo();
      encender(ledUp);

      message("dentro", estado);
      digitalWrite(ptrig, HIGH);   // genera el pulso de trigger por 10us
      delay(0.01);
      digitalWrite(ptrig, LOW);
      duracion = pulseIn(pecho, HIGH);              // Lee el tiempo del Echo
      distancia = (duracion / 2) / 29;          // calcula la distancia en centimetros

      if (distancia <= 15 && distancia >= 2) {  //Contro distancia para ver si anda
        // Enciende LED
        Serial.print("\n choco = ");
        movimiento(0, 0, 0, 0);
      }
      else {
        while (cont <= 3) {
          //lee distancia
          digitalWrite(ptrig, HIGH);   // genera el pulso de trigger por 10us
          delay(0.01);
          digitalWrite(ptrig, LOW);
          duracion = pulseIn(pecho, HIGH);              // Lee el tiempo del Echo
          distancia = (duracion / 2) / 29;          // calcula la distancia en centimetros
          delay(100);
          message("distancia", distancia);
          int dist = distancia;
          //empieza difusa
          fuzzy->setInput(1, dist);
          // paso 6 - ejecuta la fuzzificacion
          fuzzy->fuzzify();
          // paso 7 - ejecuta la desfuzzyficacion para cada salida, pasando su ID
          float output = fuzzy->defuzzify(1);

          vel = output;
          message("Velocidad", vel);
          if (vel <= 0)
            vel *= -1;
          movimiento(0, 0, vel, vel);
          cont++;
        }
      }
      break;
    case 3:
      //MOVIMIENTO A LA IZQUIERDA
      apagarTodo();
      encender(ledLeft);
      message("dentro", estado);
      digitalWrite(ptrig, HIGH);   // genera el pulso de trigger por 10us
      delay(0.01);
      digitalWrite(ptrig, LOW);
      duracion = pulseIn(pecho, HIGH);              // Lee el tiempo del Echo
      distancia = (duracion / 2) / 29;          // calcula la distancia en centimetros
      if (distancia <= 15 && distancia >= 2) {  //Contro distancia para ver si anda
        // Enciende LED
        Serial.print("\n choco = ");
        movimiento(0, 0, 0, 0);
      }
      else {
        while (cont <= 3) {
          //lee distancia
          digitalWrite(ptrig, HIGH);   // genera el pulso de trigger por 10us
          delay(0.01);
          digitalWrite(ptrig, LOW);
          duracion = pulseIn(pecho, HIGH);              // Lee el tiempo del Echo
          distancia = (duracion / 2) / 29;          // calcula la distancia en centimetros
          delay(100);
          message("Distancia", distancia);
          int dist = distancia;
          //empieza difusa
          fuzzy->setInput(1, dist);
          // paso 6 - ejecuta la fuzzificacion
          fuzzy->fuzzify();
          // paso 7 - ejecuta la desfuzzyficacion para cada salida, pasando su ID
          float output = fuzzy->defuzzify(1);
          vel = output;
          message("Velocidad", vel);
          if (vel <= 0)
            vel *= -1;
          movimiento(0, vel, 0, 0);
          cont++;
        }

      }
      break;
    case 2:
      //MOVIMIENTO A LA DERECHA
      apagarTodo();
      encender(ledRight);
      message("dentro", estado);
      digitalWrite(ptrig, HIGH);   // genera el pulso de trigger por 10us
      delay(0.01);
      digitalWrite(ptrig, LOW);
      duracion = pulseIn(pecho, HIGH);              // Lee el tiempo del Echo
      distancia = (duracion / 2) / 29;          // calcula la distancia en centimetros
      if (distancia <= 15 && distancia >= 2) {  //Contro distancia para ver si anda
        // Enciende LED
        Serial.print("\n choco = ");
        movimiento(0, 0, 0, 0);
      }
      else {
        while (cont <= 3) {
          //lee distancia
          digitalWrite(ptrig, HIGH);   // genera el pulso de trigger por 10us
          delay(0.01);
          digitalWrite(ptrig, LOW);
          duracion = pulseIn(pecho, HIGH);              // Lee el tiempo del Echo
          distancia = (duracion / 2) / 29;          // calcula la distancia en centimetros
          delay(100);
          message("distancia", distancia);
          int dist = distancia;
          //empieza difusa
          fuzzy->setInput(1, dist);
          // paso 6 - ejecuta la fuzzificacion
          fuzzy->fuzzify();
          // paso 7 - ejecuta la desfuzzyficacion para cada salida, pasando su ID
          float output = fuzzy->defuzzify(1);
          vel = output;
          message("Velocidad", vel);
          if (vel <= 0)
            vel *= -1;
          movimiento(vel, 0, 0, 0);
          cont++;
        }
      }
      break;
    case 4:
      // MOVIMIENTO ATRAS
      apagarTodo();
      encender(ledDown);
      message("dentro", estado);
      digitalWrite(ptrig, HIGH);   // genera el pulso de trigger por 10us
      delay(0.01);
      digitalWrite(ptrig, LOW);
      duracion = pulseIn(pecho, HIGH);              // Lee el tiempo del Echo
      distancia = (duracion / 2) / 29;          // calcula la distancia en centimetros
      if (distancia <= 15 && distancia >= 2) {  //Contro distancia para ver si anda
        // Enciende LED
        Serial.print("\n choco = ");
        movimiento(0, 0, 0, 0);
      }
      else {
        while (cont <= 3) {
          //lee distancia
          digitalWrite(ptrig, HIGH);   // genera el pulso de trigger por 10us
          delay(0.01);
          digitalWrite(ptrig, LOW);
          duracion = pulseIn(pecho, HIGH);              // Lee el tiempo del Echo
          distancia = (duracion / 2) / 29;          // calcula la distancia en centimetros
          delay(100);
          message("distancia", distancia);
          int dist = distancia;
          //empieza difusa
          fuzzy->setInput(1, dist);
          // paso 6 - ejecuta la fuzzificacion
          fuzzy->fuzzify();
          // paso 7 - ejecuta la desfuzzyficacion para cada salida, pasando su ID
          float output = fuzzy->defuzzify(1);
          vel = output;
          message("Velocidad", vel);
          if (vel <= 0)
            vel *= -1;
          movimiento(vel, vel, 0, 0);
          cont++;
        }
      }
      break;
    default:
      //message("TRASH", 0);

      break;
  }//END SWITCH
  cont = 0;
}// END LOOP

/**
    This method work for select the next move for the engines
*/
void movimiento(int velocidadMotorDerechoB, int velocidadMotorIzquierdoB, int velocidadMotorDerechoA, int velocidadMotorIzquierdoA) {
  analogWrite(derB, velocidadMotorDerechoB);
  analogWrite(izqB, velocidadMotorIzquierdoB);
  analogWrite(derA, velocidadMotorDerechoA);
  analogWrite(izqA, velocidadMotorIzquierdoA);

}

/**
    This method is used for print a message for console/monitor
*/
void message(String frase, int dato) {
  Serial.print("\n" + frase + "= ");
  Serial.print(dato);
}//END METHOD "message"

void encender(int pinLed) {
  digitalWrite(pinLed, HIGH);
}
void apagar(int pinLed) {
  digitalWrite(pinLed, LOW);
}
void apagarTodo() {
  apagar(ledUp);
  apagar(ledDown);
  apagar(ledRight);
  apagar(ledLeft);
}
void parar() {
  apagarTodo();
  movimiento(0, 0, 0, 0);
}

