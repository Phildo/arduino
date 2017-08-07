byte in_byte_i = 0;
byte in_bytes[32];
byte out_bytes[32];

int pin_u;
int pin_d;
int pin_l;
int pin_r;

byte dir_u;
byte dir_d;
byte dir_l;
byte dir_r;

byte out;

void setup()
{
  Serial.begin(9600);
  while(!Serial) { ; }

  pin_u = 4;
  pin_d = 5;
  pin_l = 2;
  pin_r = 3;
  dir_u = 0;
  dir_d = 0;
  dir_l = 0;
  dir_r = 0;
  pinMode(pin_u, INPUT_PULLUP);
  pinMode(pin_d, INPUT_PULLUP);
  pinMode(pin_l, INPUT_PULLUP);
  pinMode(pin_r, INPUT_PULLUP);
}

void loop()
{
  dir_u = digitalRead(pin_u);
  dir_d = digitalRead(pin_d);
  dir_l = digitalRead(pin_l);
  dir_r = digitalRead(pin_r);

  if(Serial.available() > 0)
  {
    in_bytes[in_byte_i] = Serial.read();
    if(in_bytes[in_byte_i] == '\0')
    {
      in_byte_i = 0;
      int n_bytes = 0;

      //String msg = "this is a test";
      //n_bytes = msg.length();
      //msg.getBytes(out_bytes,n_bytes+1);

      out_bytes[0] = 1 << 4; //doesn't like sending only null bytes?
      if(!dir_u) out_bytes[0] |= 1 << 0;
      if(!dir_d) out_bytes[0] |= 1 << 1;
      if(!dir_l) out_bytes[0] |= 1 << 2;
      if(!dir_r) out_bytes[0] |= 1 << 3;
      n_bytes = 1;

/*
      out_bytes[0] = dir_u ? 'a' : 'b';
      out_bytes[1] = dir_d ? 'a' : 'b';
      out_bytes[2] = dir_l ? 'a' : 'b';
      out_bytes[3] = dir_r ? 'a' : 'b';
      n_bytes = 4;
*/

      out_bytes[n_bytes] = '\0'; n_bytes++;
      Serial.write(out_bytes,n_bytes);
    }
    else in_byte_i++;
  }
}


