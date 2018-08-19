/*EE299 Project 4
 * 
 * A game that plays with RGB LED.
 * Try to make it correct with senior monitor
 */
#include<MsTimer2.h>
#include<String.h>
#include<LiquidCrystal.h>
#include<time.h>

//note define
#define NOINPUT -1
#define SINGLE 42 //*
#define FLOW 60   //flow

#define MAX_LEN 16
#define GL_PARA 120
//Serial string
char note_1  = 0;
char note_2 = 0;

//LCD string 1、2
char Row1[MAX_LEN];
char Row2[MAX_LEN];

//Liquid Crystal define
LiquidCrystal lcd(2,3,4,5,6,7,8);
LiquidCrystal scoreBoard(10,11,12,13,14,15,16);

//timers period is 10ms
const int timer2_period = 10;
//ISP for timer2 interrupt
void T_Handler();


// pin define
const int button_up = 9;
const int light_sensor = A4;
const int Rotation = A5;

//sensor
const int sensor_thres = 200;
int sensor_value = 0;
int sensor_state = 0;
int sensor_timer = 0;
int sensor_flag  = 0;

//counter 1 time = timer_max *10 ms
int scroll_timer_max =60;
int scroll_timer = 0;
int scroll_flag = 0;

//time for generate a new note
int generate_timer_max = 150;
int generate_timer = 0;
int generate_flag = 0;

//time for generate a flow
int generate_flow_timer_max  = 300;
int generate_flow_timer = 0;
int generate_flow_flag = 0;

//FSM STATE:
enum state{MENU,PREGAME,GAME};
state st,pre_st,next_st;
//

//perfect,good,bad number
int perfect_num  = 0;

int good_num = 0;

int bad_num = 0;

int combo_life  = 0;
// key variables
int key_state_up = 0;
int key_flag_up = 0;
int key_timer_up = 0;

//print state
int Game_Level = 0;
//game start count
int game_start_max = 300;//game start in 3s
int game_start_timer = 0;


int generate_row =  0;
const int period = 20;

int note_generate_calculation(int Game_Level);
int note_flow_generate_calculation(int Game_Level);

void game_initialize();

void move_right(char *row,int size);

void one_generate(char note);

char beat_one(char *row,int size);

void lcd_flash();

//debounce function
void debounce(int value,int &key_state,int &key_flag,int &key_timer);

void Unretrigerable_debounce(int value,int &key_state,int &key_flag);

void show_buttom();

//buttom char
byte buttom[8]
{
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};


void setup() 
{
  //setup
  randomSeed(time(NULL));
  lcd.begin(16,2);
  lcd.noAutoscroll();
  scoreBoard.begin(16,2);
  scoreBoard.noAutoscroll();

  MsTimer2::set(timer2_period,T_Handler);
  MsTimer2::start();

  pinMode(button_up,INPUT);
  pinMode(light_sensor,INPUT);
  pinMode(Rotation,INPUT);
  Serial.begin(9600);
  //initialize the row
  for(int i =0;i<MAX_LEN;i++)
  {
    Row1[i]=' ';
    Row2[i]=' ';
  }
  
  st = next_st = pre_st = MENU;
}

