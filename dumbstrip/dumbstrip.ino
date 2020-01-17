#include <FastLED.h> //strip

#define RING
//#define STRIP

#define FLUSH_TRIGGER "FLUSH"
#ifdef RING
  #define STRIP_LED_PIN     2
  #define STRIP_NUM_LEDS    24
  #define STRIP_BRIGHTNESS  32 //0-128
  #define STRIP_LED_TYPE    WS2812
#elif defined STRIP
  #define STRIP_LED_PIN     3
  #define STRIP_NUM_LEDS    100
  #define STRIP_BRIGHTNESS  128 //0-128
  #define STRIP_LED_TYPE    WS2811
#endif
#define BAUD_RATE 115200

#define STRIP_COLOR_ORDER BRG

CRGB color_clear;
CRGB strip_leds[STRIP_NUM_LEDS];

int buff_n;
byte *buff;
int buff_i;
int trigger_i;

int flag_i;

void inputToRGB()
{
  int b = 0;
  for(int i = 0; i < STRIP_NUM_LEDS; i++)
  {
    strip_leds[i] = CRGB(buff[b+0],buff[b+1],buff[b+2]);
    b += 3;
  }
}

void flagRGB()
{
  strip_leds[flag_i] = CRGB(0,0,0);
  flag_i++;
  if(flag_i == STRIP_NUM_LEDS) flag_i = 0;
  strip_leds[flag_i] = CRGB(50,0,0);
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

  buff_n = STRIP_NUM_LEDS*3+strlen(FLUSH_TRIGGER);
  buff = (byte *)malloc(sizeof(byte)*buff_n+1);
  memset(buff,0,sizeof(byte)*buff_n+1);
  strcpy(buff+(buff_n-strlen(FLUSH_TRIGGER)),FLUSH_TRIGGER);

  buff_i = 0;
  trigger_i = 0;
  flag_i = 0;

  color_clear = CRGB(0x00,0x00,0x00);
  for(int i = 0; i < STRIP_NUM_LEDS; i++) strip_leds[i] = color_clear;

  FastLED.addLeds<STRIP_LED_TYPE, STRIP_LED_PIN, STRIP_COLOR_ORDER>(strip_leds, STRIP_NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(STRIP_BRIGHTNESS);
  inputToRGB();
  FastLED.show();
}

void loop()
{
  int got_something = 0;
  while(Serial.available() > 0)
  {
    got_something = 1;
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

