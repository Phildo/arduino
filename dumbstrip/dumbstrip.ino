#include <FastLED.h> //strip

//#define RING
#define STRIP
//#define BIGSTRIP

#define LUT
#define COMPRESS

#define STRIP_LED_PIN     2
#define STRIP_BRIGHTNESS  128 //0-128
#define FLUSH_TRIGGER "FLUSH"
#ifdef RING
  #define STRIP_NUM_LEDS 24
  #define STRIP_LED_TYPE WS2812
#elif defined STRIP
  #define STRIP_NUM_LEDS 100
  #define STRIP_LED_TYPE WS2811
#elif defined BIGSTRIP
  #define STRIP_NUM_LEDS 300
  #define STRIP_LED_TYPE WS2812
#endif
//#define BAUD_RATE 9600
//#define BAUD_RATE 115200
//#define BAUD_RATE 230400
//#define BAUD_RATE 460800
//#define BAUD_RATE 500000
//#define BAUD_RATE 576000
//#define BAUD_RATE 921600
#define BAUD_RATE 1000000
//#define BAUD_RATE 1152000
//#define BAUD_RATE 1500000
//#define BAUD_RATE 2000000
//#define BAUD_RATE 2500000
//#define BAUD_RATE 3000000
//#define BAUD_RATE 3500000
//#define BAUD_RATE 4000000

#define STRIP_COLOR_ORDER BRG

CRGB color_clear;
CRGB strip_leds[STRIP_NUM_LEDS];

int buff_n;
byte *buff;
int buff_i;
int trigger_i;

CRGB lut[16];

void initlut()
{
  lut[0x0] = CRGB(0,0,0); //clear
  lut[0x1] = CRGB(0,20,0); //dim blue
  lut[0x2] = CRGB(0,40,0); //weird yellow
  lut[0x3] = CRGB(0,60,0); //white
  lut[0x4] = CRGB(20,80,0); //clear
  lut[0x5] = CRGB(40,100,0); //clear
  lut[0x6] = CRGB(60,120,0); //clear
  lut[0x7] = CRGB(80,140,20); //clear
  lut[0x8] = CRGB(100,160,40); //clear
  lut[0x9] = CRGB(120,180,60); //dim blue
  lut[0xA] = CRGB(140,200,80); //weird yellow
  lut[0xB] = CRGB(160,220,100); //white
  lut[0xC] = CRGB(180,240,120); //clear
  lut[0xD] = CRGB(200,0,140); //clear
  lut[0xE] = CRGB(220,0,160); //clear
  lut[0xF] = CRGB(240,0,180); //clear
}

void inputToRGB()
{
  #ifdef LUT
    #ifdef COMPRESS
  for(int i = 0; i < STRIP_NUM_LEDS/2; i++)
  {
    strip_leds[i*2  ] = lut[buff[i]&0x0F];
    strip_leds[i*2+1] = lut[buff[i]>>4];
  }
    #else
  for(int i = 0; i < STRIP_NUM_LEDS; i++)
    strip_leds[i] = lut[buff[i]];
    #endif
  #else
  int b = 0;
  for(int i = 0; i < STRIP_NUM_LEDS; i++)
  {
    strip_leds[i] = CRGB(buff[b+0],buff[b+1],buff[b+2]);
    b += 3;
  }
  #endif
}

void writestuff()
{
  //String msg = "this is a test";
  //buff_n = msg.length();
  //msg.getBytes(out_bytes,buff_n+1);
  //out_bytes[buff_n] = '\0'; buff_n++;
  //Serial.write(out_bytes,buff_n);
}

void setup()
{
  delay(300); //power-up safety delay
  Serial.begin(BAUD_RATE);
  while(!Serial) { ; }

  initlut();

  #ifdef LUT
    #ifdef COMPRESS
  buff_n = STRIP_NUM_LEDS/2+strlen(FLUSH_TRIGGER);
    #else
  buff_n = STRIP_NUM_LEDS+strlen(FLUSH_TRIGGER);
    #endif
  #else
  buff_n = STRIP_NUM_LEDS*3+strlen(FLUSH_TRIGGER);
  #endif
  buff = (byte *)malloc(sizeof(byte)*buff_n+1);
  memset(buff,0,sizeof(byte)*buff_n+1);
  strcpy((char *)buff+(buff_n-strlen(FLUSH_TRIGGER)),FLUSH_TRIGGER); //actually unnecessary

  buff_i = 0;
  trigger_i = 0;

  for(int i = 0; i < STRIP_NUM_LEDS; i++) strip_leds[i] = lut[0];

  FastLED.addLeds<STRIP_LED_TYPE, STRIP_LED_PIN, STRIP_COLOR_ORDER>(strip_leds, STRIP_NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(STRIP_BRIGHTNESS);
  inputToRGB();
  FastLED.show();
}

void loop()
{
  while(Serial.available() > 0)
  {
    buff[buff_i++] = Serial.read();
    if(trigger_i && buff[buff_i-1] == FLUSH_TRIGGER[trigger_i])
    {
      trigger_i++;
      if(trigger_i == strlen(FLUSH_TRIGGER))
      {
        if(buff_i == buff_n)
        {
          inputToRGB();
          FastLED.show();
        }
        buff_i = 0;
        trigger_i = 0;
      }
    }
    else if(buff[buff_i-1] == FLUSH_TRIGGER[0]) trigger_i = 1;
    else                                        trigger_i = 0;

    if(buff_i == buff_n) buff_i = 0;
  }
}

