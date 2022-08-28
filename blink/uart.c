
// UART -> UKRADZIONE Z EXAMPLI z lekkimi modami modami dodany callback i funkcje 
// WYSYŁANIE pooling, ODBIERANIE przerwanie 

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#include "uart.h"

void(*UartRxInterruptCallback)(char ch);

void InitUart(void)
{
    uart_init(UART_ID, 2400);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);
    uart_set_hw_flow(UART_ID, false, false);
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    uart_set_fifo_enabled(UART_ID, false);
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
    irq_set_exclusive_handler(UART_IRQ, ReceiveUartTask);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(UART_ID, true, false);

}

void UartRxInterruptRegisterCallback(void* Callback)
{
    UartRxInterruptCallback = Callback;
}

// RX interrupt handler
void ReceiveUartTask(void){
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        UartRxInterruptCallback(ch);
    }
}

// TX functions
void UartPuts(char* str)
{
   	// while characters in string, transmit character
	while(*str)
	{
		UartPutc(*str++);
	}
}

void UartPutc(char c)
{
    uart_putc(UART_ID, c);
}