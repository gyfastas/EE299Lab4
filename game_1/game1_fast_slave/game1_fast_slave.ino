/*EE299 Project 4
 * 
 * A game that plays with RGB LED.
 * Try to make it correct with senior monitor
 */
#include<MsTimer2.h>
#include<String.h>
#include<LiquidCrystal.h>
#include<time.h>
#include<Wire.h>
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
char note_1  = 0;
char note_2 = 0;

//LCD string 1、2
char Row1[MAX_LEN];
char Row2[MAX_LEN];

//Liquid Crystal define
LiquidCrystal lcd(2,3,4,5,6,7,8);
LiquidCrystal scoreBoard(9,10,11,12,13,14,15);

//timers period is 10ms
const int timer2_period = 10;
//ISP for timer2 interrupt
void T_Handler();


//button pin define
const int button_up = 9;
const int button_down = 10;
const int button_left = 11;
const int button_right = 12;
const int light_sensor = A1;

const int sensor_thres = 200;
int sensor_value = 0;
//software counter
//lcd flash rate
int flash_timer_max = 10;
int flash_timer = 0;
int flash_flag = 0;
//counter 1 time = soft_time_x *10 ms
int soft_timer1_max =30;
int soft_timer1 = 0;
int Soft_flag_1 = 0;

//time for generate a new note
int generate_timer_max = 60;
int generate_timer = 0;
int generate_flag = 0;


//FSM
enum state{MENU,GAME,WIN,LOSE};
state st,pre_st,next_st;

//play_flag
int play_flag =0;

//perfect,good,bad number
int perfect_num  = 0;
int good_num = 0;
int bad_num = 0;

// key variables
int key_state_up,key_state_down,key_state_left,key_state_right = 0;
int key_flag_up,key_flag_down,key_flag_left,key_flag_right = 0;
int key_timer_up,key_timer_down,key_timer_left,key_timer_right = 0;

const int period = 2;

void move_right(char *row,int size);

void one_generate();
void flow_generate();

char beat_one(char *row,int size);

void lcd_flash();

//debounce function
void debounce(int button_up,int &key_state,int &key_flag,int &key_timer);

void setup() {
  //setup
  randomSeed(time(NULL));
  lcd.begin(16,2);
  MsTimer2::set(timer2_period,T_Handler);
  MsTimer2::start();

  pinMode(button_up,INPUT);
  pinMode(button_down,INPUT);
  pinMode(button_left,INPUT);
  pinMode(button_right,INPUT);
  pinMode(light_sensor,INPUT);
  
  Serial.begin(9600);

  //initialize the row
  for(int i =0;i<16;i++)
  {
    Row1[i]=' ';
    Row2[i]=' ';
  }


  while(1)
  { 

    
    //lcd flash
    if(flash_flag)
    {
      flash_flag = 0;
      lcd_flash();
    }
    //Scroll right
    if(Soft_flag_1)
    {
      //scrollright
      move_right(Row1,16);
      move_right(Row2,16);
      //reset timer
      Soft_flag_1 = 0;
    }
    
    //generate a note
    if(generate_flag)
    {
        one_generate();
        generate_flag = 0;
    }


  if(key_flag_up)
  {
    key_flag_up = 0;
    play_flag = 1;
  }
  
   if(play_flag)
  { 
    play_flag = 0;
    note_1 = beat_one(Row1,16);
    Serial.write(note_1);
    
  }

  if(sensor_value<sensor_thres)
  {
    note_2 = beat_one(Row2,16);
    Serial.write(note_2);
  }
}
}

void loop()
{

}
void T_Handler()
{
    //soft timer counting
    if(soft_timer1++>soft_timer1_max)
    {
      Soft_flag_1 = 1;
      soft_timer1=0;
    }

    if (generate_timer++> generate_timer_max)
    {
      generate_flag = 1;
      generate_timer = 0;
    }
    if(flash_timer++> flash_timer_max)
    {
      flash_timer = 0;
      flash_flag = 1;
    }
    sensor_value = analogRead(light_sensor);
    debounce(button_up, key_state_up, key_flag_up, key_timer_up);
    /*
    debounce(button_down,key_state_down,key_flag_down,key_timer_down);
    debounce(button_left,key_state_left,key_flag_left,key_timer_left);
    debounce(button_right,key_state_right,key_flag_right,key_timer_right);
    */

}



void move_right(char *row,int size)
{
  for(int i =size-1;i>=0;i--)
  {
    row[i+1] = row[i];
  }
  row[0]=' ';

}

void one_generate()
{
  int x1 = random(0,2);
  if(x1==0)
  {
    int x2 = random(0,4);
    if(x2==0)
    {
      Row1[0]=UP;
    }
    else if (x2==1)
    {
      Row1[0]=DOWN;
    }
    else if(x2==2)
    {
      Row1[0]=LEFT;
    }
    else if(x2==3)
    {
      Row1[0]=RIGHT;
    }
  }
  else
  {
    int x2 = random(0,4);
    if(x2==0)
    {
      Row2[0]=UP;
    }
    else if (x2==1)
    {
      Row2[0]=DOWN;
    }
    else if(x2==2)
    {
      Row2[0]=LEFT;
    }
    else if(x2==3)
    {
      Row2[0]=RIGHT;
    }
  }
}

void flow_generate()
{
  int x1 = random(0,2);
  int x2 = random(0,4);
  int x3 = random(2,10);
  if (x1 == 0)
  {
    if (x2 == 0)
    {
      Row1[0] = UP;
    }
    else if (x2 == 1)
    {
      Row1[0] = DOWN;
    }
    else if (x2 == 2)
    {
      Row1[0] = LEFT;
    }
    else if (x2 == 3)
    {
      Row1[0] = RIGHT;
    }
    for (int i = 1; i < x3; i++)
    {
      Row1[i] = Row1[0];
    }
    for(int i = 1;i<x3;i++)
    {
      Row1[i] = Row1[0];
    }
  }
  else
  {
    if (x2 == 0)
    {
      Row2[0] = UP;
    }
    else if (x2 == 1)
    {
      Row2[0] = DOWN;
    }
    else if (x2 == 2)
    {
      Row2[0] = LEFT;
    }
    else if (x2 == 3)
    {
      Row2[0] = RIGHT;
    }
    for(int i = 1;i<x3;i++)
    {
      Row2[i]=Row2[0];
    }
  }

}

char beat_one(char*row,int size)
{ 
  char result;

  // search for a note in the last four places
  // good place:last 4
  // perfect place:last 2
  for (int i = 1; i<=4;i++)
  {
    if(row[size-i]!=' ')
    {
      result = row[size-i];
      row[size-i]=' ';
      return result;
    }
    
  }
}

void debounce(int button_up,int &key_state,int &key_flag,int &key_timer)
{
  //key management
  //frequency + duration =0 -> end of the music
  switch(key_state)
  {
    case 0:
        if(digitalRead(button_up))
          {
             key_state= 1;
            key_flag = 1;
            key_timer = 0;
          }
          break;
    case 1:
      if(digitalRead(button_up)==LOW)
      {
         key_state=0;
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
        if(digitalRead(button_up) == LOW)
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


void lcd_flash()
{
  lcd.setCursor(0, 0);
  lcd.print(Row1);
  lcd.setCursor(0, 1);
  lcd.print(Row2);
}
