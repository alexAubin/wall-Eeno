 
 import processing.serial.*;

 // ########
 // # Init #
 // ########

 // The serial port to real on 
 Serial myPort; 
 
 void setup () 
 {
   // Set window size
   size(1600, 800);        
 
   // List all the available serial ports
   println(Serial.list());
   
   // This assumes the right port is the first in the list
   myPort = new Serial(this, Serial.list()[0], 9600);
 
   // Don't generate a serialEvent() unless you get a newline character:
   myPort.bufferUntil('\n');
 
   // set inital background:
   background(0);
 }
 
 void draw () 
 {
    // Empty because everything happens in the serialEvent()
 }
 
 // ############################
 // # Read values from Arduino #
 // ############################
 
 // Store values of angle and distance for each position of the sensor
 int maxIndex = 0;
 float[] angles    = new float[400]; // Yup, it's pretty quick and dirty
 float[] distances = new float[400];
 
 // For each new line
 void serialEvent (Serial myPort) 
 {
   // get the ASCII string:
   String inString = myPort.readStringUntil('\n');
 
   // If string is null, abort function
   if (inString == null) return;
 
   // Trim off any whitespace:
   inString = trim(inString);
 
   // Parse values
   String[] values = split(inString,' ');
   int index = int(values[0]);
   float angle = float(values[1])+10; 
   float dist = float(values[2]);
   dist = map(dist, 0, 60, 0, 1); // Distance gets rescaled from [0,60] to [0,1]
 
   // Store values
   angles[index]    = angle;
   distances[index] = dist;
   maxIndex = max(maxIndex,index); 
 
   // Refresh display
   displayScan(index);
 }

 // ###################################
 // # Display values via the graphics #
 // ###################################
 
void displayScan(int currentIndex)
 {
    // Clear windows, set drawing settings
    clear();
    stroke(0,200,50);
    strokeWeight(4);
    
    // Start at middle bottom
    int previousX = width/2;
    int previousY = height;
    // For each value measured
    for (int i = 0 ; i < maxIndex ; i++)
    { 
       // Get associated angle and distance
       float angle     = angles[i];
       float dist = distances[i];
       // Convert to a position on the screen
       int newX = int(width*(0.5 + dist*cos(angle*3.1415/180)));
       int newY = int(height*(1-dist*sin(angle*3.1415/180)));
       // Draw line between previous and current point
       line(previousX, previousY,newX,newY);
       previousX = newX;
       previousY = newY;
    }
    
    // Draw line from middle-bottom to current measurement to indicate scan progress, radar-like
    float angle = angles[currentIndex];
    float dist  = distances[currentIndex];
    int X = int(width*(0.5 + dist*cos(angle*3.1415/180)));
    int Y = int(height*(1-dist*sin(angle*3.1415/180)));
    line(width/2, height, X, Y);
}

