#include <UBLOX.h>

void UBLOX::init(void){
	GPSPort.begin(115200);
	GPSState=0;
	newData = false;
}

void UBLOX::Heading(int32_t *lat1, int32_t *lon1, int32_t *lat2, int32_t *lon2,float *bearing){
  dLon = ToRad((*lon2 - *lon1)/10000000.0);
  lat1_f = ToRad(*lat1/10000000.0);
  lat2_f= ToRad(*lat2/10000000.0);

  y = sin(dLon)*cos(lat2_f);
  x = cos(lat1_f)*sin(lat2_f)-sin(lat1_f)*cos(lat2_f)*cos(dLon);
  *bearing = ToDeg(atan2(y,x));
  if(*bearing < 0){
    *bearing += 360;
  }

}

void UBLOX::Distance(int32_t *lat1, int32_t *lon1, int32_t *lat2, int32_t *lon2,float *dist){
  dLon = ToRad((*lon2 - *lon1)/10000000.0);
  dLat = ToRad((*lat2 - *lat1)/10000000.0);
  lat2_f = ToRad((*lat2)/10000000.0);
  x = dLon*cos(lat2_f);
  *dist = sqrt(x*x + dLat*dLat)*6372795;
}

void UBLOX::Monitor(){
  if (GPSPort.available() > 100){
    while (GPSPort.available() > 0){
      switch (GPSState){
      case 0:
        inByte = GPSPort.read();
        if (inByte == 0xB5){
          GPSState = 1;
        }
        break;
      case 1:
        inByte = GPSPort.read();
        if (inByte == 0x62){
          GPSState = 2;
        }
        else{
          GPSState = 0;
        }
        break;
      case 2:
        inByte = GPSPort.read();
        if (inByte == 0x01){
          GPSState = 3;
        }
        else{
          GPSState = 0;
        }
        break;
      case 3:
        inByte = GPSPort.read();
        if (inByte == 0x02){
          GPSState = 4;
        }
        else{
          if (inByte == 0x12){
            GPSState = 5;
          }
          else{
            if(inByte == 0x03){
              GPSState = 6;
            }
            else{
              GPSState = 0;
            }
          }
        }
        break;
      case 4://POSLLH
        inByte = GPSPort.read();
        inByte = GPSPort.read();
        for(i=0;i<28;i++){
          inByte = GPSPort.read();
          data.buffer[i] = inByte;
        }
        GPSState = 0;
        break;
      case 5://VELNED
        inByte = GPSPort.read();
        inByte = GPSPort.read();
        for(i=0;i<4;i++){//iTOW
          inByte = GPSPort.read();
          data.buffer[i] = inByte;
        }
        for(i=28;i<60;i++){
          inByte = GPSPort.read();
          data.buffer[i] = inByte;
        }
        GPSState = 0;
        break;
      case 6://
        inByte = GPSPort.read();
        inByte = GPSPort.read();
        for(i=0;i<4;i++){//iTOW
          inByte = GPSPort.read();
          data.buffer[i] = inByte;
        }
        inByte = GPSPort.read();
        data.vars.gpsFix = inByte;
        GPSState = 0;
        break;
      }
    }
    newData = true;
  }
}