void loop()
{
  //state change
  if(st!=next_st)
  { 
    pre_st = st;
    st = next_st;
    //jump function(trigger once)
    switch(st)
    {
      case MENU:
                break;
      case PREGAME:
                game_start_timer = 0;
                break;            
      case GAME:
                //show buttom lines
                //initiaizing
                scoreBoard.clear();
                Serial.write('s');
                game_initialize();
                break;
      default:break;
    }

    
  }


  switch(st)
  {
  
  case MENU:
        // key -> ready to start

        if(scroll_flag)
        {
          
          Game_Level = analogRead(Rotation)/GL_PARA;
          scroll_timer_max = 60 - Game_Level * 5;
          key_flag_up = digitalRead(button_up);
         if(key_flag_up)
          {
          key_flag_up = 0;
          next_st = PREGAME;
          }
          lcd.setCursor(0,0);
          lcd.print("START");
          lcd.setCursor(0, 1);
          lcd.print("LEVEL:");
          scroll_flag = 0;
          lcd.setCursor(6,1);
          lcd.print(Game_Level);
        }
        break;
  
  case PREGAME:
        if(scroll_flag)
        {
          lcd.setCursor(0,0);
          scoreBoard.setCursor(0,0);
          lcd.print("START IN");
          scoreBoard.print("START IN");
          scroll_flag = 0;
          lcd.setCursor(8,0);
          lcd.print((game_start_max-game_start_timer)/100 +1);
          scoreBoard.setCursor(8, 0);
          scoreBoard.print((game_start_max - game_start_timer) / 100 + 1);
        }
        break;
  case GAME:
        //Scroll right
        if (scroll_flag)
        {

          Game_Level = analogRead(Rotation) / GL_PARA ;
          scroll_timer_max = 60 - Game_Level * 5 ;
          generate_timer_max = note_generate_calculation(Game_Level);
          generate_flow_timer_max = note_flow_generate_calculation(Game_Level);

          move_right(Row1, MAX_LEN);
          move_right(Row2, MAX_LEN);
          lcd_flash();
          //reset timer
          scroll_flag = 0;
          sensor_value = analogRead(light_sensor);
          if (sensor_value < sensor_thres)
            sensor_flag = 1;
          else
            sensor_flag = 0;
          //score board show
          scoreBoard.setCursor(0,0);
          scoreBoard.print("P:");
          scoreBoard.setCursor(2,0);
          scoreBoard.print(perfect_num);
          scoreBoard.setCursor(6,0);
          scoreBoard.print("G:");
          scoreBoard.setCursor(8,0);
          scoreBoard.print(good_num);
          scoreBoard.setCursor(11,0);
          scoreBoard.print("B:");
          scoreBoard.setCursor(13,0);
          scoreBoard.print(bad_num);
          scoreBoard.setCursor(0,1);
          scoreBoard.print("COMBO:");
          scoreBoard.print(combo_life);
          scoreBoard.setCursor(10,1);
          scoreBoard.print("Lv:");
          scoreBoard.setCursor(14,1);
          scoreBoard.print(Game_Level);
        }
        if (key_flag_up)
        {
          key_flag_up = 0;
          note_1 = beat_one(Row1, MAX_LEN);
          Serial.write(note_1);
        }
        if (sensor_flag)
        { 
          sensor_flag = 0;
          note_2 = beat_one(Row2, MAX_LEN);
          Serial.write(note_2);
        }
        //generate a note
        if(generate_flow_flag)
        {
          if(generate_row==0)
          Row1[0]= '*';
          else
          Row2[0]= '*';
          
          generate_flow_flag--;
        }
        else if (generate_flag)
        {
          one_generate('*');
          generate_flag = 0;
        }
        break;
  default: break;
  }
}
void T_Handler()
{   

    if(st == MENU)
    {
      sensor_value = analogRead(light_sensor);
      if (sensor_value < sensor_thres)
        sensor_value = 1;
      else
        sensor_value = 0;
      
      Unretrigerable_debounce(sensor_value,sensor_state,sensor_flag);
    }
    //soft timer counting
    if(st == PREGAME)
    { 
      if(game_start_timer++>game_start_max)
      { 
        //game start triger by counting to 3s
        next_st = GAME;
        game_start_timer = 0;
      }
    }

    if (scroll_timer++ > scroll_timer_max)
    {
      scroll_flag = 1;
      scroll_timer = 0;
    }

    if(st ==GAME)
    {
      if (generate_timer++> generate_timer_max&&generate_flag==0)
      {
        generate_flag = 1;
        generate_timer = 0;
      }
      if (generate_flow_timer > generate_flow_timer_max&&generate_flow_flag==0)
      {
        generate_flow_flag = random(2,10);
        generate_row = random(0,2);
        generate_flow_timer = 0;
      }

      debounce(digitalRead(button_up), key_state_up, key_flag_up, key_timer_up);
    }

}

void move_right(char *row,int size)
{ 
  //not beat
  if(row[size-1]!=' ')
  {
    bad_num++;
    combo_life = 0;
  }
  for(int i =size-2;i>=0;i--)
  {
    row[i+1] = row[i];
  }
  row[0]=' ';

}

void one_generate(char note)
{
  int x1 = random(0,2);
  if(x1==0)
    Row1[0] = note;
  else
    Row2[0] = note;
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
      if(i<2)
      {
        perfect_num++;
        combo_life++;
        return 'p';
      }
      else
        {good_num ++;
         combo_life++;
         result = row[size-i];
         row[size-i]=' ';
         return 'g';
        }
    } 
  }
  return 'b';
}

void debounce(int value,int &key_state,int &key_flag,int &key_timer)
{
  //key management
  //frequency + duration =0 -> end of the music
  switch(key_state)
  {
    case 0:
        if(value)
          {
            key_state= 1;
            key_flag = 1;
            key_timer = 0;
          }
          break;
    case 1:
      if(!value)
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
        if(!value)
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
  lcd.setCursor(0,1);
  lcd.print(Row2);
}


void show_buttom()
{
  lcd.setCursor(15,0);
  lcd.print(byte(0));
  lcd.setCursor(15,1);
  lcd.print(byte(1));
}


void game_initialize()
{
    scroll_timer = 0;
    scroll_flag = 0;
    generate_flag = 0;
    generate_flow_flag = 0;
    generate_flow_timer = 0;
    generate_timer = 0;
    sensor_flag = 0;
    sensor_timer = 0;
}

void Unretrigerable_debounce(int value,int &key_state,int &key_flag)
{
  switch(key_state)
  {
    case 0:
        if(value)
        {
          key_state = 1;
          key_flag = 1;
        }
        break;
    
    case 1:
        if(value==0)
        {
          key_state = 0;
        }
        break;
    default: break;
  }
}


int note_generate_calculation(int Game_Level)
{
  //basic:3 times as the scroll rate 
  int basic_speed = 3*scroll_timer_max;
  int GL_gain  = 6*Game_Level;
  return basic_speed-GL_gain;
}
int note_flow_generate_calculation(int Game_Level)
{
  //basic:6 times as the scroll rate 
  int basic_speed = 8*scroll_timer_max;
  int GL_gain  = 7*Game_Level;
  return basic_speed -GL_gain;
}