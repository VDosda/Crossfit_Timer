#include <SimpleTimer.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <Wire.h>
#include <DS3231.h>

//------------------------------------------------------------------------------------------- CLOCK
DS3231 RTC;
bool h12Flag;
bool pmFlag;
unsigned long current_time_for_RTC;
unsigned long interval;
int previous_RTC_time;
SimpleTimer timer_click_clock_reset;
bool clock_activate;
int previous_RTC_time_clock;
//------------------------------------------------------------------------------------------- BUZZER
#define BUZZER   7
SimpleTimer timer_buzzer_short;
SimpleTimer timer_buzzer_long;
int sec_tampon_buzzer;

//------------------------------------------------------------------------------------------- BUTTONS
#define BTN_START   8
#define BTN_MODE    9
#define BTN_UP      10
#define BTN_DOWN    11
#define BTN_RESET   12
bool buttonState_START;
bool buttonState_MODE;
bool buttonState_UP;
bool buttonState_DOWN;
bool buttonState_RESET;

bool one_push_btn_flag;

//------------------------------------------------------------------------------------------- SETUP
struct charInt2 { 
 char * mode;
 int tab[2];
};

charInt2 mode_tab[4] = {
  {"H1", {0,0}},
  {"H2", {0,0}},
  {"RD", {0,1}},
  {"CL", {0,0}}
};

bool setup_mode_active_flag;
int setup_mode_position_count;
int timer_mode_position_count;
int MODE;

SimpleTimer count_blinking_digit_on;
SimpleTimer count_blinking_digit_off;

bool up_down_mode_on_off_flag;
//------------------------------------------------------------------------------------------- LED MATRIX
#define LED_TYPE    WS2811
#define N_LEDS     46
#define PIN_LEDS_DIGIT       3
CRGB leds_digit[N_LEDS];
//------------------------------------------------------------------------------------------- BATTERY LEVEL
#define PIN_LED         2
CRGB led_battery_level[1]; //= Adafruit_NeoPixel(1, PIN_LED, NEO_GRB + NEO_KHZ800);
float sensor_value;
float voltage;
float voltage_tamp;
bool red_point_flag;
//------------------------------------------------------------------------------------------- DISPLAY
int unit_of_time_display[6]={-1,-1,-1,-1,-1,-1};
int unit_of_time_display_tamp[6]={-1,-1,-1,-1,-1,-1};
char * mode_display_tamp = "  ";

int BRIGHTNESS=200;


//------------------------------------------------------------------------------------------- DELAY timer
SimpleTimer timer_dots;
bool timer_dots_flag;

//------------------------------------------------------------------------------------------- TIMER

int SEC_TIMER, SEC_TIMER_TARGET_H1, SEC_TIMER_TARGET_H2;
int MIN_TIMER, MIN_TIMER_TARGET_H1, MIN_TIMER_TARGET_H2;
int H_TIMER, H_TIMER_TARGET_H1, H_TIMER_TARGET_H2;
int RD_TIMER, RD_TIMER_TARGET;
int total_H1_H2;
bool setup_timer_flag;
bool timer_preparation_10_sec_flag;
bool running_flag;
bool running_finish_flag;
bool pause_flag;
bool pre_start_flag;
bool interval_flag;
bool rounds_flag;
bool rounds_target_display_flag;

unsigned long previousMillis;
///////////////////////////////////////////////////////////////////////////////////////////////////////

void btnStartStopOk(){
  if (buttonState_START && one_push_btn_flag)
  { 
     total_H1_H2 = SEC_TIMER_TARGET_H1 + MIN_TIMER_TARGET_H1 + SEC_TIMER_TARGET_H2 + MIN_TIMER_TARGET_H2; // do nothing if total == 0;
     if(running_flag && !timer_preparation_10_sec_flag){
        pause_flag = !pause_flag;
        Serial.println("pause");
     }
     if(!setup_mode_active_flag && !running_flag && total_H1_H2 != 0 && !running_finish_flag){
        running_flag = true;
        Serial.println("Timer Running");
     }else if (setup_mode_active_flag){    //to set the start of the timer ( because of the blinking some leds can be off) and reset setup mode
        setup_mode_position_count = 2;
        resetTimerTamps();
        resetTimer();
        dots(1, true); //in case setup mode is on the round setup, to put back the dots
        Serial.println("setup ready");
     }
     setup_mode_active_flag = false;
     delay(300);
   }
}


void btnMode(){
  if (buttonState_MODE && one_push_btn_flag)
  {
      setUpModeBtn();
  }
}



void btnUpDown(){
 if (buttonState_UP || buttonState_DOWN)
   {
     if (setup_mode_active_flag){
          setUpTimerBtn();
     }else if (!running_flag && !running_finish_flag){
          up_down_mode_on_off_flag = true;
     }
   }
}

void btnReset(){
   if (buttonState_RESET && one_push_btn_flag && !clock_activate)
   {
      resetTimer();
      delay(300);
   }else if(buttonState_RESET){  //CLOCK RTC
      if (timer_click_clock_reset.isReady()){
         clock_activate = !clock_activate;
         if(clock_activate){
            MODE = 2;
            timer_click_clock_reset.reset();
            timer_mode_position_count = 3;
         }else{
            MODE = 0;
            timer_click_clock_reset.reset();
            resetTimer();
            timer_mode_position_count = 0;
          }
      }
   }else{
      timer_click_clock_reset.reset();
   }
}

