#include<MsTimer2.h>
const int DECODER_EN1 = 6;
const int DECODER_EN2 = 7;
const int DECODER_A1 = 8;
const int DECODER_B1 = 9;
const int DECODER_C1 = 10;
const int DECODER_A2 = 11;
const int DECODER_B2 = 12;
const int DECODER_C2 = 13;

// a function to flash the LED Metrix
void LED_M_FLASH(int DECODER_IN_1,int DECODER_IN_2);

//Enbale the LED Matrix
void LED_M_EN();
//Unenable the LED Matrix
void LED_M_UEN();

void setup() {
  //3-8 decoder setup
  pinMode(DECODER_EN1,OUTPUT);
  pinMode(DECODER_EN2,OUTPUT);
  pinMode(DECODER_A1,OUTPUT);
  pinMode(DECODER_B1,OUTPUT);
  pinMode(DECODER_C1,OUTPUT);
  pinMode(DECODER_A2,OUTPUT);
  pinMode(DECODER_B2,OUTPUT);
  pinMode(DECODER_C2,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  LED_M_EN();
  LED_M_FLASH(0,0);
  delay(10);
  LED_M_FLASH(0,1);
  delay(10);
  LED_M_FLASH(0,2);
  delay(10);
  LED_M_FLASH(0,3);
  delay(10);
  LED_M_FLASH(0,4);
  
}

void LED_M_EN()
{
  digitalWrite(DECODER_EN1,HIGH);
  digitalWrite(DECODER_EN2,HIGH);
}
void LED_M_UEN()
{
  digitalWrite(DECODER_EN2,LOW);
  digitalWrite(DECODER_EN1,LOW);
}
void LED_M_FLASH(int DECODER_IN_1,int DECODER_IN_2)
{
  switch(DECODER_IN_1)
  {
    case 0: digitalWrite(DECODER_A1,LOW);digitalWrite(DECODER_B1,LOW);digitalWrite(DECODER_C1,LOW);break;
    case 1: digitalWrite(DECODER_A1,HIGH);digitalWrite(DECODER_B1,LOW);digitalWrite(DECODER_C1,LOW);break;
    case 2: digitalWrite(DECODER_A1,LOW);digitalWrite(DECODER_B1,HIGH);digitalWrite(DECODER_C1,LOW);break;
    case 3: digitalWrite(DECODER_A1,HIGH);digitalWrite(DECODER_B1,HIGH);digitalWrite(DECODER_C1,LOW);break;
    case 4: digitalWrite(DECODER_A1,LOW);digitalWrite(DECODER_B1,LOW);digitalWrite(DECODER_C1,HIGH);break;
    case 5: digitalWrite(DECODER_A1,HIGH);digitalWrite(DECODER_B1,LOW);digitalWrite(DECODER_C1,HIGH);break;
    case 6: digitalWrite(DECODER_A1,LOW);digitalWrite(DECODER_B1,HIGH);digitalWrite(DECODER_C1,HIGH);break;
    case 7: digitalWrite(DECODER_A1,HIGH);digitalWrite(DECODER_B1,HIGH);digitalWrite(DECODER_C1,HIGH);break;
    default:digitalWrite(DECODER_A1,LOW);digitalWrite(DECODER_B1,LOW);digitalWrite(DECODER_C1,LOW);break;
  }
  switch(DECODER_IN_2)
  {
    case 0: digitalWrite(DECODER_A2,LOW);digitalWrite(DECODER_B2,LOW);digitalWrite(DECODER_C2,LOW);break;
    case 1: digitalWrite(DECODER_A2,HIGH);digitalWrite(DECODER_B2,LOW);digitalWrite(DECODER_C2,LOW);break;
    case 2: digitalWrite(DECODER_A2,LOW);digitalWrite(DECODER_B2,HIGH);digitalWrite(DECODER_C2,LOW);break;
    case 3: digitalWrite(DECODER_A2,HIGH);digitalWrite(DECODER_B2,HIGH);digitalWrite(DECODER_C2,LOW);break;
    case 4: digitalWrite(DECODER_A2,LOW);digitalWrite(DECODER_B2,LOW);digitalWrite(DECODER_C2,HIGH);break;
    case 5: digitalWrite(DECODER_A2,HIGH);digitalWrite(DECODER_B2,LOW);digitalWrite(DECODER_C2,HIGH);break;
    case 6: digitalWrite(DECODER_A2,LOW);digitalWrite(DECODER_B2,HIGH);digitalWrite(DECODER_C2,HIGH);break;
    case 7: digitalWrite(DECODER_A2,HIGH);digitalWrite(DECODER_B2,HIGH);digitalWrite(DECODER_C2,HIGH);break;
    default:digitalWrite(DECODER_A2,LOW);digitalWrite(DECODER_B2,LOW);digitalWrite(DECODER_C2,LOW);break;
  }

}

