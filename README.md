# DOCUMENTATION

### ENUM LIST:


* PING
* STATUS
* WORKING
* FINISHED
* MATRIX_FIRST_INFO
* MATRIX_FIRST_EL
* MATRIX_SECOND_INFO
* MATRIX_SECOND_EL
* SUCCESSFULLY_SENT
* TAKE_MATRIX_INFO
* SEND_MATRIX_INFO
* TAKE_MATRIX
* REQUEST_STATUS_NUM
* REQUEST_JOB
* STEAL_JOB
_________
* #### PING
  * Send length - 1 byte
  * Send format: PING
  * Purpose: Check is there slave on given address
  * Answer length – 1 byte
  * Answer format: PING

* #### STATUS
  * Send length – 1 byte
  * Send format: STATUS
  * Purpose: Check if microcontroller on given address has finished his job
  * Answer length – 1 byte
  * Answer format: WORKING or FINISHED

* #### MATRIX_FIRST_INFO
  * Send length – 7 bytes
  * Send format: MATRIX_FIRST_INFO + (integer)row + (integer)column + (integer)id row
  * Purpose: Send size of the first matrix and which column is on beginning of the data chunk which slave is going to get later.

* #### MATRIX_FIRST_EL
  * Send length – 3 bytes
  * Send format: MATRIX_FIRST_EL + (integer)matrix element
  * Purpose: Send element from the first matrix.

* #### MATRIX_SECOND_INFO
  * Send length – 7 bytes
  * Send format: MATRIX_FIRST_INFO + (integer)row + (integer)column
  * Purpose: Send size of the second matrix.

* #### MATRIX_SECOND_EL
  * Send length – 3 bytes
  * Send format: MATRIX_SECOND_EL + (integer)matrix element
  * Purpose: Send element from the second matrix.

* #### SUCCESSFULLY_SENT
  * Send length – 1 byte
  * Send format: SUCCESFULLY_SENT
  * Purpose: Check if matrix has been successfully sent.
* #### TAKE_MATRIX_INFO
  * Send length – 1 byte
  * Send format: TAKE_MATRIX_INFO
  * Purpose: Get the size of receiving matrix and which rows will be sent for result matrix.
  * Answer length – 5 bytes
  * Answer format: SEND_MATRIX_INFO + (integer)row + (integer)id row

* #### TAKE_MATRIX
  * Send length – 1 byte
  * Send format: TAKE_MATRIX
  * Purpose: Take each element from the result matrix on slave and put it in the result matrix on master.
  * Answer length – 4 bytes
  * Answer format: (long)element of the result matrix

* #### REQUEST_STATUS_NUM
  * Send length – 1 byte
  * Send format: REQUEST_STATUS_NUM
  * Purpose: Get how many rows have left to calculate on the slave.
  * Answer length – 3 bytes
  * Answer format: REQUEST_STATUS_NUM + (integer)rows left

* #### REQUEST_JOB
  * Send length – 1 byte
  * Send format: REQUEST_JOB
  * Purpose: Get info about the matrix you are going to steal from slave.
  * Answer length – 5 bytes
  * Answer format: SEND_MATRIX_INFO + (integer)row + (integer)id row

* #### STEAL_JOB
  * Send length – 1 byte
  * Send format: STEAL_JOB
  * Purpose: Steal each element from the selected rows in the first matrix on slave and put it in the result matrix on master.
  * Answer length – 2 bytes
  * Answer format: (integer)element of the first matrix
_________
### COMMUNICATION EXAMPLE:

Master → Slave I | PING<br>
Master ← Slave I | PING<br>
Master → Slave II | PING<br>
Master ← Slave II | PING<br>
Master → Slave I | MATRIX_FIRST_INFO + INTEGER + INTEGER + INTEGER<br>
Master → Slave I | MATRIX_FIRST_EL + INTEGER<br>
Master → Slave I | MATRIX_FIRST_EL + INTEGER<br>
Master ← Slave I | SUCCESSFULLY_SENT<br>
Master → Slave I | MATRIX_SECOND_INFO + INTEGER + INTEGER<br>
Master → Slave I | MATRIX_SECOND_EL + INTEGER<br>
Master → Slave I | MATRIX_SECOND_EL + INTEGER<br>
Master → Slave I | MATRIX_SECOND_EL + INTEGER<br>
Master → Slave I | MATRIX_SECOND_EL + INTEGER<br>
Master ← Slave I | SUCCESSFULLY_SENT<br>
Master → Slave II | MATRIX_FIRST_INFO + INTEGER + INTEGER + INTEGER<br>
Master → Slave II | MATRIX_FIRST_EL + INTEGER<br>
Master → Slave II | MATRIX_FIRST_EL + INTEGER<br>
Master → Slave II | MATRIX_SECOND_INFO + INTEGER + INTEGER<br>
Master → Slave II | MATRIX_SECOND_EL + INTEGER<br>
Master → Slave II | MATRIX_SECOND_EL + INTEGER<br>
Master → Slave II | MATRIX_SECOND_EL + INTEGER<br>
Master → Slave II | MATRIX_SECOND_EL + INTEGER<br>
Master ← Slave II | SUCCESSFULLY_SENT<br>
Master → Slave I | STATUS<br>
Master ← Slave I | WORKING<br>
Master → Slave II | STATUS<br>
Master ← Slave II | WORKING<br>
Master → Slave I | STATUS<br>
Master ← Slave I | FINISHED<br>
Master → Slave I | TAKE_MATRIX_INFO<br>
Master ← Slave I | SEND_MATRIX_INFO<br>
Master ← Slave I | INTEGER + INTEGER<br>
Master → Slave II | REQUEST_STATUS_NUM<br>
Master ← Slave II | REQUEST_STATUS_NUM + INTEGER<br>
Master → Slave II | REQUEST_JOB<br>
Master ← Slave II | SEND_MATRIX_INFO + INTEGER + INTEGER<br>
Master → Slave II | STEAL_MATRIX<br>
Master ← Slave II | INTEGER + INTEGER<br>
Master → Slave I | MATRIX_FIRST_INFO + INTEGER + INTEGER + INTEGER<br>
Master → Slave I | MATRIX_FIRST_EL<br>
Master → Slave I | MATRIX_FIRST_EL<br>
Master ← Slave I | SUCCESSFULLY_SENT<br>
Master → Slave II| STATUS<br>
Master ← Slave II| WORKING<br>
Master → Slave I | STATUS<br>
Master ← Slave I | FINISHED<br>
Master → Slave I | TAKE_MATRIX_INFO<br>
Master ← Slave I | SEND_MATRIX_INFO<br>
Master ← Slave I | INTEGER + INTEGER<br>
Master → Slave II | REQUEST_STATUS_NUM<br>
Master ← Slave II | REQUEST_STATUS_NUM + INTEGER<br>
Master → Slave II | STATUS<br>
Master ← Slave II | FINISHED<br>
Master → Slave II | TAKE_MATRIX_INFO<br>
Master ← Slave II | SEND_MATRIX_INFO<br>
Master ← Slave II | INTEGER + INTEGER<br>
Master → Slave I | REQUEST_STATUS_NUM<br>
Master ← Slave I | REQUEST_STATUS_NUM + INTEGER<br>
Master → Slave I | PING<br>
Master → Slave II | PING<br>
