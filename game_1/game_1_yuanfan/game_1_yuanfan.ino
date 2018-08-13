/*EE299 Project 4
 * 
 * A game that plays with RGB LED.
 * Try to make it correct with senior monitor
 */
#include<MsTimer2.h>
#include<String.h>
#define RED 0x01
#define GREEN 0x02
#define BLUE 0x04

#define MAX_LEN 30
//timers period is 10ms
const int timer2_period = 10;
//ISP for timer2 interrupt
void T_Handler();

//pin define
int Red = 11;
int Green = 12;
int Blue = 13;
int beep = 10;

//color [note][time]
int cu_note = 0;
int cu_time = 0;

//Serial string
String p_s = "";

//software counter
//level 1:1s 1note generated
int Soft_time_1 = 100;
int Soft_flag_1 = 0;
//define max and min react time,defaut: min 300ms,max 1000ms
int min_r_time = 30;
int max_r_time = 100;

//FSM
enum state{GAME,LOSE};
state st,pre_st,next_st;
void setup() {
  //Set up step
  //BEGIN
  MsTimer2::set(timer2_period,T_Handler);
  MsTimer2::start();
  pinMode(Red,OUTPUT);
  pinMode(Green,OUTPUT);
  pinMode(Blue,OUTPUT);

  Serial.begin(9600);
  //END

  //MAIN LOOP
  //BEGIN
  while(1)
  { 
    //Soft Interrupt 1
    //BEGIN
    if(Soft_flag_1>=Soft_time_1)
    {
      //generate a note
      
      //reset timer
      Soft_flag_1 = 0;
    }
    //END

    //FSM State Change
    //BEGIN
    
    //END
    //Transmit Event
    //BEGIN
    while(Serial.available()>0)
    {
      p_s+=char(Serial.read());
      
      
    }
    p_s = "";
    //END
  }
  //END

}


void T_Handler()
{
    //soft timer generate
    //BEGIN
    Soft_flag_1 ++;
    //END

    //note time counting
    //BEGIN
    if(cu_time>0)
    {
      cu_time --;
    }
    else
    {
      
    }
    //END
}
