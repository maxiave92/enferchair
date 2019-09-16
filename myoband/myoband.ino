#include <SoftwareSerial.h>

int sensorSelector = 0;
const int se = A0;
int menu = 0;
int sensorAccion = 0;
const int se2 = A5;


SoftwareSerial mySerial(10, 11); //rx tx
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9450);
  mySerial.begin(9450);

}

void loop() {
  // put your main code here, to run repeatedly:  



  sensorSelector = analogRead(se);
  delay(10);
  sensorSelector = analogRead(se);
  delay(10);
  Serial.print("\n senal = ");
  Serial.print(sensorSelector);
  delay(500);

  if (sensorSelector >= 450)
    menu = 1;



  //////////////////////////////////////////ADELANTE//////////////////////////////////////////////
  while (menu == 1) {

    sensorSelector = analogRead(se);
    delay(10);
    sensorSelector = analogRead(se);
    delay(10);
    Serial.print("\n senal MENU = ");
    Serial.print(sensorSelector);
    delay(500);
    Serial.print("\n DELANTE");
    if (sensorSelector >= 450) {
      menu++;
    }

    sensorAccion = analogRead(se2);
    delay(10);
    sensorAccion = analogRead(se2);
    delay(10);
    Serial.print("\n senal accion = ");
    Serial.print(sensorAccion);
    delay(500);

    if (sensorAccion > 450) {
      Serial.print("\n DELANTEE 1111111");
      mySerial.write(1);
    }



  }

  //////////////////////////////////////////DERECHA//////////////////////////////////////////////
  while (menu == 2) {

    sensorSelector = analogRead(se);
    delay(10);
    sensorSelector = analogRead(se);
    delay(10);
    Serial.print("\n senal MENU = ");
    Serial.print(sensorSelector);
    delay(500);
    Serial.print("\n DERECHA");
    if (sensorSelector >= 450)
      menu++;
    sensorAccion = analogRead(se2);
    delay(10);
    sensorAccion = analogRead(se2);
    delay(10);
    Serial.print("\n senal accion = ");
    Serial.print(sensorAccion);
    delay(500);

    if (sensorAccion > 450) {
      Serial.print("\n DERECHAAAAA 1111111");
      mySerial.write(2);
    }


  }

  //////////////////////////////////////////IZQUIERDA//////////////////////////////////////////////
  while (menu == 3) {
    Serial.print("\n IZQUIERDA");

    sensorSelector = analogRead(se);
    delay(10);
    sensorSelector = analogRead(se);
    delay(10);
    Serial.print("\n senal MENU = ");
    Serial.print(sensorSelector);
    delay(100);

    if (sensorSelector >= 450)
      menu++;

    sensorAccion = analogRead(se2);
    delay(10);
    sensorAccion = analogRead(se2);
    delay(10);
    Serial.print("\n senal accion = ");
    Serial.print(sensorAccion);
    delay(500);

    if (sensorAccion > 450) {
      Serial.print("\n IZQUIERDAAAAAAA !!!!!!");
      mySerial.write(3);
    }


  }


  //////////////////////////////////////////ATRAS//////////////////////////////////////////////
  while (menu == 4) {
    Serial.print("\n ATRAS");

    sensorSelector = analogRead(se);
    delay(10);
    sensorSelector = analogRead(se);
    delay(10);
    Serial.print("\n senal MENU = ");
    Serial.print(sensorSelector);
    delay(500);

    if (sensorSelector >= 450)
      menu = 1;

    sensorAccion = analogRead(se2);
    delay(10);
    sensorAccion = analogRead(se2);
    delay(10);

    Serial.print("\n senal accion = ");
    Serial.print(sensorAccion);
    delay(500);

    if (sensorAccion > 450) {
      Serial.print("\n ATRAAAAAS 1111111");
      mySerial.write(4);
    }



  }


}


int calibration()

{
  int total =0;
  int counter;
for(counter=0;counter<11;counter++)  {
    delay(1000);
      int val = analogRead(sensorPin);
    Serial.print("SimplyFun Calibration Value: ");
    Serial.print(val);
    Serial.print("\n");
    total+=val;
  }
  int calibrated = total/10;
  Serial.print("Calibrated Value:");
  Serial.print(calibrated);
  Serial.print("\n");
  return calibrated;
}       


