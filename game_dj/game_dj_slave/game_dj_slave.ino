/*EE299 Project 4
 * 
 * A game that plays with RGB LED.
 * Try to make it correct with senior monitor
 */
#include<MsTimer2.h>
#include<String.h>
#include<time.h>

//note define
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


//arrow define
#define NOINPUT -1
#define UP 94 //^
#define DOWN 95 //_
#define RIGHT 62 // >
#define LEFT 60 // <

#define MAX_LEN 16
//Serial string
char c;

  //melody 
    int melody[300][2]=
{
    {rui6,200},{mi6,200},{la5,100},{so5,100},{la5,100},{so5,100},{rui6,200},{mi6,200},{la5,100},{so5,100},{la5,100},{so5,100},{rui6,200},{mi6,200},{la5,100},{so5,100},{la5,100},{so5,100},{do1,200},{xi0,200},{la5,200},{so5,200},
        {rui6,200},{mi6,200},{la5,100},{so5,100},{la5,100},{so5,100},{rui6,200},{mi6,200},{la5,100},{so5,100},{la5,100},{so5,100},
        {rui6,200},{mi6,200},{so6,200},{do7,200},{xi6,100},{do7,100},{xi6,100},{la6,100},{so6,200},{mi6,200},
        {rui6,200},{mi6,200},{la5,100},{so5,100},{la5,100},{so5,100},{rui6,200},{mi6,200},{la5,100},{so5,100},{la5,100},{so5,100},
        {rui6,200},{mi6,200},{la5,100},{so5,100},{la5,100},{so5,100},{do1,200},{xi0,200},{la5,200},{so5,200},
        {la5,200},{so5,100},{la5,100},{do1,200},{la5,100},{do1,100},{rui6,200},{rui6,100},{mi6,100},{so6,100},{do7,100},{mi6,100},{so6,100},
        {do7,400},{xi6,133},{do7,133},{xi6,133},{la6,200},{so6,200},{la6,400},{la6,200},{do7,200},{0,200},
        {la6,250},{la6,150},{0,50},{so6,100},{la6,200},{do7,200},{rui7,200},{mi7,200},
        {la6,250},{la6,150},{0,50},{so6,100},{la6,200},{so6,200},{mi6,200},{so6,200},
        {la6,250},{la6,150},{la6,150},{0,50},{so6,100},{la6,200},{do7,200},{rui7,200},{mi7,200},
        {mi7,400},{rui7,400},{do7,400},{la6,400},
        {la6,200},{la6,200},{so6,100},{la6,200},{do7,200},{rui7,200},{mi7,200},
        {la6,200},{la6,200},{so6,100},{la6,200},{so6,200},{so6,200},{mi6,200},
        {la6,200},{la6,200},{so6,100},{so6,200},{la6,200},{do7,200},{rui7,200},
        {mi7,400},{rui7,400},{do7,400},{la6,400},
        {do7,400},{xi6,400},{la6,400},{so6,400},
        {so6,200},{so6,100},{la6,100},{mi6,200},{rui6,200},{mi6,400},{0,200},
        {mi6,200},{so6,200},{la6,400},{rui7,400},{xi6,400},{0,200},
        {do7,400},{xi6,200},{so6,200},{la6,400},
        {do7,400},{xi6,400},{la6,400},{so6,400},
        {so6,200},{so6,100},{la6,100},{mi6,200},{rui6,200},{mi6,400},{mi6,200},{so6,200},
        {la6,200},{la6,200},{la6,200},{la6,200},{do7,400},{rui7,400},{xi6,400},{0,200},
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

//timers period is 10ms
const int timer2_period = 10;
//ISP for timer2 interrupt
void T_Handler();

//pin define
int beep = 10;
int Red = 11;
int Green = 12;
int Blue = 13;


//variable for music play      
int audio_ptr=0;
// use pace to modify your music duration             
float pace=1.4;     
//play_flag
int play_flag =0;

void color_clear();

void setup() {
  //setup
  randomSeed(time(NULL));
  MsTimer2::set(timer2_period,T_Handler);
  MsTimer2::start();
  pinMode(Red,OUTPUT);
  pinMode(Green,OUTPUT);
  pinMode(Blue,OUTPUT);
  pinMode(beep,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  
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


  while(Serial.available()>0)
  {
    c = Serial.read();
    if(c==UP)
    { 
      color_clear();
      digitalWrite(Red,HIGH);
      play_flag = 1;
    }
    else if (c==DOWN)
    { 
      color_clear();
      digitalWrite(Green,HIGH);
      play_flag = 1;
    }
    else if (c==LEFT)
    {
      color_clear();
      digitalWrite(Blue,HIGH);
      play_flag = 1;
    }
    else if(c==RIGHT)
    {
      digitalWrite(Red,HIGH);
      digitalWrite(Green,HIGH);
      digitalWrite(Blue,HIGH);
      play_flag = 1;
    }
  } 
}
void T_Handler()
{

}

void color_clear()
{
  digitalWrite(Red,LOW);
  digitalWrite(Green,LOW);
  digitalWrite(Blue,LOW);
}
