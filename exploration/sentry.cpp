
#include "sentry.h"

#include <Arduino.h>
#include <Servo.h>

Servo sentryServo;

int distanceTab[totalScanStep+1];

void initSentry()
{
    // Init sentry servomotor
    sentryServo.attach(SENTRY_SERVO_PIN);
    sentryServo.write(70);

    // Init ultrason module
    pinMode(SENTRY_TRIGGER_PIN, OUTPUT);
    digitalWrite(SENTRY_TRIGGER_PIN, LOW);
    pinMode(SENTRY_ECHO_PIN, INPUT);
}

// Returns the distance measured, in centimeters
long getDistance()
{
    // Trigger emition
    digitalWrite(SENTRY_TRIGGER_PIN, HIGH);
    delayMicroseconds(100);
    digitalWrite(SENTRY_TRIGGER_PIN, LOW);

    // Read value from sensor
    long echoValue = pulseIn(SENTRY_ECHO_PIN, HIGH);

    // Convert echo value to distance in centimeteri
    long distance = echoValue / 58;
    if (distance == 5) distance = 3000;
    return distance;
}

void scan()
{
    sentryServo.write(70);
    for (int i = 0 ; i < totalScanStep ; i++)
    {

        // Convert step to angle
        // angle varies between 0, 10°, 20°, ... N°, N-10°, N-20°, ...0
        int angle = scanAngleMin + i * (scanAngleMax - scanAngleMin) / totalScanStep;

        // Write command to servo
        sentryServo.write(angle);

        // Wait for servo to get to position
        delay(delayBetweenSteps);

        // Get distance from sensor
        long dist = getDistance();

        // Write current index, angle and measured distance to serial
        /*
           Serial.print(indexPosition);
           Serial.print(" ");
           Serial.print(angle);
           Serial.print(" ");
           Serial.println(dist);
           */
        distanceTab[i] = dist;

        // Wait a bit and go to next iteration
        delay(100);
    }
    sentryServo.write(70);

}

int* getDistanceTab()
{
    return distanceTab;
}


