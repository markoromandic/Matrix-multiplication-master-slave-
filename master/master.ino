#include <Wire.h>
#include <protokol.h>

int *f_matrix;
int *s_matrix;
long *r_matrix;

byte slaves[128];
byte numSlaves = 0;

byte finished = 0;

int col1, col2, row1, row2;

void setup() {
  Wire.begin();
  Serial.begin(115200);
}

void loop() {
  findSlaves();
  if(readMatrix()){
//    printMatrix(col1, row1, f_matrix);
//    printMatrix(col2, row2, s_matrix);
    seperateTasks();
    free(f_matrix);
//    printMatrix(row2, col2, s_matrix);
    free(s_matrix);
    buildResultMatrix();
    while(!finished){
      for(byte i = 0; i < 128; i++){
        if(slaves[i]){
          if(checkStatus(i)){
            getBackMatrix(i);
            finished = 1;
          }
          delay(10);
        }
      }
    }
    printMatrix(row1, col2, r_matrix);
  }

  delay(500);
}

bool readMatrix(){
  finished = 0;
  Serial.println("Enter height of first matrix ");
  while(!Serial.available());
  row1 = Serial.parseInt();
  Serial.print("Row: ");
  Serial.println(row1);
  Serial.println("Enter width of first matrix ");
  while(!Serial.available());
  col1 = Serial.parseInt();
  Serial.print("Col: ");
  Serial.println(col1);
  
  f_matrix = malloc(row1 * col1 * sizeof(int));

  Serial.println("Enter elements of the first matrix");

  int *element = f_matrix;
  
  for(int i = 0; i < row1; i++){
    Serial.print("Enter elements of row ");
    Serial.print(i + 1);
    Serial.println(" matrix");
    for(int j = 0; j < col1; j++){
       while(!Serial.available());
       *element++ = Serial.parseInt();
       Serial.print("i, j: ");
       Serial.print(i + 1);
       Serial.print(" ");
       Serial.println(j + 1);
    }
  }
  
  Serial.println("Enter height of second matrix ");
  while(!Serial.available());
  row2 = Serial.parseInt();
  Serial.print("Row: ");
  Serial.println(row2);
  Serial.println("Enter width of second matrix ");
  while(!Serial.available());
  col2 = Serial.parseInt();
  Serial.print("Col: ");
  Serial.println(col2);

  if(col1 != row2){
    Serial.println("Invalid sizes");
    return false;
  }

  s_matrix = malloc(row2 * col2 * sizeof(int));
  
  element = s_matrix;
  
  for(int i = 0; i < row2; i++){
    Serial.print("Enter elements of row ");
    Serial.print(i + 1);
    Serial.println(" matrix");
    for(int j = 0; j < col2; j++){
       while(!Serial.available());
       *element++ = Serial.parseInt();
       Serial.print("i, j: ");
       Serial.print(i + 1);
       Serial.print(" ");
       Serial.println(j + 1);
    }
  }

  return true;
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

bool seperateTasks(){
  int rows_per_slave = row1 / numSlaves;
  int *rows_to_send;
  int slaveNum = 0;
  for(byte i = 0; i < 128;  i++){
    if(slaves[i]){
      rows_to_send = f_matrix + slaveNum * rows_per_slave * col1;

      int rowNum = slaveNum * rows_per_slave;

      slaveNum++;
      
      if(slaveNum == numSlaves){
        rows_per_slave += row1 % numSlaves;
        if(!sendTask(i, rows_to_send, rows_per_slave, col1, rowNum))
          return false;
      }
      else if(!sendTask(i, rows_to_send, rows_per_slave, col1, rowNum))
        return false;

//    SENDING SECOND MATRIX
      sendTask(i, s_matrix, row2, col2);
    }
  }

  return true;
}

bool sendTask(byte address, int *p_matrix, int row, int col){
  Wire.beginTransmission(address);
  Wire.write(MATRIX_SECOND_INFO);

  sendInt(row);
  sendInt(col);

  Wire.endTransmission();
  
  for(int i = 0; i < row * col; i++){
    Wire.beginTransmission(address);
    Wire.write(MATRIX_SECOND_EL);
    sendInt(*p_matrix++);
    Wire.endTransmission();
  }
  
  Wire.requestFrom(address, 1);
    
  if(SUCCESFULLY_SENT == Wire.read())
    return true;
  return false;
}

bool sendTask(byte address, int *p_matrix, int row, int col, int idRow){
  Wire.beginTransmission(address);

  Wire.write(MATRIX_FIRST_INFO);
  
  sendInt(row);
  sendInt(col);
  sendInt(idRow);

  Wire.endTransmission();
  
  for(int i = 0; i < row * col; i++){
    Wire.beginTransmission(address);
    Wire.write(MATRIX_FIRST_EL);
    sendInt(*p_matrix++);
    Wire.endTransmission(); 
  }

  Wire.requestFrom(address, 1);
    
  if(SUCCESFULLY_SENT == Wire.read())
     return true; 
  return false;
}

void findSlaves(){
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
}

bool pingSlave(byte address){
  Wire.beginTransmission(address);
  Wire.write(PING);
  Wire.endTransmission();

  Wire.requestFrom(address, 1);

  if(Wire.read() == PING)
    return true;

  return false;
}

bool checkStatus(byte address){
  Wire.beginTransmission(address);
  Wire.write(STATUS);
  Wire.endTransmission();

  Wire.requestFrom(address, 1);

  byte id = Wire.read();

  if(id == WORKING){
    return false;
  }
  else if(id == FINISHED){
    return true;
  }
}

bool getBackMatrix(byte address){
  Wire.beginTransmission(address);
  Wire.write(TAKE_MATRIX_INFO);
  Wire.endTransmission();

  Wire.requestFrom(address, 5);

  if(!Wire.read() == SEND_MATRIX_INFO)
    return false;

  int row = readInt();
  int id_r = readInt();
  
  Wire.beginTransmission(address);
  Wire.write(TAKE_MATRIX);
  Wire.endTransmission();

  int bytesToRecieve = row * col2 * sizeof(long);

  int bytesReceieved = 0;

  long *p_matrix = r_matrix;
  
  p_matrix += id_r * col2;
  
  while(bytesToRecieve > bytesReceieved){
    int val = bytesToRecieve-bytesReceieved<32?bytesToRecieve-bytesReceieved:32;

    Wire.requestFrom(address, val);
    
    for(int i = 0; i < (val)/4; i++)
      *p_matrix++ = readLong();
      
    bytesReceieved += 32;
  }
  return true;
}

void buildResultMatrix(){
  r_matrix = malloc(row2 * col1 * sizeof(long));
  
  long *pr_matrix = r_matrix;
    
  for(int i = 0; i < row2 * col1 * sizeof(long); i++)
    *pr_matrix++=0;
}

void sendInt(int number){
  Wire.write(number);
  Wire.write(number>>8);
}

int readInt(){
  return Wire.read() | Wire.read() << 8;
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

