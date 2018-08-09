/*
EE299 Lab4 -try1
Simple Music Player

Author: Yuanfan Guo
Rev.0 2018.8.9
*/
#include<MsTimer2.h>
#include "pitch.h"
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
//time period for interrupt
const int timer2_period = 20;

//variable for music play			
unsigned int audio_ptr=0;							
unsigned int pace=1;  						
//melody +duration
int melody[][2]=
{
		{rui1,200},{mi1,200},{la0,100},{so0,100},{la0,100},{so0,100},{rui1,200},{mi1,200},{la0,100},{so0,100},{la0,100},{so0,100},{rui1,200},{mi1,200},{la0,100},{so0,100},{la0,100},{so0,100},{do1,200},{xi0,200},{la0,200},{so0,200},
				{rui1,200},{mi1,200},{la0,100},{so0,100},{la0,100},{so0,100},{rui1,200},{mi1,200},{la0,100},{so0,100},{la0,100},{so0,100},
				{rui1,200},{mi1,200},{so1,200},{do2,200},{xi1,100},{do2,100},{xi1,100},{la1,100},{so1,200},{mi1,200},
				{rui1,200},{mi1,200},{la0,100},{so0,100},{la0,100},{so0,100},{rui1,200},{mi1,200},{la0,100},{so0,100},{la0,100},{so0,100},
				{rui1,200},{mi1,200},{la0,100},{so0,100},{la0,100},{so0,100},{do1,200},{xi0,200},{la0,200},{so0,200},
				{la0,200},{so0,100},{la0,100},{do1,200},{la0,100},{do1,100},{rui1,200},{rui1,100},{mi1,100},{so1,100},{do2,100},{mi1,100},{so1,100},
				{do2,400},{xi1,133},{do2,133},{xi1,133},{la1,200},{so1,200},{la1,400},{la1,200},{do2,200},{0,200},
				
				{la1,250},{la1,150},{0,50},{so1,100},{la1,200},{do2,200},{rui2,200},{mi2,200},
				{la1,250},{la1,150},{0,50},{so1,100},{la1,200},{so1,200},{mi1,200},{so1,200},
				{la1,250},{la1,150},{la1,150},{0,50},{so1,100},{la1,200},{do2,200},{rui2,200},{mi2,200},
				{mi2,400},{rui2,400},{do2,400},{la1,400},
				{la1,200},{la1,200},{so1,100},{la1,200},{do2,200},{rui2,200},{mi2,200},
				{la1,200},{la1,200},{so1,100},{la1,200},{so1,200},{so1,200},{mi1,200},
				{la1,200},{la1,200},{so1,100},{so1,200},{la1,200},{do2,200},{rui2,200},
				{mi2,400},{rui2,400},{do2,400},{la1,400},
				
				{do2,400},{xi1,400},{la1,400},{so1,400},
				{so1,200},{so1,100},{la1,100},{mi1,200},{rui1,200},{mi1,400},{0,200},
				{mi1,200},{so1,200},{la1,400},{rui2,400},{xi1,400},{0,200},
				{do2,400},{xi1,200},{so1,200},{la1,400},
				{do2,400},{xi1,400},{la1,400},{so1,400},
				{so1,200},{so1,100},{la1,100},{mi1,200},{rui1,200},{mi1,400},{mi1,200},{so1,200},
				{la1,200},{la1,200},{la1,200},{la1,200},{do2,400},{rui2,400},{xi1,400},{0,200},
				
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

//this is the ISP
void T_Handler();

void setup() {
  // put your setup code here, to run once:
  //20ms interrupt
  MsTimer2::set(timer2_period,T_Handler);
  MsTimer2::start();
}

void loop() {
  // put your main code here, to run repeatedly:

}


//20ms timer ISP
void T_Handler()
{
  //frequency + duration =0 -> end of the music
  if(melody[audio_ptr][0]==0&&melody[audio_ptr][1]==0)
  {
    audio_ptr = 0;
  }
  else 
  {
    tone(beep,melody[audio_ptr][0],melody[audio_ptr][1]/pace);
    audio_ptr++;
  }
}
