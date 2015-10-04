
#include "motors.h"
#include "sentry.h"
#include "exploration.h"

void setup() 
{ 
  
  Serial.begin(115200);
  
  initSentry();
  
  initExploration();

  Serial.println("Started");

}

void printTab()
{
    int* distanceTab = getDistanceTab();
   
    for (int i = 0 ; i < totalScanStep+1 ; i++)
    {
        if (i) Serial.print(";");
        Serial.print(distanceTab[i]);
    }
    Serial.println("");
}


void loop()
{
  Serial.println("Scanning"); Serial.flush();
  scan();
  Serial.println("Priting tab"); Serial.flush();
  printTab();
  delay(1000);
  Serial.println("Updating map"); Serial.flush();
  updateMapUsingSentryScan();
  Serial.println("Printing map"); Serial.flush();
  printMap();
  Serial.println("Done"); Serial.flush();
  delay(30000);
  
}


