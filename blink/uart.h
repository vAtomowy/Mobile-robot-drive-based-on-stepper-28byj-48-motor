
/* uart.h */

// UART -> UKRADZIONE Z EXAMPLI z lekkimi modami dodany callback i funkcje 
// WYSYŁANIE pooling, ODBIERANIE przerwanie 

// _____________________SETTINGS_____________________>
#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define UART_TX_PIN 0
#define UART_RX_PIN 1
// _____________________SETTINGS_____________________|

// prototype function 
void InitUart(void); 
void ReceiveUartTask(void);
void UartPuts(char * str); 
void UartPutc(char c);

void UartRxInterruptRegisterCallback(void* Callback);
