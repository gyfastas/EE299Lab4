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
#define FLOW 61   //=

#define MAX_LEN 15
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
const int light_sensor = A1;

//sensor
const int sensor_thres = 200;
int sensor_value = 0;
int sensor_state = 0;
int sensor_timer = 0;
int sensor_flag  = 0;

//counter 1 time = soft_time_x *10 ms
int soft_timer1_max =50;
int soft_timer1 = 0;
int Soft_flag_1 = 0;

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

//play_flag
int play_flag =0;

//perfect,good,bad number
int perfect_num  = 0;

int good_num = 0;

int bad_num = 0;

// key variables
int key_state_up = 0;
int key_flag_up = 0;
int key_timer_up = 0;

//print state
int Game_Level = 0;
//game start count
int game_start_max = 300;//game start in 3s
int game_start_timer = 0;


const int period = 2;

void game_initialize();

void move_right(char *row,int size);

void one_generate(char note);

char beat_one(char *row,int size);

void lcd_flash();

//debounce function
void debounce(int button_up,int &key_state,int &key_flag,int &key_timer);

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
  MsTimer2::set(timer2_period,T_Handler);
  MsTimer2::start();

  pinMode(button_up,INPUT);
  pinMode(light_sensor,INPUT);
  
  Serial.begin(9600);
  lcd.createChar(0,buttom);
  lcd.createChar(1,buttom);
  //initialize the row
  for(int i =0;i<15;i++)
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
    //jump function(trigger once)
    switch(st)
    {
      case MENU:
                lcd.clear();
                lcd.print("START");
                lcd.setCursor(0,1);
                lcd.print("LEVEL:");
                break;
      case PREGAME:
                game_start_timer = 0;
                lcd.clear();
                lcd.print("START IN");
                break;            
      case GAME:
                //show buttom lines
                //initiaizing
                game_initialize();
                show_buttom();
                break;
      default:break;
    }
    st = next_st;
    
  }


  switch(st)
  {
  
  case MENU:
        if(sensor_flag)
        {
          sensor_flag = 0;
          if(Game_Level++>5)
            Game_Level = 0;
          
          soft_timer1_max = 50-Game_Level*5;
          generate_flow_timer_max = soft_timer1_max*8-Game_Level*5;
          generate_timer_max = soft_timer1_max*4-Game_Level*10;
        }
        // key -> ready to start
        if(key_flag_up)
        {
          key_flag_up = 0;
          next_st = PREGAME;
        }

        if(Soft_flag_1)
        {
          Soft_flag_1 = 0;
          lcd.setCursor(6,1);
          lcd.print(Game_Level);
        }
        break;
  
  case PREGAME:
        if(Soft_flag_1)
        {
          Soft_flag_1 = 0;
          lcd.setCursor(8,0);
          lcd.print((game_start_max-game_start_timer)/100 +1);
        }
        break;
  case GAME:
        //Scroll right
        if (Soft_flag_1)
        {
          move_right(Row1, 15);
          move_right(Row2, 15);
          lcd_flash();
          //reset timer
          Soft_flag_1 = 0;
        }

        //generate a note
        if(generate_flow_flag)
        {
          one_generate(FLOW);
          generate_flow_flag--;
        }
        else if (generate_flag)
        {
          one_generate(SINGLE);
          generate_flag = 0;
        }

        if (key_flag_up)
        {
          key_flag_up = 0;
          play_flag = 1;
        }

        if (play_flag)
        {
          play_flag = 0;
          note_1 = beat_one(Row1, 15);
          Serial.write(note_1);
        }

        if (sensor_flag)
        { 
          sensor_flag  = 0;
          note_2 = beat_one(Row2, 15);
          Serial.write(note_2);
        }
        break;
  default: break;
  }
}
void T_Handler()
{
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
    if (soft_timer1++ > soft_timer1_max)
    {
      Soft_flag_1 = 1;
      soft_timer1 = 0;
    }
    if(st ==GAME)
    {
      if (generate_timer++> generate_timer_max)
      {
        generate_flag = 1;
        generate_timer = 0;
      }
      if (generate_flow_timer > generate_flow_timer_max)
      {
        generate_flow_flag = random(2,10);
        generate_flow_timer = 0;
      }
      
    }
    //sensor flag generate
    sensor_value = analogRead(light_sensor);
    if (sensor_value < sensor_thres)
      sensor_value = 1;
    else
      sensor_value = 0;

    debounce(sensor_value,sensor_state,sensor_flag,sensor_timer);
    debounce(digitalRead(button_up), key_state_up, key_flag_up, key_timer_up);
}

void move_right(char *row,int size)
{ 
  //not beat
  if(row[size-1]!=' ')
  {
    bad_num++;
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
        perfect_num++;
      else
        good_num ++;
      result = row[size-i];
      row[size-i]=' ';
      return result;
    } 
  }
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
    soft_timer1 = 0;
    Soft_flag_1 = 0;
    generate_flag = 0;
    generate_flow_flag = 0;
    generate_flow_timer = 0;
    generate_timer = 0;
    sensor_flag = 0;
    sensor_timer = 0;
}
