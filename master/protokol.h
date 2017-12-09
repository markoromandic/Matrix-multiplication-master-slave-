typedef bool(*callback_t)(void*);

typedef enum commands_
{
  PING,
  STATUS,
  MATRIX_SECOND,
  UNKNOWN_C,
}commands;

typedef struct prot{
  String command;
  int numBytesSend;
  
  String response;
  int numBytesGet;
  
  callback_t function;
}protokol;

