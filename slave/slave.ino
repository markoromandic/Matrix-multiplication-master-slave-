#include <Wire.h>
#include "protokol.h"

byte current_command;

int *s_matrix;

int x1, y1, x2, y2;

void setup() {
  // put your setup code here, to run once:
  Wire.begin(8);
  Wire.onReceive(recieveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(115200);
}

void loop() {
}


void recieveEvent(int howMany){
    current_command = Wire.read();

    switch(current_command){
      case PING:
        Serial.println("PING");
        break;
      case MATRIX_SECOND:
        Serial.println("MATRIX_SECOND");
        buildSecondMatrix();
        break;
      case UNKNOWN_C:
        Serial.println("UNKNOWN");
        break;
     }
}

void pingResponse(){
  Wire.write(PING);
}

void buildSecondMatrix(){
  x2 = Wire.read();
  y2 = Wire.read();

  s_matrix = malloc(x2 * y2 * sizeof(int));

  for(int i = 0; i < x2 * y2; i++){
    int val = Wire.read();
    Serial.println(val);
    *s_matrix++ = val;
  }
  
  Serial.print("x2: ");
  Serial.println(x2);
  Serial.print("y2: ");
  Serial.println(y2);

  printMatrix(x2, y2, s_matrix);
}

void printMatrix(int x, int y, int *p_matrix){
  Serial.println("Printing matrix");
  
  for(int i = 0; i < x; i++){
    for(int j = 0; j < y; j++){
      Serial.print((*p_matrix)++);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void requestEvent(){
  switch(current_command){
      case PING:
        pingResponse();
        break;
      case UNKNOWN_C:
        Serial.println("UNKNOWN");
        break;
  }
}

