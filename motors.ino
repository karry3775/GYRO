// Motor 1
#define ENA 3//17 // A3
#define IN2 12
#define IN1 11

// Motor 2
#define ENB 5 //16 // A2
#define IN3 10
#define IN4 9

// CW motor on
void turnLeftMotorCCW() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void turnRightMotorCW() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// CCW motor on
void turnLeftMotorCW() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void turnRightMotorCCW() {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}


// turn off
void turnLeftMotorOff() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void turnRightMotorOff() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void setSpeed(float s) {
  setLeftMotorSpeed(s);
  setRightMotorSpeed(s);
}

void setLeftMotorSpeed(float s) {
  if (s > 1) {
    analogWrite(ENA, s);
    turnLeftMotorCW();
  }
  else if (s < -1) {
    analogWrite(ENA, -s);
    turnLeftMotorCCW();
  }
  else {
    turnLeftMotorOff();
  }
}

void setRightMotorSpeed(float s) {
  if (s > 1) {
    analogWrite(ENB, s);
    turnRightMotorCW();
  }
  else if (s < -1) {
    analogWrite(ENB, -s);
    turnRightMotorCCW();
  }
  else {
    turnRightMotorOff();
  }
}

void setupMotors() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // turn off the motors at first
  turnLeftMotorOff();
  turnRightMotorOff();
}
