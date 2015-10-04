#ifndef _SENTRY_H_
#define _SENTRY_H_

#define _PI_ 3.141592653

// Define pin mapping
#define SENTRY_TRIGGER_PIN 2
#define SENTRY_ECHO_PIN    3
#define SENTRY_SERVO_PIN   4

void initSentry();

// Returns the distance measured, in centimeters
long getDistance();

// Scan parameters
#define totalScanStep     10
#define scanAngleMin      20
#define scanAngleMax      140
#define delayBetweenSteps 300

void scan();
int* getDistanceTab();

#endif
