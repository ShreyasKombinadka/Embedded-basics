#include <Arduino.h>

const int LED = 12; // Pin where the led is connected

String cmd = "";      // received command
bool recv = false;    // Flag to indicate cmd being received
char buffer[10] = {}; // Buffer to store the received charecters till the complete message arrives

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
  static String cmd_prev = ""; // variable to store the previous cmd
  if (cmd != cmd_prev)
  {
    Serial.println(cmd);
    if (cmd == "ON") // Turn on the led
      digitalWrite(PIN, HIGH);
    else if (cmd == "OFF") // Turn off the led
      digitalWrite(PIN, LOW);
    else
      Serial.println("INVALID CMD"); // Any other cmd is invalid
    cmd_prev = cmd;                  // Assign the current cmd to previou cmd when the next cmd arrives
  }
}

void cmd_receiver() // The receiver function
{
  char cmd_r = 0;             // Variable to store received charecters
  if (Serial.available() > 0) // The number of bytes to read must be greater than 0
  {
    cmd_r = Serial.read();            // Assign current bytes varibale
    static int i = 0;                 // Integer for buffer indexing
    if (cmd_r == '#' && recv == true) // Terminate cmd receiving when the end delimeter '#' arrives
    {
      if (buffer[0] != 0) // Assign stored char from buffer to cmd only if its not a empty list
        cmd = buffer;
      recv = false;     // Reset transmission flag
      i = 0;            // Reset buffer index
      buffer[i] = '\0'; // Clear buffer by adding end symbol to index 0
    }
    else if (cmd_r == '#' && recv == false) // Start the receiving by setting the transmission flag when start delimeter arrivees
      recv = true;

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