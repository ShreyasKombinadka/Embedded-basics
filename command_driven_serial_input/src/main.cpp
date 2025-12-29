#include <Arduino.h>

const int LED = 12; // Pin where the led is connected

const int blinkDelay = 1000; // Blink duration for led(1 second)

String cmd = "";           // received command
bool cmd_received = false; // flag to indictae receiving of a new cmd

void led_ctrl(int PIN); // Function to control led
void cmd_receiver();    // The receiver function

void setup()
{
  Serial.begin(9600); // Baud rate for serial
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop()
{
  cmd_receiver(); // Function to control led
  led_ctrl(LED);  // The receiver function
}

void led_ctrl(int PIN) // Function to control led
{
  static bool led_state = false;    // Led state
  static unsigned long start_r = 0; // Start time holder for repeatative operations
  static unsigned long start_t = 0; // Start time holder for time limted operations
  static bool jmp = false;          // Flag to indicate jump to different operation
  static int op_return = 0;         // Jump operation

  bool cmd_used = 0;        // Local flag to indicate consumption of cmd
  if (cmd_received == true) // Check if any new cmd has arrived
  {
    cmd_used = true; // Set local cmd usage flag to clear the cmd received flag after using it
    jmp = false;     // If new cmd has arrived clear jump flag
  }

  // Operation selection
  static int op = 0;                    // Current operation
  static int op_prev = 0;               // previous operation
  if (jmp == true && cmd_used == false) // make the jump only if no new cmd has arrived
  {
    op = op_return; // Jump to the previous operation
  }

  else
  {
    if (cmd == "ON")
      op = 1; // ON opearation
    else if (cmd == "OFF")
      op = 2; // OFF opearation
    else if (cmd == "BLINK")
      op = 3; // BLINK opearation
    else if (cmd == "FLIP")
      op = 4; // FLIP operation
    else
    {
      op = -1;           // Unknown cmd
      if (op != op_prev) // Print error only if the cmd has chnaged
      {
        Serial.println("INVALID COMMAND");
        op_prev = op; // Assign current op to previous op for next cycle
      }
    }
  }

  // Led state control
  switch (op)
  {
  case 1: // ON opearation
    digitalWrite(PIN, HIGH);
    led_state = true;
    break;

  case 2: // OFF opearation
    digitalWrite(PIN, LOW);
    led_state = false;
    break;

  case 3:              // BLINK opearation
    if (op != op_prev) // Initial start time reset
    {
      start_r = 0;
      op_return = op; // Set the jump location
    }

    if (start_r == 0)
      start_r = millis(); // Set start time

    if (millis() - start_r > blinkDelay) // Change led state after the delay time
    {
      digitalWrite(PIN, led_state ? LOW : HIGH);
      led_state = !led_state; // Flip led state
      start_r = 0;            // Reset start time
      jmp = true;             // Set jump flag
    }
    break;

  case 4: // FLIP operation
    if (op != op_prev)
    {
      start_t = millis(); // Set start time
      digitalWrite(PIN, led_state ? LOW : HIGH);
      led_state = !led_state; // Flip led state
      op_return = op_prev;    // Set the jump location
      jmp = false;            // Clear jump flag
    }

    if (millis() - start_t > blinkDelay) // Change led state after the delay time
    {
      digitalWrite(PIN, led_state ? LOW : HIGH);
      led_state = !led_state; // Flip led state
      start_t = 0;            // Reset start time
      jmp = true;             // Set jump flag
    }
    break;

  default: // For unknown cmd
    digitalWrite(PIN, LOW);
    led_state = false;
    break;
  }

  if (op != op_prev) // If change in operation
  {
    if (jmp == true)
      Serial.println("Returning to previous operation...");
    else
      Serial.println(cmd);
    op_prev = op; // Assign the current op to previou op when the next cmd arrives

    if (cmd_used == true)
      cmd_received = false; // Clear cmd received flag after the use of cmd
  }
}

void cmd_receiver() // The receiver function
{
  char cmd_r = 0;             // Variable to store received charecters
  if (Serial.available() > 0) // The number of bytes to read must be greater than 0
  {
    cmd_r = Serial.read();            // Assign current bytes varibale
    static bool recv = false;         // Flag to indicate cmd being received
    static char buffer[10] = {};      // Buffer to store the received charecters till the complete message arrives
    static int i = 0;                 // Integer for buffer indexing
    if (cmd_r == '#' && recv == true) // Terminate cmd receiving when the end delimeter '#' arrives
    {

      cmd_received = true; // Set cmd received flag to indicate a new cmd has arrived
      if (buffer[0] != 0)  // Assign stored char from buffer to cmd only if its not a empty list
        cmd = buffer;

      recv = false;     // Reset transmission flag
      i = 0;            // Reset buffer index
      buffer[i] = '\0'; // Clear buffer by adding end symbol to index 0
    }
    else if (cmd_r == '#' && recv == false) // Start the receiving by setting the transmission flag when start delimeter arrivees
    {
      recv = true; // Set receive flag
    }

    else if (cmd_r != '#' && recv == true) // Store the data btwn the start and end delimeter which is the cmd
    {
      if (i < 8) // Only store if the buffer is not full
      {
        buffer[i] = toupper(cmd_r);
        buffer[i + 1] = '\0';
        i++;
      }
      else // Reset if the buffer is full
      {
        Serial.println("Buffer overflow");
        recv = false;
        i = 0;
        buffer[i] = '\0';
      }
    }
  }
}