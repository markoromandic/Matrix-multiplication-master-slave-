#include <Wire.h>
#include <protokol.h>

#define GREEN_LED 4
#define RED_LED 5

byte current_command;

volatile int progress;
volatile int idRow;
volatile int elementsSent = 0;
int *f_matrix;
int *bp_matrix;
int *s_matrix;
long *r_matrix;

volatile int col1, row1, col2, row2;

volatile byte status_c = 0;

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

  if(counter++ < 10 - progress / 10)
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
  if(status_c){
    calculateMatrix();
    printMatrix(row1, col2, r_matrix);
  }
}

void calculateMatrix(){
    r_matrix = malloc(row1 * col2 * sizeof(long));

    long *pr_matrix = r_matrix;
    
    for(int i = 0; i < row1 * col2 * sizeof(long); i++)
      *pr_matrix++=0;

    pr_matrix = r_matrix;

   int totalJob = row1 * col2 * row2;
   
   for(int i = 0; i < row1; i++)
      for(int j = 0; j < col2; j++){
        for(int k = 0; k < row2; k++){
          progress = i * j * k * 100 / totalJob;
          *pr_matrix += (long)f_matrix[i * col1 + k] * (long)s_matrix[k * col2 + j];
        }
        pr_matrix++;
      }

    Serial.println("TUJ SAM");
    status_c = 0;
}

void recieveEvent(int howMany){
    current_command = Wire.read();
    
    switch(current_command){
      case PING:
        Serial.println("PING");
        break;
      case MATRIX_FIRST_INFO:
        row1 = readInt();
        col1 = readInt();
        idRow = readInt();
        f_matrix = malloc(row1 * col1 * sizeof(int));
        bp_matrix = f_matrix;
        break;
      case MATRIX_FIRST_EL:
        putElementInMatrix(row1, col1); 
        break;
      case MATRIX_SECOND_INFO:
        row2 = readInt();
        col2 = readInt();
        s_matrix = malloc(row2 * col2 * sizeof(int));
        bp_matrix = s_matrix;
        break;
      case MATRIX_SECOND_EL:
        putElementInMatrix(row2, col2);
        break;
      case STATUS:
        Serial.println("STATUS");
        break;
      case TAKE_MATRIX_INFO:
        Serial.println("TAKE_MATRIX_INFO");
        break;
      case TAKE_MATRIX:
        Serial.println("TAKE_MATRIX");
        break;
      case UNKNOWN_C:
        Serial.println("UNKNOWN");
        break;
      }
}

void putElementInMatrix(int row, int col){
    *bp_matrix++ = readInt();
}

void printMatrix(int row, int col, int *p_matrix){
  Serial.println("Printing matrix");
  
  for(int i = 0; i < row; i++){
    for(int j = 0; j < col; j++){
      Serial.print(*p_matrix++);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void printMatrix(int row, int col, long *p_matrix){
  Serial.println("Printing matrix");
  
  for(int i = 0; i < row; i++){
    for(int j = 0; j < col; j++){
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
      case MATRIX_FIRST_EL:
        finishedResponse();
        printMatrix(row1, col1, f_matrix);
        break;
      case MATRIX_SECOND_EL:
        finishedResponse();
        printMatrix(row2, col2, s_matrix);
        status_c = 1;
        break;
      case STATUS:
        statusResponse();
        break;
      case TAKE_MATRIX_INFO:
        sendMatrixInfo();
        break;
      case TAKE_MATRIX:
        sendMatrix();
        break;
      case UNKNOWN_C:
        Serial.println("UNKNOWN");
        break;
  }
}

void sendMatrixInfo(){
  Wire.write(SEND_MATRIX_INFO);
  sendInt(row1);
  sendInt(idRow);
  elementsSent = 0;
}

void sendMatrix(){
  long *p_matrix = r_matrix + elementsSent;

  int elementsToSend = row1 * col2;

  int val = elementsToSend-elementsSent<8?elementsToSend-elementsSent:8;
  
  for(int i = 0; i < val; i++){
    sendLong(*p_matrix++);
    elementsSent++;
  }
}

void pingResponse(){
  Wire.write(PING);
}

void statusResponse(){
  if(status_c)
    Wire.write(WORKING);
  else{
    Wire.write(FINISHED);
    progress = 0;    
  }
}

void finishedResponse(){
  Wire.write(SUCCESFULLY_SENT);
}

int readInt(){
  return Wire.read() | Wire.read() << 8;
}

void sendInt(int number){
  Wire.write(number);
  Wire.write(number>>8);
}

void sendLong(long num){
  Wire.write(num >> 24);
  Wire.write(num>>16);
  Wire.write(num>>8);
  Wire.write(num);
}

long readLong(){
  long num = Wire.read();
  num = (num << 8)|Wire.read();
  num = (num << 8)|Wire.read();
  num = (num << 8)|Wire.read();
  
  return num;
}

