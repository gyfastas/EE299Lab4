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
#define xi0  31
#define do1  33
#define doS1 35
#define rui1  37
#define ruiS1 39
#define mi1  41
#define fa1  44
#define faS1 46
#define so1  49
#define soS1 52
#define la1  55
#define laS1 58
#define xi1  62
#define do2  65
#define doS2 69
#define rui2  73
#define ruiS2 78
#define mi2  82
#define fa2  87
#define faS2 93
#define so2  98
#define soS2 104
#define la2  110
#define laS2 117
#define xi2  123
#define do3  131
#define doS3 139
#define rui3  147
#define ruiS3 156
#define mi3  165
#define fa3  175
#define faS3 185
#define so3  196
#define soS3 208
#define la3  220
#define laS3 233
#define xi3  247
#define do4  262
#define doS4 277
#define rui4  294
#define ruiS4 311
#define mi4  330
#define fa4  349
#define faS4 370
#define so4  392
#define soS4 415
#define la4  440
#define laS4 466
#define xi4  494
#define do5  523
#define doS5 554
#define rui5  587
#define ruiS5 622
#define mi5  659
#define fa5  698
#define faS5 740
#define so5  784
#define soS5 831
#define la5  880
#define laS5 932
#define xi5  988
#define do6  1047
#define doS6 1109
#define rui6  1175
#define ruiS6 1245
#define mi6  1319
#define fa6  1397
#define faS6 1480
#define so6  1568
#define soS6 1661
#define la6  1760
#define laS6 1865
#define xi6  1976
#define do7  2093
#define doS7 2217
#define rui7  2349
#define ruiS7 2489
#define mi7  2637
#define fa7  2794
#define faS7 2960
#define so7  3136
#define soS7 3322
#define la7  3520
#define laS7 3729
#define xi7  3951
#define do8  4186
#define doS8 4435
#define rui8  4699
#define ruiS8 4978



//beep on pin 9
const int beep = 11;
//button on pin 10
const int button_1 = 10;
//time period for interrupt (ms)
const int timer2_period = 10;

//variable for music play			
int audio_ptr=0;
// use pace to modify your music duration							
float pace=1.2;  		
//play_flag
int play_flag =0;
//go flag
int go_flag = 0;
//this is the ISP
void T_Handler();

int soft_counter = 0;
int soft_counter1_max = 10;
//Transmit event
void Transmit_event();
// key variables

