#ifndef _EXPLORATION_H_
#define _EXPLORATION_H_

#define _PI_ 3.141592653

void initExploration();
void printMap();
void updateMapUsingSentryScan();
void updateTilesInsideCone(short int x_observer, short int y_observer, float theta, float dTheta, float d);
void updateMapTile(short int x, short int y, bool isObstacle);

#define mapWidth 20
#define mapSize  (mapWidth * mapWidth)

#define sentryConeSize (_PI_ / 30.0)
#define mapScale 3
#define dSaturation 20

#endif
