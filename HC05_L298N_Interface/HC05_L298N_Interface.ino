// L298N motor controller pin connections
int enA = 5;
int in1 = 6;
int in2 = 7;
int in3 = 8;
int in4 = 9;
int enB = 10;

// Right drive motor -> OUT1 and OUT2 on L298N
// Left drive motor -> OUT3 and OUT4 on L298N
// HC-05 Bluetooth TX -> RX on Arduino
// HC-05 Bluetooth RX -> TX on Arduino

boolean error = false; // Boolean for communication error checking

void setup() {
  Serial.begin(9600); // Required for Bluetooth communication

  // Defines all L298N control pins as OUTPUT
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);

  // Tells the motors to stop on power-up.
  mR_Set(0);
  mL_Set(0);
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
      drive(r, theta);
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

// Sets IN1 and IN2 such that the right motor is disabled.
void mR_Stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

// Sets IN1 and IN2 such that the right motor can run forwards.
void mR_For() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

// Sets IN1 and IN2 such that the right motor can run backwards.
void mR_Back() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

// Sets the speed and direction of the right motor. Accepts inputs from -255 (full speed backwards) to 255 (full speed forwards). An input of 0 results in a full stop.
void mR_Set(int dutyCycle) {
  if (dutyCycle == 0) {
    mR_Stop();
  } else if (dutyCycle > 0) {
    mR_For();
  } else {
    mR_Back();
  }
  analogWrite(enA, abs(dutyCycle));
}

// Sets IN3 and IN4 such that the left motor is disabled.
void mL_Stop() {
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

// Sets IN3 and IN4 such that the left motor can run forwards.
void mL_For() {
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

// Sets IN3 and IN4 such that the left motor can run backwards.
void mL_Back() {
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

// Sets the speed and direction of the left motor. Accepts inputs from -255 (full speed backwards) to 255 (full speed forwards). An input of 0 results in a full stop.
void mL_Set(int dutyCycle) {
  if (dutyCycle == 0) {
    mL_Stop();
  } else if (dutyCycle > 0) {
    mL_For();
  } else {
    mL_Back();
  }
  analogWrite(enB, abs(dutyCycle));
}

// Your code here. You can call mL_Set(int dutyCycle) and mR_Set(int dutyCycle) to set the motor speeds and directions. You can also use r and theta in your code. They reflect the joystick position.
// Code written here will execute: (1) Immediately when the joystick is pressed down, (2) Immediately when the joystick is released, (3) 10 times per second while the joystick is being pressed
void drive(int r, int theta) {
  
}
