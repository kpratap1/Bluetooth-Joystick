// HC-05 Bluetooth TX -> RX on Arduino
// HC-05 Bluetooth RX -> TX on Arduino

boolean error = false; // Boolean for keeping track of communication errors

void setup() {
  Serial.begin(9600); // Required for Bluetooth communication
}

void loop() {  
  if(Serial.available() > 4 && !error) {   
    // Assigns the 5 bytes transmitted from Bluetooth per frame to variables.
    int checkbyte = Serial.read(); // Checkbyte is always 255.
    int r = Serial.read(); // Radius of the joystick touch. Ranges from 0 - 251. 126 is added to r if theta is larger than 180.
    int checkr = Serial.read(); // Sums with r to make 252.
    int theta = Serial.read(); // Theta of the joystick touch. Ranges from 0 - 180. 180 is subtracted from theta if theta is larger than 180.
    int checktheta = Serial.read(); // Sums with theta to make 252.
    
    if (!(checkbyte == 255 && checkr + r == 252 && checktheta + theta == 252)) { // Communication error checking
      error = true;
    } else {
      if (r > 125) {
        r = r - 126;  // reverts r to its displayed value on the app
        theta = theta + 180; // reverts theta to its displayed value on the app
      }
      
      // Your code here. r and theta are ints that reflect the joystick position.
      // Code written here will execute: (1) Immediately when the joystick is pressed down, (2) Immediately when the joystick is released, (3) 10 times per second while the joystick is being pressed

    }
  }

  if (Serial.available() > 0 && error) { // Dump Serial bytes until 255 is seen, indicating the start of the next Bluetooth frame.
    if (Serial.peek() != 255) {
      Serial.read();
    } else {
      error = false;
    }
  }
}