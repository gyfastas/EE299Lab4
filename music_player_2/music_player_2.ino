/*
EE299 Lab4 -try1_2
Simple Music Player with button to play/stop
keep pushing to play

__Version__2:
1. Serial is used to control the music play!
2. Write your own music on Serial!
3. FSM : MENU CHANGE expand it if you want!
Author: Yuanfan Guo
Rev.0 2018.8.9
*/
#include<MsTimer2.h>
#include<String.h>
//some note
#define do0 261
#define rui0 294
#define mi0  330
#define fa0 349
#define so0 392
#define la0 440
#define xi0 494
#define do1 523
#define rui1 587
#define mi1 659
#define fa1 698
#define so1 784
#define la1 880
#define xi1 988
#define do2 1045
#define rui2 1174
#define mi2 1318
#define fa2 1397
#define so2 1568
#define la2 1760
#define xi2 1975


//beep on pin 9
const int beep = 9;
//button on pin 10
const int button_1 = 10;
//time period for interrupt (ms)
const int timer2_period = 10;

//variable for music play			
int audio_ptr=0;
// use pace to modify your music duration							
float pace=1.5;  		
//play_flag
int play_flag =0;

//this is the ISP
void T_Handler();

//music_player function
void music_player();

//Transmit event
void Transmit_event();
// key variables

unsigned int key_state = 0;
unsigned int key_flag = 0;
unsigned int key_timer = 0;
const unsigned int period = 1;//here we do a period*20ms on/off 
String p_s;
int melody[200][2]=
{
      {la1,200},{do2,200},{rui2,200},{rui2,200},{rui2,200},{mi2,200},{mi2,400},{mi2,200},{mi2,200},
              {so2,200},{la2,200},{rui2,200},{do2,200},{mi2,400},{la1,200},{do2,200},
              {rui2,200},{rui2,200},{rui2,200},{mi2,200},{mi2,400},{mi2,200},{mi2,200},
              {fa2,200},{mi2,200},{rui2,200},{do2,200},{do2,400},{la1,200},{do2,200},
              {rui2,200},{rui2,200},{rui2,200},{mi2,200},{mi2,400},{mi2,200},{mi2,200},
              {so2,200},{la2,200},{rui2,200},{do2,200},{mi2,400},{la1,200},{do2,200},
                    {fa2,400},{mi2,400},{rui2,400},{do2,400},{rui2,200},{mi2,200},{xi1,200},{so1,200},{la1,400},{la1,200},{do2,200},
                    {rui2,200},{rui2,200},{rui2,200},{mi2,200},{mi2,400},{mi2,200},{mi2,200},
                                  {so2,200},{la2,200},{rui2,200},{do2,200},{mi2,400},{la1,200},{do2,200},
                                  {rui2,200},{rui2,200},{rui2,200},{mi2,200},{mi2,400},{mi2,200},{mi2,200},
                                  {fa2,200},{mi2,200},{rui2,200},{do2,200},{do2,400},{la1,200},{do2,200},
                                  {rui2,200},{rui2,200},{rui2,200},{mi2,200},{mi2,400},{mi2,200},{mi2,200},
                                  {so2,200},{la2,200},{rui2,200},{do2,200},{mi2,400},{la1,200},{do2,200},
              {fa2,400},{mi2,400},{rui2,400},{do2,400},{rui1,200},{do1,200},{mi1,200},{so1,200},{la1,400},{0,0}
};

//FSM
enum state{MENU,WRITE};
state st,pre_st,next_st;

void setup() {
  // put your setup code here, to run once:
  //20ms interrupt
  MsTimer2::set(timer2_period,T_Handler);
  MsTimer2::start();
  pinMode(button_1,INPUT);

  //__version__2 : serial
  Serial.begin(9600);

  //__version__2 : FSM
  st = pre_st = next_st = MENU;
  
}

void loop() {
  // put your main code here, to run repeatedly:

  //FSM State Change
  if(next_st !=st)
  { st = next_st;
    pre_st = st;
  }
  //
  if(key_flag)
  {
    key_flag = 0;
    music_player();
  }
  Transmit_event();
}


//20ms timer ISP
void T_Handler()
{ 
  //music_play
  if(play_flag)
  {
    music_player();
  }
  //frequency + duration =0 -> end of the music
  switch(key_state)
  {
    case 0:
        if(digitalRead(button_1))
          {
            key_state = 1;
            key_flag = 1;
            key_timer = 0;
          }
          break;
    case 1:
      if(digitalRead(button_1)==LOW)
      {
        key_state =0;
      }
        else
      {
        // long enough!Let's send a flag and get into state2 
        if(++key_timer>period)
        { key_timer = 0;
          key_state = 2;
          key_flag = 1;
        }
        }
        break;
     case 2:
        if(digitalRead(button_1) == LOW)
        {
            key_state = 0;
            key_timer = 0;
        }
        else
        {
            if(++key_timer > period)
            // long enough! Let's send our masseage
            {
              key_timer = 0;
              key_flag = 1;
              
            }
         }
        break;
     //never forget default case 
     default:key_state = 0;break; 
  } 
}

//music_player module used to play music
void music_player()
{
    if(melody[audio_ptr][0]==0&&melody[audio_ptr][1]==0)
  {
    audio_ptr = 0;
  }
  else 
  {
    tone(beep,melody[audio_ptr][0],melody[audio_ptr][1]*pace);
    audio_ptr++;
  }
}

void Transmit_event()
{
  while(Serial.available()>0)
  {
    p_s = Serial.readString();

    switch(st)
    {
      case MENU:
           if(p_s.equals("play"))
           {
            play_flag = 1;
            Serial.print("play");
           }
           if(p_s.equals("pause"))
           {
            play_flag = 0;
            Serial.print("Pause");
           }
           if(p_s.equals("stop"))
           {
            Serial.print("The music is stop now.");
            audio_ptr = 0;
            play_flag = 0;
           }
           if(p_s.equals("Write"))
           {
            next_st = WRITE;
           }
           break;
      case WRITE:
           if(p_s.equals("quit"))
           {
            next_st = MENU;
           }
      break;
      default:break;
    }
    
  }
}

