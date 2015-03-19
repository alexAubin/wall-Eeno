// Sentry is using ultrason sensor HC-SR04

#include <Servo.h>

// #################
// # Configuration #
// #################

// Define pin mapping
int SENTRY_TRIGGER_PIN = 2;
int SENTRY_ECHO_PIN    = 4;
int SENTRY_SERVO_PIN   = 3;

// Scan parameters
#define totalScanStep 120
int scanAngleMin = 0;
int scanAngleMax = 160;
#define delayBetweenSteps 50

// #########
// # Setup #
// #########

// Create a servo
Servo sentryServo;

void setup()
{
  // Init ultrason module
  pinMode(SENTRY_TRIGGER_PIN, OUTPUT);
  digitalWrite(SENTRY_TRIGGER_PIN, LOW);
  pinMode(SENTRY_ECHO_PIN, INPUT);
  
  // Init sentry servomotor
  sentryServo.attach(3);
  sentryServo.write(0);

  // Start serial communication 
  Serial.begin(9600);
}

// ###############
// # Loop / scan #
// ###############

int i = 0;
void loop()
{  
  // Decomment and upload to deactivate sentry
  //return;
  
  // Get indexes of the scan step

    // 'index' varies between 0 and totalScanStep*2 - 1,
  int index = i%(totalScanStep*2);
    // 'indexPosition varies between 0 and totalStep, i.e.
    // follows 0, 1, 2, .. N, N-1, N-2, ... 0
  int indexPosition = (index <= totalScanStep ? index :  2 * totalScanStep - index);

  // Convert step to angle

    // angle varies between 0, 10°, 20°, ... N°, N-10°, N-20°, ...0
  int angle = (index<totalScanStep ? index : 2 * totalScanStep - index) * (scanAngleMax - scanAngleMin) / totalScanStep + scanAngleMin;
  
  // Write command to servo
  sentryServo.write(angle);
  
  // Wait for servo to get to position
  delay(delayBetweenSteps);
 
  // Get distance from sensor
  long dist = getDistance();
  
  // Write current index, angle and measured distance to serial
  Serial.print(indexPosition);
  Serial.print(" ");
  Serial.print(angle);
  Serial.print(" ");
  Serial.println(dist);

  // Wait a bit and go to next iteration
  delay(10);
  i++;
}

// Returns the distance measured, in centimeters
long getDistance()
{
  // Trigger emition
  digitalWrite(SENTRY_TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SENTRY_TRIGGER_PIN, LOW);
  
  // Read value from sensor
  long echoValue = pulseIn(SENTRY_ECHO_PIN, HIGH);
  
  // Convert echo value to distance in centimeter
  return echoValue/58;
}
