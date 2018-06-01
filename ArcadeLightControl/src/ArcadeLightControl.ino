#include <Adafruit_PWMServoDriver.h>

const uint8_t PROGMEM gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);

const int maxRed = 0xff;
const int maxGreen = 0xff;
const int maxBlue = 0xd0;

void setup()
{
  pwm1.begin();
  pwm1.setPWMFreq(1000); 
  setColor(0,0);
  setColor(1,0);
  setColor(2,0);
  setColor(3,0);

  Serial.begin(9600);
}

int byteNum = 0;
int channel = 0;
int32_t rgb = 0;
  
void loop()
{
  while (!Serial.dtr()) ;    
  while(!Serial.available());
  while(Serial.available()){
    char c = Serial.read();
    uint32_t n = hex2int(c);
    if( byteNum == 0 ){
      channel = n;
    }
    else if( byteNum >= 2 ){
      rgb |= n<<((7-byteNum)*4);
    }
    if( c == '\r' || c == '\n'){
      byteNum = 0;
      rgb=0;
      channel=0;
    }
    else {
      byteNum++;
    }
    if( byteNum == 8 ){
      setColor(channel, rgb);
      Serial.print("OK ");
      Serial.print(channel);
      Serial.print(" ");
      Serial.println(rgb,HEX);
    }
  }
  
}

void setColor(uint8_t channel, uint32_t rgb)
{
  uint16_t r = (rgb>>16)&0xff;
  uint16_t g = (rgb>>8)&0xff;
  uint16_t b = (rgb>>0)&0xff;

  r = pgm_read_byte(&gamma8[r]);
  g = pgm_read_byte(&gamma8[g]);
  b = pgm_read_byte(&gamma8[b]);
  
  r=maxRed*(r<<4) / 0xff;
  g=maxGreen*(g<<4)/ 0xff;
  b=maxBlue*(b<<4) / 0xff;
  /*
  Serial.print(r);
  Serial.print(" ");
  Serial.print(g);
  Serial.print(" ");
  Serial.print(b);
  Serial.println(" ");
  */
  pwm1.setPin(channel*4+0,r,true);
  pwm1.setPin(channel*4+1,g,true);
  pwm1.setPin(channel*4+2,b,true);
}

int hex2int(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return -1;
}
