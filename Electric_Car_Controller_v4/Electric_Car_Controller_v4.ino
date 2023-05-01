#include <Servo.h>

// L298N motor controller pin connections
int enA = 3;
int in1 = 4;
int in2 = 5;

// Drive motor -> OUT1 and OUT2 on L298N
// HC-05 Bluetooth TX -> RX on Arduino
// HC-05 Bluetooth RX -> TX on Arduino

// Initiates the both steering servos
Servo left;
Servo right;

// Straight steering position for each servo. 
int right0 = 90;
int left0 = 90;

// left and right servo pin connections
int rightServoPWM = 9;
int leftServoPWM = 10;

boolean error = false; // Boolean for communication error checking

void setup() {
  Serial.begin(9600); // Required for Bluetooth communication

  // Defines all L298N control pins as OUTPUT
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // Sets both servos to steer straight ahead. 
  right.attach(rightServoPWM);
  right.write(right0);
  left.attach(leftServoPWM);
  left.write(left0);

  // Tells the motor to stop on power-up.
  m_Set(0);
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

      if (r != 0) { // User inputs a command to move the car
        int steerAngle; // represents the deviation from straight steering in servo units
        
        if (theta >= 0 && theta <= 180) { // forwards
          m_Set(round(r*2.04)); // converts r (0-125) to dutyCycle (0-255)
          steerAngle = round((90 - theta)/5); // Maps theta to steering angle. Steering angle is proportional to the difference between theta and 90 (straight steering) when going forwards.
        } else { // backwards
          m_Set(-round(r*2.04)); // converts r (0-125) to dutyCycle (0-255). The negative sign indicates backwards.
          steerAngle = round((theta - 270)/5); // Maps theta to steering angle. Steering angle is proportional to the difference between theta and 270 (straight steering) when going backwards.
        }
        
        // Sets the servo to the steering angle
        right.write(right0 + steerAngle);      
        left.write(left0 + steerAngle);  
      } else { // No user inputs this loop.
        m_Set(0);
      }
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

// Sets IN1 and IN2 such that the drive motor is disabled.
void m_Stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

// Sets IN1 and IN2 such that the drive motor can run forwards.
void m_For() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

// Sets IN1 and IN2 such that the drive motor can run backwards.
void m_Back() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

// Sets the speed and direction of the drive motor. Accepts inputs from -255 (full speed backwards) to 255 (full speed forwards). An input of 0 results in a full stop.
void m_Set(int dutyCycle) {
  if (dutyCycle == 0) {
    m_Stop();
  } else if (dutyCycle > 0) {
    m_For();
  } else {
    m_Back();
  }
  analogWrite(enA, abs(dutyCycle));
}