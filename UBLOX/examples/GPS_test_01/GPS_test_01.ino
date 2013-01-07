#include <UBLOX.h>

UBLOX gps;

#define debug Serial

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
    debug.print(gps.data.vars.gpsFix);
    debug.print(",");
    debug.print(gps.data.vars.lat);
    debug.print(",");
    debug.print(gps.data.vars.lon);
    debug.print(",");
    debug.print(gps.data.vars.height);
    debug.print(",");
    debug.print(heading);
    debug.print(",");
    debug.println(distance);
  }
}
