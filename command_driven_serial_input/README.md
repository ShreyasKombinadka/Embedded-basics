## Command-driven serial input

A simple command-driven system that receives commands over serial, decodes them, and controls an LED accordingly.  
Designed to work without blocking delays and to handle both continuous and time-limited operations.

- Uses serial communication to receive commands from a host (PC).
- Implements command-based LED control.
- Supports continuous and time-based commands.
- Uses non-blocking timing (`millis`) for all timing-related operations.
- Handles command reception and execution independently.

### Available commands
- `ON`    → Turn LED ON continuously  
- `OFF`   → Turn LED OFF  
- `BLINK` → Blink LED continuously at a fixed interval  
- `FLIP`  → Toggle LED state once, then return to the previous operation  

### Command format
- Commands must be enclosed between start and end delimiters.
- Format: `#COMMAND#`

> Examples : 
#ON#
#OFF#
#BLINK#
#FLIP#

### Notes
- Commands are case-insensitive.
- Invalid commands are detected and reported over serial.
- Timing is fully non-blocking; the main loop keeps running.
- `FLIP` is a temporary operation and automatically returns to the previous mode.
- Command execution is edge-based (new commands are consumed once).