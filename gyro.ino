
#include <AutoPID.h>-
#define PITCH_OFFSET 150


// PWM Deadband values for both directions
const double PWM_FORWARD_START = 77;  //128; // 127
const double PWM_FORWARD_MAX = 255;
const double PWM_REVERSE_START = -PWM_FORWARD_START;
const double PWM_REVERSE_MAX = -PWM_FORWARD_MAX;


#define OUTPUT_MIN PWM_REVERSE_MAX
#define OUTPUT_MAX PWM_FORWARD_MAX

#define SETPOINT -0.0//-1.5;


#define MIN_PITCH 1.25 // degress
#define MAX_PITCH 30 // degrees

#define MIN_PITCH_FORWARD MIN_PITCH + SETPOINT  
#define MIN_PITCH_REVERSE -MIN_PITCH + SETPOINT
#define MAX_PITCH_FORWARD MAX_PITCH + SETPOINT
#define MAX_PITCH_REVERSE -MAX_PITCH + SETPOINT

#define MIN_INIT_GYRO_READS 100

double pitch = 0;
double pwm = 0.0;

double numGYROreads = 0;

// Gains
double Kp = 50;// 28; //25;
double Kd = 0; //800; // 60
double Ki = 0;// 10; //30;
double setpoint = SETPOINT;

// Create the constructor in accordance with documentation here
//https://ryand.io/AutoPID/
AutoPID balance(&pitch, &setpoint, &pwm, OUTPUT_MIN, OUTPUT_MAX, Kp, Ki, Kd);

struct Attitude {
  double yaw = 0.0;
  double pitch = 0.0;
  double roll = 0.0;

  void print() {
    Serial.print("Yaw: \t");
    Serial.print(yaw);
    Serial.print("\t");
    Serial.print(pitch);
    Serial.print("\t");
    Serial.println(roll);
  }
};


void setup() {
  Serial.begin(115200);
  setupMotors();
  setupMPU6050();
  balance.setTimeStep(10);
}

void basicControl(float p) {
  // Get pitch
  if (p > 0) {
    Serial.print(p);
    Serial.print("\t");
    Serial.println(p + PITCH_OFFSET);
    setSpeed(p + PITCH_OFFSET);
  }
  else {
    Serial.print(p);
    Serial.print("\t");
    Serial.println(p - PITCH_OFFSET);
    setSpeed(p - PITCH_OFFSET);
  }
}


// Lets keep this for now
float clampCorrection(float correction) {
  if (correction > 500) { return 500; }
  else if (correction < -500) { return -500; }
  return correction;
}

void loop() {
  Attitude attitude;
  int ret_val = updateAttitude(attitude);
  if (ret_val == 0) {
    numGYROreads += 1;
    if (numGYROreads < MIN_INIT_GYRO_READS) {
      return;
    }
    // Update pitch
    const double pitchRead = attitude.pitch;
    Serial.print("Pitch read directly from attitude: ");
    Serial.println(pitchRead);
    pitch = pitchRead;    // What is this piece of code doing ????
    balance.run();
  }

  // Else keep doing whatever we should
  // Adjust for deadband in both directions
  double outputSpeed = pwm;
  if (pitch > 0) {
    // Forward direction
    if (pitch > MIN_PITCH_FORWARD and pitch < MAX_PITCH_FORWARD) {
      outputSpeed = outputSpeed > PWM_REVERSE_START ? PWM_REVERSE_START : outputSpeed;
    }
    else {
      //      outputSpeed = PWM_FORWARD_MAX;  
      outputSpeed = 0;
    }
  }
  else {
    // Reverse direction
    if (pitch < MIN_PITCH_REVERSE and pitch > MAX_PITCH_REVERSE) {
      outputSpeed = outputSpeed < PWM_FORWARD_START ? PWM_FORWARD_START : outputSpeed;
    }
    else {
      //      outputSpeed = PWM_REVERSE_MAX; 
      outputSpeed = 0;
    }
  }
  //
  //  Serial.print("Pitch: \t");
  //  Serial.print(pitch);
  //  Serial.print("\tGenerated PWM: \t");
  //  Serial.print(pwm);
  //  Serial.print("\tSetpoint: \t");
  //  Serial.print(setpoint);
  //  Serial.print("\t: outputSpeed: \t");
  //  Serial.println(outputSpeed);

    // Cause of how I have wired my motors
  setSpeed(-outputSpeed);
}

void loopMotorTest() {
  // just run the motor I guess:
  setLeftMotorSpeed(255);
  setRightMotorSpeed(1000);
}
