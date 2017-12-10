typedef bool(*callback_t)(void*);

typedef enum commands_
{
  PING,
  STATUS,
  MATRIX_FIRST_INFO,
  MATRIX_FIRST,
  MATRIX_FIRST_EL,
  MATRIX_SECOND_INFO,
  MATRIX_SECOND,
  MATRIX_SECOND_EL,
  SUCCESFULLY_SENT,
  WORKING,
  FINISHED,
  TAKE_MATRIX_INFO,
  TAKE_MATRIX,
  SEND_MATRIX,
  SEND_MATRIX_INFO,
  UNKNOWN_C,
}commands;


typedef struct prot{
  String command;
  int numParam;
  String response;
  callback_t function;
}protokol;
