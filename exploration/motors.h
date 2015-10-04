#ifndef _MOTORS_H_
#define _MOTORS_H_

#define pinServoRoueGauche 9
#define pinServoRoueDroite 10

#define centerGauche 90
#define centerDroite 90

#define rotationEtalon 7150 // time to do 360°

void initServos();
void stopServos();
void avancer(int vitesse, int duree);
void tourner(float fraction);

void calibrationRepos();
void calibrationRotation();

#endif
