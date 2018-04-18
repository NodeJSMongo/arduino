// it will hold all the messages coming from the nodejs server
String inputString = "";

// whether the string received form nodejs is complete
// a string is considered complete by the carriage return '\r'
boolean stringComplete = false;

/**
 *
 * arduino board setup
 *
 */

void setup()
{
  // set the Baud Rate
  Serial.begin(115200);
  Serial.println("Hello World");
  delay(1000);
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
 void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a "\n" we detect the end of the string
    if (inChar == '\r') {
      stringComplete = true;
    }
  }
}
