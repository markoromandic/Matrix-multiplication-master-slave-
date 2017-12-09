#include <Wire.h>
#include "protokol.h"

int *f_matrix;
int *s_matrix;

byte slaves[128];
byte numSlaves = 0;

int x1, x2, y1, y2;

void setup() {
  Wire.begin();
  Serial.begin(115200);
}

void loop() {
  numSlaves = 0;
  for(byte i = 0; i < 128; i++){
    if(pingSlave(i)){
      numSlaves++;
      slaves[i] = 1;
    }
    else
      slaves[i] = 0;
  }
  
  Serial.print("Number of slaves: ");
  Serial.println(numSlaves);

  readMatrix();
  printMatrix(x1, y1, f_matrix);
  printMatrix(x2, y2, s_matrix);
  seperateTasks();
  
  delay(500);
}

bool readMatrix(){
  Serial.println("Enter size of first matrix x, y");
  while(!Serial.available());
  x1 = Serial.parseInt();
  Serial.print("x: ");
  Serial.println(x1);
  while(!Serial.available());
  y1 = Serial.parseInt();
  Serial.print("y: ");
  Serial.println(y1);
  
  f_matrix = malloc(x1 * y1 * sizeof(int));

  Serial.println("Enter elements of the first matrix");

  int *element = f_matrix;
  
  for(int i = 0; i < x1; i++){
    Serial.print("Enter elements of row ");
    Serial.print(i + 1);
    Serial.println(" matrix");
    for(int j = 0; j < y1; j++){
       while(!Serial.available());
       *element++ = Serial.parseInt();
       Serial.print("i, j: ");
       Serial.print(i);
       Serial.print(" ");
       Serial.println(j);
    }
  }
  
  Serial.println("Enter size of second matrix x, y");
  while(!Serial.available());
  x2 = Serial.parseInt();
  Serial.print("x: ");
  Serial.println(x2);
  while(!Serial.available());
  y2 = Serial.parseInt();
  Serial.print("y: ");
  Serial.println(y2);

  if(y1 != x2){
    Serial.println("Invalid sizes");
    return false;
  }

  s_matrix = malloc(x2 * y2 * sizeof(int));
  
  element = s_matrix;
  
  for(int i = 0; i < x2; i++){
    Serial.print("Enter elements of row ");
    Serial.print(i + 1);
    Serial.println(" matrix");
    for(int j = 0; j < y2; j++){
       while(!Serial.available());
       *element++ = Serial.parseInt();
       Serial.print("i, j: ");
       Serial.print(i);
       Serial.print(" ");
       Serial.println(j);
    }
  }

  return true;
}

void printMatrix(int x, int y, int *p_matrix){
  Serial.println("Printing matrix");
  
  for(int i = 0; i < x; i++){
    for(int j = 0; j < y; j++){
      Serial.print(*p_matrix++);
      Serial.print(" ");
    }
    Serial.println();
  }
}

bool seperateTasks(){
  int rows_per_slave = y1 / numSlaves;
  int *rows_to_send;
  int slaveNum = 0;
  for(byte i = 0; i < 128;  i++){
    if(slaves[i]){
      sendSecondMatrix(i);
      
      rows_to_send = f_matrix + slaveNum * rows_per_slave * x1;
      
//      sendTask(i, rows_to_send);
    }
  }
}

bool sendTask(byte adresa, int *p_matrix, int numBytes){
  
  return true;
}

bool sendSecondMatrix(byte adress){
    Wire.beginTransmission(adress);
    Wire.write(MATRIX_SECOND);
    Wire.write(x2);
    Wire.write(y2);
    for(int i = 0; i < x2 * y2; i++){
      int val = *s_matrix;
      Wire.write(val);
      s_matrix++;
    }
    Wire.endTransmission();
}

byte pingSlave(byte adresa){
  Wire.beginTransmission(adresa);
  Wire.write(PING);
  Wire.endTransmission();

  Wire.requestFrom(adresa, 1);

  byte id = Wire.read();

  
  if(id == PING){
    Serial.print("ID: ");
    Serial.println(id);
    return 1;
  }
  return 0;
}
