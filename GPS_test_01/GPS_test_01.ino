#include <SerialPort.h>
#include <UBLOX.h>
#include <Streaming.h>

UBLOX gps;

SerialPort<0,128,128> debug;

byte inByte;
float heading,distance;
int32_t latDest = 329770690;
int32_t lonDest = -967126270;
void setup(){
  debug.begin(115200);
  gps.init();
  debug<<"start\r\n";
}

void loop(){
  gps.Monitor();
  if (gps.newData == true){
    gps.newData = false;
    gps.Heading(&gps.data.vars.lat,&gps.data.vars.lon,&latDest,&lonDest,&heading);
    gps.Distance(&gps.data.vars.lat,&gps.data.vars.lon,&latDest,&lonDest,&distance);
    debug << gps.data.vars.gpsFix << ","<< gps.data.vars.lat <<","<< gps.data.vars.lon << "," << gps.data.vars.height << "," << heading << "," << distance <<"\r\n";
  }
}
