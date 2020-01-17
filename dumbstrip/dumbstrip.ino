#include <FastLED.h> //strip

#define RING
//#define STRIP

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

#define STRIP_COLOR_ORDER BRG

#define FLUSH_TRIGGER "FLUSH"

CRGB color_clear;
CRGB strip_leds[STRIP_NUM_LEDS];

int buff_n;
byte *buff;
int buff_i;
int trigger_i;

void inputToRGB()
{
  int b = 0;
  for(int i = 0; i < STRIP_NUM_LEDS; i++)
  {
    strip_leds[i] = CRGB(buff[b+0],buff[b+1],buff[b+2]);
    b += 3;
  }
  strip_leds[0] = CRGB(50,0,0);
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
  Serial.begin(9600);
  while(!Serial) { ; }

  buff_n = STRIP_NUM_LEDS*3+strlen(FLUSH_TRIGGER);
  buff = (byte *)malloc(sizeof(byte)*buff_n+1);
  memset(buff,0,sizeof(byte)*buff_n+1);
  strcpy(buff+(buff_n-strlen(FLUSH_TRIGGER)),FLUSH_TRIGGER);

  color_clear = CRGB(0x00,0x00,0x00);
  for(int i = 0; i < STRIP_NUM_LEDS; i++) strip_leds[i] = color_clear;

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
    if(buff[buff_i-1] == FLUSH_TRIGGER[trigger_i])
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

