#define SUMD_HEAD 0xa8
#define PB 7
#define Power_On 12
#define PPM_CHANNELS 16

//Global Variables
bool ARM = 0;
uint32_t sOutLast = 0;
static uint16_t CRC16_value;
static uint16_t PPM_Arm[PPM_CHANNELS] = { 512, 512, 512, 512, 512, 512, 512, 512 , 512, 512, 512, 512, 512, 512, 512, 512 };
static uint16_t PPM_Disarm[PPM_CHANNELS] = { 512, 512, 512, 512, 512, 512, 512, 512 , 512, 512, 512, 512, 512, 512, 512, 512 };
static uint16_t PPM_Straight[PPM_CHANNELS] = { 512, 512, 512, 512, 512, 512, 512, 512 , 512, 512, 512, 512, 512, 512, 512, 512 };

//Function Prototypes
void sendSUMDFrame(uint8_t, uint16_t);
void sumdWriteCRC(uint8_t);
void CRC16_add(uint8_t);
inline void CRC16_reset(void);
uint16_t servoBits2Us(uint16_t);



void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Power_On, OUTPUT);
  pinMode(PB, INPUT_PULLUP);
  Serial.begin(115200);
  digitalWrite(Power_On, HIGH);
}


void loop() {
  while(digitalRead(PB)){
  }
  ARM = !ARM;
  digitalWrite(LED_BUILTIN, ARM);
  if(ARM) {
    sendSUMDFrame(0, PPM_Arm );
    //
    //Reset CRC?
    //Disarm
  }
  
  delay(500);
}


void sendSUMDFrame(uint8_t failsafe, uint16_t PPM[16] )
{
  uint32_t now = micros();
  while ((now - sOutLast) < 10000) {
    now = micros();
  }
  
  sOutLast = now;
  CRC16_reset();
  sumdWriteCRC(SUMD_HEAD);
  sumdWriteCRC(failsafe ? 0x81 : 0x01);
  sumdWriteCRC(16);
  for (uint8_t i = 0; i < 16; i++) {
    uint16_t val = servoBits2Us(PPM[i]) << 3;
    sumdWriteCRC(val >> 8);
    sumdWriteCRC(val & 0xff);
  }
  Serial.write(CRC16_value >> 8);
  Serial.write(CRC16_value & 0xff);
}

void sumdWriteCRC(uint8_t c)
{
  Serial.write(c);
  CRC16_add(c);
}

void CRC16_add(uint8_t c) // CCITT polynome
{
  uint8_t i;
  CRC16_value ^= (uint16_t)c << 8;
  for (i = 0; i < 8; i++) {
    if (CRC16_value & 0x8000) {
      CRC16_value = (CRC16_value << 1) ^ 0x1021;
    } else {
      CRC16_value = (CRC16_value << 1);
    }
  }
}

inline void CRC16_reset(void)
{
  CRC16_value = 0;
}

uint16_t servoBits2Us(uint16_t x)
{
  uint16_t ret;

  if (x < 12) {
    ret = 808 + x * 16;
  } else if (x < 1012) {
    ret = x + 988;
  } else if (x < 1024) {
    ret = 2000 + (x - 1011) * 16;
  } else {
    ret = 2192;
  }

  return ret;
}



