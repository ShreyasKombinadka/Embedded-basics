## Command-driven serial input

A simple command-driven system to receive commands over serial, decode them, and perform the required action on the microcontroller.

- Uses serial communication to send commands from a host (PC) to the microcontroller.
- Implements command-based control of an LED.
- Supports both single-action and continuous (time-based) commands.
- Uses non-blocking timing (`millis`) for repetitive actions.

### Available commands
- `ON`    → Turn LED ON
- `OFF`   → Turn LED OFF
- `BLINK` → Blink LED at a fixed interval

### Command format
- Commands must be enclosed between start and end delimiters.
- Format:  #COMMAND#
> Example:
#ON#
#OFF#
#BLINK#

### Notes
- Commands are case-insensitive.
- Invalid commands are ignored and reported over serial.
- The system continues running without blocking delays.