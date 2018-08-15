/*EE299 Project 4
 * 
 * A game that plays with RGB LED.
 * Try to make it correct with senior monitor
 */
#include<MsTimer2.h>
#include<String.h>
#include<LiquidCrystal.h>
#include<time.h>
//arrow define
#define NOINPUT -1
#define UP 24
#define DOWN 25
#define RIGHT 26
#define LEFT 27

#define MAX_LEN 16
//note structure define
struct note
{
  char dic ; //UP,DOWN,LEFT,RIGHT
  bool isBeat; 
};

//a easy queue class for note
class queue
{
  public:
  queue()
  { 
    //initialize it
    for (int i =0;i<MAX_LEN;i++)
    { 
      //dic = 0 ->no arrow
      data[i].dic = 0;
      data[i].isBeat = false;
    }
    front = rear = current_size  = 0;
  }
  bool empty()
  {
    if(front==rear)
    return 1;
    else
    return 0;
  }

  bool full()
  {
    if((rear+1)%MAX_LEN == front)
    {
      return 1;
    }
    else
      return 0;
  }

  bool pop()
  {
    if(empty())
    return 0;
    else
    { 
      front =(front+1)%MAX_LEN;
      current_size--;
      return 1;
    }
  }

  bool Front(note &x)
  {
    if(empty())
    {
      return 0;
    }
    else
    {
      x = data[front];
      return 1;
    }
  }

  bool Rear(note &x)
  {
    if(empty())
    {
      return 0;
    }
    else
    {
      x = data[rear];
      return 1;
    }
  }

  bool push(note x)
  {
    if((rear+1)%MAX_LEN==front)
      return 0;
    else
    { 
      rear = (rear+1)%MAX_LEN;
      data[rear].dic = x.dic;
      data[rear].isBeat = x.isBeat;
      current_size ++;
      return 1;
    }
  }

  bool single_generate()
  {
    if(full())
    {
      return 0;
    }
    else
    { 
      note x;
      x.dic = random(UP,LEFT+1);
      x.isBeat = false;
      push(x);
      return 1;
    }
  }

  //don't use it if you are not sure the size
  bool multi_generate(int num)
  { 
    note x;
    for(int i =0;i<num;i++)
    {
      x.dic = random(UP,LEFT+1);
      x.isBeat = false;
      push(x);
    }
  }
  int size()
  {
    return current_size;
  }

  bool to_String(String &x)
  {
    if(empty())
    {
      return 0;
    }
    else
    { int i = rear;
      while( (i-1)%MAX_LEN!=front)
      {
        x += data[i].dic;
      }
    }
  }
  private:
  note data[MAX_LEN];
  int current_size;
  int front;
  int rear;
};



LiquidCrystal lcd(2,3,4,5,6,7,8); //using bus1 on starter kit 

//timers period is 10ms
const int timer2_period = 10;
//ISP for timer2 interrupt
void T_Handler();

//pin define
int Red = 11;
int Green = 12;
int Blue = 13;
int beep = 10;



//Serial string
String p_s = "";

//LCD string
String lcd_note = "";

//note queue
queue Note_Q ;

//software counter
//counter 1 time = soft_time_x *10 ms
int Soft_time_1 = 30;
int Soft_flag_1 = 0;
//define max and min react time,defaut: min 300ms,max 1000ms
int min_r_time = 50;
int max_r_time = 100;
int generate_time = random(min_r_time,max_r_time);
int generate_flag = 0;

//FSM
enum state{GAME,LOSE};
state st,pre_st,next_st;

void setup() {
  randomSeed(time(NULL));
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
      //scrollright
      lcd.scrollDisplayRight();
      //reset timer
      Soft_flag_1 = 0;
    }
    //END

    if(generate_flag>=generate_time)
    {
        Note_Q.single_generate();
    }
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
    //soft timer counting
    //BEGIN
    Soft_flag_1 ++;
    generate_flag++;
    //END
    
}

