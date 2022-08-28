
/* uart.c */

#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#include "uart.h"

static int chars_rxed = 0;

void InitUart(void){ 

    uart_init(UART_ID, 1200);                                     // slow baud 
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);               // config gpio
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);  // finally baud  
    uart_set_hw_flow(UART_ID, false, false);                      // flow control off
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);       // frame format
    uart_set_fifo_enabled(UART_ID, false);                        // FIFO disable
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;      // handler from receive interrupt 
    irq_set_exclusive_handler(UART_IRQ, ReceiveUartData);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(UART_ID, true, false);                   // enable receive interrupt 
}

// RX interrupt handler
void ReceiveUartData(void) {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        // Can we send it back?
        if (uart_is_writable(UART_ID)) {
            // Change it slightly first!
            ch++;
            uart_putc(UART_ID, ch);
        }
        chars_rxed++;
    }
}

// TX functions
void UartPuts(char* str)
{
    uart_puts(UART_ID, str);
}

void UartPutc(char c)
{
    uart_putc(UART_ID, c);
}