add_executable(blink
        blink.c
        drives.c
        timers.c
        uart.c
        circ_buffer.c
        )

# pull in common dependencies
target_link_libraries(blink pico_stdlib)

# # rozumiem, że urat przez USB jako static library 
# # -> ramka standardowa 8n1 baud 115_200 
# pico_enable_stdio_usb(blink 1)
# pico_enable_stdio_uart(blink 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(blink)

# add url via pico_set_program_url
example_auto_set_url(blink)
