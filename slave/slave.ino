#include <Wire.h>
#include "protokol.h"

#define GREEN_LED 2
#define RED_LED 3

byte current_command;

int *f_matrix;
int *s_matrix;

int x1, y1, x2, y2;

void setup() {
  // put your setup code here, to run once:
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  
  Wire.begin(8);
  Wire.onReceive(recieveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(115200);

  noInterrupts();

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  OCR1A = 15625;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12);
  TCCR1B |= (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  
  interrupts();
}

ISR(TIMER1_COMPA_vect){
  static bool on = false;
  static byte counter = 0;

  if(counter++ < 10)
    return;
  counter = 0;
  digitalWrite(GREEN_LED, on);
  
  digitalWrite(RED_LED, !on);

  if(on == true)
    on = false;
  else
    on = true;
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
        x2 = readInt();
        y2 = readInt();
        s_matrix = malloc(x2 * y2 * sizeof(int));
        buildMatrix(x2, y2, s_matrix);
        printMatrix(x2, y2, s_matrix);
        break;
      case MATRIX_FIRST:
        Serial.println("MATRIX_FIRST");
        x1 = readInt();
        y1 = readInt();
        f_matrix = malloc(x1 * y1 * sizeof(int));
        buildMatrix(x1, y1, f_matrix);
        printMatrix(x1, y1, f_matrix);
        break;
      case UNKNOWN_C:
        Serial.println("UNKNOWN");
        break;
     }
}

int readInt(){
  return Wire.read() | Wire.read() << 8;
}

void pingResponse(){
  Wire.write(PING);
}

void buildMatrix(int x, int y, int *p_matrix){
  for(int i = 0; i < x * y; i++)
    *p_matrix++ = readInt();
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

