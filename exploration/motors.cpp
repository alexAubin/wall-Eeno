
#include "motors.h"

#include <Arduino.h>
#include <Servo.h>

Servo servoRoueGauche;  // create servo object to control a servo
Servo servoRoueDroite;  // create servo object to control a servo

void initServos()
{
  servoRoueGauche.attach(pinServoRoueGauche);
  servoRoueDroite.attach(pinServoRoueDroite);
}

void stopServos()
{
  servoRoueGauche.write(centerGauche);
  servoRoueDroite.write(centerDroite);
  servoRoueGauche.detach();
  servoRoueDroite.detach();
}

void avancer(int vitesse, int duree)
{
  initServos();
  servoRoueGauche.write(centerGauche+vitesse);
  servoRoueDroite.write(centerDroite-vitesse);
  delay(duree);
  stopServos();
}

void tourner(float fraction)
{
  int vitesse = 2.5 * fraction / abs(fraction);
  int duree = (int) (rotationEtalon * abs(fraction));

  initServos();
  servoRoueGauche.write(centerGauche+vitesse);
  servoRoueDroite.write(centerDroite+vitesse);
  delay(duree);
  stopServos();
}

void calibrationRepos()    { avancer(0,10000); }
void calibrationRotation() { tourner(4); delay(10000); }

