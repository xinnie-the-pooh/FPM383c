#define ms 2
#define DE 1600 //100*16 0.9degree per plus
#define ST P2_0 //step pin
#define DI P2_2   //dir pin
#define EN P2_4   //dir pin
uint8_t PS_ReceiveBuffer[20];
uint8_t PS_EmptyBuffer[12] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x03,0x0D,0x00,0x11};
uint8_t PS_GetImageBuffer[12] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x03,0x01,0x00,0x05};
uint8_t PS_GetChar1Buffer[13] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x04,0x02,0x01,0x00,0x08};

uint8_t PS_BlueLEDBuffer[16] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x07,0x3C,0x03,0x01,0x01,0x00,0x00,0x49};
uint8_t PS_RedLEDBuffer[16] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x07,0x3C,0x03,0x04,0x04,0x00,0x00,0x4f};
uint8_t PS_GreenLEDBuffer[16] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x07,0x3C,0x03,0x02,0x02,0x00,0x00,0x4B};

uint8_t PS_SearchMBBuffer[17] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x04,0x01,0x00,0x00,0xFF,0xFF,0x02,0x0C};
uint8_t PS_AutoEnrollBuffer[17] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x31,'\0','\0',0x04,0x00,0x16,'\0','\0'}; 

void FPM383C_SendData(int len,uint8_t PS_Databuffer[])
{
  Serial1.write(PS_Databuffer,len);
  while(Serial1.read() >= 0);
  memset(PS_ReceiveBuffer,0xFF,sizeof(PS_ReceiveBuffer));
}

void FPM383C_ReceiveData(uint16_t Timeout)
{
  uint8_t i = 0;
  while(Serial1.available() == 0 && (--Timeout))
  {
    delay(1);
  }
  while(Serial1.available() > 0)
  {
    delay(2);
    PS_ReceiveBuffer[i++] = Serial1.read();
    if(i > 15) break; 
  }
}

void PS_ControlLED(uint8_t PS_ControlLEDBuffer[])
{
  FPM383C_SendData(16,PS_ControlLEDBuffer);
}

uint8_t PS_GetImage()
{
  FPM383C_SendData(12,PS_GetImageBuffer);
  FPM383C_ReceiveData(2000);
  return PS_ReceiveBuffer[6] == 0x07 ? PS_ReceiveBuffer[9] : 0xFF;
}

uint8_t PS_GetChar1()
{
  FPM383C_SendData(13,PS_GetChar1Buffer);
  FPM383C_ReceiveData(2000);
  return PS_ReceiveBuffer[6] == 0x07 ? PS_ReceiveBuffer[9] : 0xFF;
}



uint8_t PS_SearchMB()
{
  FPM383C_SendData(17,PS_SearchMBBuffer);
  FPM383C_ReceiveData(2000);
  return PS_ReceiveBuffer[6] == 0x07 ? PS_ReceiveBuffer[9] : 0xFF;
}

uint8_t PS_AutoEnroll(uint16_t PageID)
{
  PS_AutoEnrollBuffer[10] = (PageID>>8);
  PS_AutoEnrollBuffer[11] = (PageID);
  PS_AutoEnrollBuffer[15] = (0x54+PS_AutoEnrollBuffer[10]+PS_AutoEnrollBuffer[11])>>8;
  PS_AutoEnrollBuffer[16] = (0x54+PS_AutoEnrollBuffer[10]+PS_AutoEnrollBuffer[11]);
  FPM383C_SendData(17,PS_AutoEnrollBuffer);
  FPM383C_ReceiveData(10000);
  return PS_ReceiveBuffer[6] == 0x07 ? PS_ReceiveBuffer[9] : 0xFF;
}

uint8_t PS_Enroll(uint16_t PageID)
{
  if(PS_AutoEnroll(PageID) == 0x00)
  {
    PS_ControlLED(PS_GreenLEDBuffer);
    return PS_ReceiveBuffer[9];
  }
  PS_ControlLED(PS_RedLEDBuffer);
  return 0xFF;
}

void PS_Identify()
{
  if(PS_GetImage() == 0x00)
  {
    if(PS_GetChar1() == 0x00)
    {
      if(PS_SearchMB() == 0x00)
      {
        if(PS_ReceiveBuffer[9] == 0x00)
        {
          PS_ControlLED(PS_GreenLEDBuffer);
          digitalWrite(EN,LOW);
          motor();
          digitalWrite(EN,HIGH);
          return;
        }
      }
    }
  }
  PS_ControlLED(PS_RedLEDBuffer);
}



void setup()
{  Serial.begin(9600); 
  Serial1.begin(57600); //fmc383 use this
   pinMode(PUSH2,INPUT_PULLUP);
  pinMode(RED_LED,OUTPUT);
  pinMode(DI,OUTPUT);
  pinMode(ST,OUTPUT);
  pinMode(EN,OUTPUT);
  digitalWrite(DI,HIGH);
  digitalWrite(EN,HIGH);
}

void loop()
{
 PS_ControlLED(PS_BlueLEDBuffer);
    delay(1000);
      PS_Identify();//judge, if right ,green and blink led(simulate step motor),error,turn red
    
    delay(600);
   while(digitalRead(PUSH2)==0) //change finger while push button 2
   {
   FPM383C_SendData(12,PS_EmptyBuffer); //clear mem
    PS_Enroll(0);//reg  new mem
    }
   

}
void motor()//fake step motor fuction
{
  for(int i=0;i<DE;i++){
  digitalWrite(RED_LED,HIGH);
  digitalWrite(ST,HIGH);
  delay(ms);
  digitalWrite(RED_LED,LOW);
   digitalWrite(ST,LOW);
  delay(ms);
  }}