unsigned int key_state = 0;
unsigned int key_flag = 0;
unsigned int key_timer = 0;
const unsigned int period = 5;
String p_s;


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

  int melody[300][2]=
{
    {rui6,200},{mi6,200},{la5,100},{so5,100},{la5,100},{so5,100},{rui6,200},{mi6,200},{la5,100},{so5,100},{la5,100},{so5,100},{rui6,200},{mi6,200},{la5,100},{so5,100},{la5,100},{so5,100},{do1,200},{xi0,200},{la5,200},{so5,200},
        {rui6,200},{mi6,200},{la5,100},{so5,100},{la5,100},{so5,100},{rui6,200},{mi6,200},{la5,100},{so5,100},{la5,100},{so5,100},
        {rui6,200},{mi6,200},{so6,200},{do7,200},{xi6,100},{do7,100},{xi6,100},{la6,100},{so6,200},{mi6,200},
        {rui6,200},{mi6,200},{la5,100},{so5,100},{la5,100},{so5,100},{rui6,200},{mi6,200},{la5,100},{so5,100},{la5,100},{so5,100},
        {rui6,200},{mi6,200},{la5,100},{so5,100},{la5,100},{so5,100},{do1,200},{xi0,200},{la5,200},{so5,200},
        {la5,200},{so5,100},{la5,100},{do1,200},{la5,100},{do1,100},{rui6,200},{rui6,100},{mi6,100},{so6,100},{do7,100},{mi6,100},{so6,100},
        {do7,400},{xi6,133},{do7,133},{xi6,133},{la6,200},{so6,200},{la6,400},{la6,200},{do7,200},{0,200},
        //����
        {la6,250},{la6,150},{0,50},{so6,100},{la6,200},{do7,200},{rui7,200},{mi7,200},
        {la6,250},{la6,150},{0,50},{so6,100},{la6,200},{so6,200},{mi6,200},{so6,200},
        {la6,250},{la6,150},{la6,150},{0,50},{so6,100},{la6,200},{do7,200},{rui7,200},{mi7,200},
        {mi7,400},{rui7,400},{do7,400},{la6,400},
        {la6,200},{la6,200},{so6,100},{la6,200},{do7,200},{rui7,200},{mi7,200},
        {la6,200},{la6,200},{so6,100},{la6,200},{so6,200},{so6,200},{mi6,200},
        {la6,200},{la6,200},{so6,100},{so6,200},{la6,200},{do7,200},{rui7,200},
        {mi7,400},{rui7,400},{do7,400},{la6,400},
        //
        {do7,400},{xi6,400},{la6,400},{so6,400},
        {so6,200},{so6,100},{la6,100},{mi6,200},{rui6,200},{mi6,400},{0,200},
        {mi6,200},{so6,200},{la6,400},{rui7,400},{xi6,400},{0,200},
        {do7,400},{xi6,200},{so6,200},{la6,400},
        {do7,400},{xi6,400},{la6,400},{so6,400},
        {so6,200},{so6,100},{la6,100},{mi6,200},{rui6,200},{mi6,400},{mi6,200},{so6,200},
        {la6,200},{la6,200},{la6,200},{la6,200},{do7,400},{rui7,400},{xi6,400},{0,200},
        //�߳�
        {la6,200},{do7,200},{rui7,200},{rui7,200},{rui7,200},{mi7,200},{mi7,400},{mi7,200},{mi7,200},
        {so7,200},{la7,200},{rui7,200},{do7,200},{mi7,400},{la6,200},{do7,200},
        {rui7,200},{rui7,200},{rui7,200},{mi7,200},{mi7,400},{mi7,200},{mi7,200},
        {fa7,200},{mi7,200},{rui7,200},{do7,200},{do7,400},{la6,200},{do7,200},
        {rui7,200},{rui7,200},{rui7,200},{mi7,200},{mi7,400},{mi7,200},{mi7,200},
        {so7,200},{la7,200},{rui7,200},{do7,200},{mi7,400},{la6,200},{do7,200},
              {fa7,400},{mi7,400},{rui7,400},{do7,400},{rui7,200},{mi7,200},{xi6,200},{so6,200},{la6,400},{la6,200},{do7,200},
              {rui7,200},{rui7,200},{rui7,200},{mi7,200},{mi7,400},{mi7,200},{mi7,200},
                            {so7,200},{la7,200},{rui7,200},{do7,200},{mi7,400},{la6,200},{do7,200},
                            {rui7,200},{rui7,200},{rui7,200},{mi7,200},{mi7,400},{mi7,200},{mi7,200},
                            {fa7,200},{mi7,200},{rui7,200},{do7,200},{do7,400},{la6,200},{do7,200},
                            {rui7,200},{rui7,200},{rui7,200},{mi7,200},{mi7,400},{mi7,200},{mi7,200},
                            {so7,200},{la7,200},{rui7,200},{do7,200},{mi7,400},{la6,200},{do7,200},
        {fa7,400},{mi7,400},{rui7,400},{do7,400},{rui6,200},{do1,200},{mi6,200},{so6,200},{la6,400},{0,0}
};

  
  while(1)
  {
  //FSM State Change
  if(next_st !=st)
  { st = next_st;
    pre_st = st;
  }
  if(play_flag)
  { 
    play_flag = 0;
    if(melody[audio_ptr][0]==0&&melody[audio_ptr][1]==0)
    {
      audio_ptr=0;
    }
    else
    {
      tone(beep,melody[audio_ptr][0],melody[audio_ptr][1]*pace);
      audio_ptr++;
    }
  }
  //
  if(key_flag)
  {
    key_flag = 0;
    play_flag = 1;
  }
  Transmit_event();
  }
}



//20ms timer ISP
void T_Handler()
{ 
  //auto play
  if(++soft_counter>=soft_counter1_max)
  {
    soft_counter = 0;
  if(go_flag)
  {
    play_flag = 1 - play_flag;
  }
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


void Transmit_event()
{
  while(Serial.available()>0)
  {
    p_s +=char(Serial.read());

    switch(st)
    {
      case MENU:
           if(p_s.equals("play"))
           {
            go_flag = 1;
            Serial.print("play");
           }
           if(p_s.equals("pause"))
           {
            go_flag = 0;
            play_flag = 0;
            Serial.print("Pause");
           }
           if(p_s.equals("stop"))
           {
            Serial.print("The music is stop now.");
            audio_ptr = 0;
            play_flag = 0;
            go_flag = 0;
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
  //clear the input string when transmit finish
  p_s = "";
}

