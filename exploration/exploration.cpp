
#include "sentry.h"
#include "exploration.h"
#include <Arduino.h>

signed char explorationMap[mapSize];


void initExploration()
{
    for (int c = 0 ; c < mapSize ; c++) explorationMap[c] = 0;
}

void printMap()
{
    for (int x = 0 ; x < mapWidth ; x++)
    {
        for (int y = 0 ; y < mapWidth ; y++)
        {
            if (explorationMap[x+y*mapWidth] == 0)      Serial.print('=');
            else if (explorationMap[x+y*mapWidth] <  0) Serial.print('.');
            else                                        Serial.print('X');
        }
        Serial.println("");
    }
    Serial.flush();
}

void updateMapUsingSentryScan()
{
    int* distanceTab = getDistanceTab();

    for (int i = 0 ; i < totalScanStep ; i++)
    {
        float theta = scanAngleMin + i * (scanAngleMax - scanAngleMin) / totalScanStep;
        Serial.println(theta); Serial.flush();
        theta *= _PI_ / 180;

        float dTheta = sentryConeSize;

        int d = distanceTab[i] / mapScale;
        if (d > dSaturation) d = dSaturation;

        updateTilesInsideCone(mapWidth/2,0,theta,dTheta,d);

    }
}

void updateTilesInsideCone(short int x_observer, short int y_observer, float theta, float dTheta, float d)
{
    signed char rotationMatrix[4] = {0, 0, 0, 0};

    // This first step transforms the problem into one where theta is close to positive x axis

    // Theta is close to positive x axis
    if ((theta < 0.5 * _PI_  - dTheta) || (theta > 1.5 * _PI_ + dTheta))
    {
        rotationMatrix[0] = 1; rotationMatrix[1] = 0;
        rotationMatrix[2] = 0; rotationMatrix[3] = 1;
    }
    // Theta is close to negative x axis
    else if ((theta > 0.5 * _PI_ + dTheta) && (theta < 1.5 * _PI_ - dTheta))
    {
        rotationMatrix[0] = -1; rotationMatrix[1] = 0;
        rotationMatrix[2] = 0;  rotationMatrix[3] = -1;
        theta -= _PI_;
    }
    // Theta is close to positive y axis
    else if ((theta > 0.5 * _PI_ - dTheta) && (theta < 0.5 * _PI_ + dTheta))
    {
        rotationMatrix[0] = 0; rotationMatrix[1] = -1;
        rotationMatrix[2] = 1; rotationMatrix[3] = 0;
        theta -= _PI_ / 2;
    }
    // Theta is close to negative y axis
    else if ((theta > 1.5 * _PI_ - dTheta) && (theta < 1.5 * _PI_ + dTheta))
    {
        rotationMatrix[0] = 0;  rotationMatrix[1] = 1;
        rotationMatrix[2] = -1; rotationMatrix[3] = 0;
        theta -= _PI_ / 2;
    }

    // This second step runs through the tiles of the cone

    float tanMinus = tan(theta - dTheta) - 0.0001;
    float tanPlus  = tan(theta + dTheta) + 0.0001;

    short int dSquareReal =   d   *   d;
    short int dSquare     = (d+1.1) * (d+1.1);

    // Scan x from 0 to d
    for (char x = 0 ; x <= d ; x++)
    {
        // For a given x, scan the y axis for tiles inside the cone
        // and with (x,y) distance to origin lower than d.

        // Constrain from the cone
        float yMin = x * tanMinus;
        float yMax = x * tanPlus ;

        short int xSquare    = x * x;

        for (short int y = yMin ; y <= yMax ; y++)
        {
            short int ySquare = y * y;
            short int currentDSquare = xSquare + ySquare;

            // Constrain from the distance
            if (currentDSquare > dSquare) continue;

            char x_map = x_observer + rotationMatrix[0] * x + rotationMatrix[1] * y;
            char y_map = y_observer + rotationMatrix[2] * x + rotationMatrix[3] * y;

            bool isObstacle = (currentDSquare - dSquareReal) > 0 ? true : false;

            updateMapTile(x_map, y_map, isObstacle);
        }
    }
}

void updateMapTile(short int x, short int y, bool isObstacle)
{
    if ((x < 0) || (x >= mapWidth)) return;
    if ((y < 0) || (y >= mapWidth)) return;

    signed char v = explorationMap[x+y*mapWidth];
    if (isObstacle) explorationMap[x+y*mapWidth] += (signed char) (( 127 - v) * 0.05);
    else            explorationMap[x+y*mapWidth] += (signed char) ((-127 - v) * 0.05);
}