void batteryLevel(){
 sensor_value = analogRead(A0);
 voltage = sensor_value*(3.7/1023); //Max 2.05/2.10   Min 1.65
 int color[3];
 if(int(voltage_tamp*10) != int(voltage*10)){
    voltage_tamp=voltage;
    if(voltage > 1.9 && voltage < 2.3)// && red_point_flag)
    {
      color[0]=0; color[1]=150; color[2]=0;
    }else{
      red_point_flag = false; //if voltage go under 3.2 ones, it has to stay red
      color[0]=150; color[1]=0; color[2]=0;
    }
    led_battery_level[0] = CRGB(color[0], color[1], color[2]);
    FastLED.show();
  }
}

//////////////////////////////////////  SETUP   //////////////////////////////////////
void setup()
{
  Serial.begin(115200);

  pinMode(BUZZER, OUTPUT);
  timer_buzzer_short.setInterval(400);
  timer_buzzer_long.setInterval(800);

  pinMode(BTN_START, INPUT);
  pinMode(BTN_MODE, INPUT);
  pinMode(BTN_UP, INPUT);
  pinMode(BTN_DOWN, INPUT);
  pinMode(BTN_RESET, INPUT);

  one_push_btn_flag = true;

  SEC_TIMER = 0;
  MIN_TIMER = 0;
  SEC_TIMER_TARGET_H1 = 0;
  SEC_TIMER_TARGET_H2 = 0;
  MIN_TIMER_TARGET_H1 = 0;
  MIN_TIMER_TARGET_H2 = 0;
  RD_TIMER_TARGET = 1;

  total_H1_H2 = 0;
  setup_mode_position_count = 2; // to star to 0 when click on btn (2+1->0)
  timer_mode_position_count = 0; //set setup mode on H1
  MODE = 0; //Mode set on UP

  //DOTS
  timer_dots_flag=true;
  dots(1, true);
  
  //TIMERS LIB
  timer_dots.setInterval(500);
  count_blinking_digit_on.setInterval(600);
  count_blinking_digit_off.setInterval(300);

  up_down_mode_on_off_flag = true;
  
  //TIMER CF
  running_flag = false;
  running_finish_flag = false;
  timer_preparation_10_sec_flag = true;
  pre_start_flag = true;
  setup_timer_flag=true;
  rounds_flag = false;
  interval_flag = false;
  pause_flag = false;

  interval=1000UL;

  //SETUP CF TIMER
  setup_mode_active_flag = false;
  
  //I2C for RTC
  Wire.begin();
  clock_activate = false;
  timer_click_clock_reset.setInterval(2000);
  mode_tab[3].tab[0] = RTC.getHour(h12Flag, pmFlag);
  mode_tab[3].tab[1] = RTC.getMinute();

  //battery level
  //battery level
  pinMode(A0, INPUT_PULLUP); 
  voltage_tamp = 0;
  red_point_flag = true;
  FastLED.addLeds<WS2811, PIN_LED, RGB>(led_battery_level, 1);

  FastLED.addLeds<WS2811, PIN_LEDS_DIGIT, GRB>(leds_digit, N_LEDS);
  
  //strip.begin();
}


//////////////////////////////////////  LOOP   //////////////////////////////////////
void loop(){

     setMillisecondFromRTC();// set Intervall millis() for exact second from RTC
  
    if (buttonState_START || buttonState_MODE || buttonState_UP || buttonState_DOWN || buttonState_RESET){
      one_push_btn_flag = false;
    }else{
      one_push_btn_flag = true;
    }
    
    //---------------------------- START/STOP/OK btn --------------------------------
     buttonState_START=digitalRead(BTN_START);
     btnStartStopOk();
     
     //---------------------------- MODE btn --------------------------------
     if (!running_flag){ //if the timer not running
         buttonState_MODE=digitalRead(BTN_MODE);
         btnMode();
     
         //---------------------------- UP DOWN btn --------------------------------  
         buttonState_UP=digitalRead(BTN_UP);
         buttonState_DOWN=digitalRead(BTN_DOWN);
         btnUpDown();
    
         // to choose btwn counting down or up mode
         if(up_down_mode_on_off_flag && !clock_activate){
            upDownTimerMode();
            up_down_mode_on_off_flag = false;
            delay(400);
            resetTimerTamps();
            dots(1, true);
            pre_start_flag = true; // to display right mode using the preStart of intervals functions
         }
     }
     if (running_flag || running_finish_flag){
      buzzer();
     }
     
    //--------------------------BTN RESET----------------------------------
    buttonState_RESET = digitalRead(BTN_RESET);
    if (!setup_mode_active_flag){ //in setup mode we shouldnt have the possibility to use reset btn
      btnReset();
    }
    
    //-------------------------------------------------------------
  
     if(setup_mode_active_flag){
        setUpTimerBlinking();
        setUpDisplay(); 
     }else if (!running_finish_flag){ // to make the buzzer works at the end of timer only ones
        switch(MODE){
          case 0:{
              intervalTimerUP();
            break;
          }
          case 1:{
              intervalTimerDOWN();
            break;
          }
          case 2:{
             clockRTC();
             break;
          }
        }
     }

    batteryLevel();
}
