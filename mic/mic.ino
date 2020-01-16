#define MICS_N 3
#define SMOOTH 1000

//btns
#define MIC0_PIN 4
#define LED0_PIN 9
#define MIC1_PIN 3
#define LED1_PIN 8
#define MIC2_PIN 10
#define LED2_PIN 11

#define MAX_LONG 4294967295

struct mic
{
  int id;
  int mic_pin;
  int led_pin;

  int mic_out;
  unsigned long mic_t;
  int smooth_mic_out;
  unsigned long smooth_mic_t;
};

mic mics[MICS_N];

void setup_mic(int id, int m_pin, int l_pin, mic *m)
{
  pinMode(m_pin,INPUT);
  digitalWrite(m_pin,0);
  pinMode(l_pin,OUTPUT);
  digitalWrite(l_pin,0);

  m->id = id;
  m->mic_pin = m_pin;
  m->led_pin = l_pin;
  m->mic_out = 0;
  m->mic_t = 0;
  m->smooth_mic_out = 0;
  m->smooth_mic_t = 0;
}
void loop_mic(mic *m)
{
  if(digitalRead(m->mic_pin))
  {
    if(!m->mic_out)
    {
      Serial.print("(");
      Serial.print(m->id);
      Serial.print(")");
      Serial.print("off:");
      Serial.print(m->mic_t);
      Serial.println();
      m->mic_t = 0;
    }
    m->mic_out = 1;
    if(!m->smooth_mic_out)
    {
      Serial.print("(");
      Serial.print(m->id);
      Serial.print(")");
      Serial.print("soff:");
      Serial.print(m->smooth_mic_t);
      Serial.println();
      m->smooth_mic_t = 0;
    }
    m->smooth_mic_out = 1;
  }
  else
  {
    if(m->mic_out)
    {
      Serial.print("(");
      Serial.print(m->id);
      Serial.print(")");
      Serial.print("on:");
      Serial.print(m->mic_t);
      Serial.println();
      m->mic_t = 0;
    }
    m->mic_out = 0;
    if(m->mic_t > SMOOTH)
    {
      if(m->smooth_mic_out)
      {
        Serial.print("(");
        Serial.print(m->id);
        Serial.print(")");
        Serial.print("son:");
        Serial.print(m->smooth_mic_t);
        Serial.println();
        m->smooth_mic_t = 0;
      }
      m->smooth_mic_out = 0;
    }
  }

  digitalWrite(m->led_pin,m->smooth_mic_out);

  if(m->mic_t        < MAX_LONG) m->mic_t++;
  if(m->smooth_mic_t < MAX_LONG) m->smooth_mic_t++;
}

void setup()
{
  delay(300); //power-up safety delay
  Serial.begin(9600);

  int i = 0;
  //setup_mic(i,MIC0_PIN,LED0_PIN,&mics[i]); i++;
  //setup_mic(i,MIC1_PIN,LED1_PIN,&mics[i]); i++;
  setup_mic(i,MIC2_PIN,LED2_PIN,&mics[i]); i++;
}

void loop()
{
  for(int i = 0; i < MICS_N; i++) loop_mic(&mics[i]);
}

