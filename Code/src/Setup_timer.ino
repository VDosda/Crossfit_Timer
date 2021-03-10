void setUpDisplay(){
  if(timer_mode_position_count == 2){//RD mode
        TimerDisplay(mode_tab[timer_mode_position_count].mode, 0, 0, mode_tab[timer_mode_position_count].tab[1], false, true, true, false, true);
        dots(1, false);
  }else{
        TimerDisplay(mode_tab[timer_mode_position_count].mode, 0, mode_tab[timer_mode_position_count].tab[0], mode_tab[timer_mode_position_count].tab[1], false, true, false, false, true);
        dots(1, true);
  }
}

void setMinutes(){ 
if(timer_mode_position_count == 0 ){
       MIN_TIMER_TARGET_H1 = mode_tab[timer_mode_position_count].tab[0];
    }
   if(timer_mode_position_count == 1 ){
       MIN_TIMER_TARGET_H2 = mode_tab[timer_mode_position_count].tab[0]; 
    }
    if(timer_mode_position_count == 3 ){
       RTC.setHour(mode_tab[timer_mode_position_count].tab[0]); 
    }
  }

void setSeconds(){
  if(timer_mode_position_count == 0 ){
       SEC_TIMER_TARGET_H1 = mode_tab[timer_mode_position_count].tab[1];
    }
   if(timer_mode_position_count == 1 ){
       SEC_TIMER_TARGET_H2 = mode_tab[timer_mode_position_count].tab[1]; 
    }
    if(timer_mode_position_count == 2 ){
      RD_TIMER_TARGET = mode_tab[timer_mode_position_count].tab[1];
    }
    if(timer_mode_position_count == 3 ){
       RTC.setMinute(mode_tab[timer_mode_position_count].tab[1]); 
    }
  }



////////////////////////////////////// BTN
void setUpModeBtn(){
  setup_mode_active_flag = true;
    setup_mode_position_count = setup_mode_position_count+1;
    if (timer_mode_position_count == 3){     //if Clock mode
        if (setup_mode_position_count > 2){
          setup_mode_position_count = 1;
        }
    }else if (timer_mode_position_count == 2){ //if Round mode
        dots(1, false);
        cleanDisplay(2);
        cleanDisplay(3);
        if(setup_mode_position_count > 0 && setup_mode_position_count < 2){ 
          setup_mode_position_count = 2;
        }
        else if (setup_mode_position_count > 2){
          setup_mode_position_count = 0;
        }
     }else{ 
        if (setup_mode_position_count > 2){
          setup_mode_position_count = 0;
        }
     }

  delay(300);
}

void setUpTimerBtn(){
  int up_down_count;
  up_down_count = (buttonState_UP) ? 1 : -1;

  SEC_TIMER = 0;
  MIN_TIMER = 0;
  H_TIMER = 0;
  RD_TIMER = 1;
  
  switch(setup_mode_position_count){    
    case 0:{
      timer_mode_position_count = timer_mode_position_count + up_down_count;
      if (timer_mode_position_count > 2){ timer_mode_position_count = 0; }
      if (timer_mode_position_count < 0){ timer_mode_position_count = 2; }
      break;
    }
    case 1:{
      mode_tab[timer_mode_position_count].tab[0] = mode_tab[timer_mode_position_count].tab[0] + up_down_count;
      if (timer_mode_position_count == 3){ // interval mode
        if (mode_tab[timer_mode_position_count].tab[0] > 24){ mode_tab[timer_mode_position_count].tab[0] = 1; }
        if (mode_tab[timer_mode_position_count].tab[0] < 0){ mode_tab[timer_mode_position_count].tab[0] = 24; }
      }else{
        if (mode_tab[timer_mode_position_count].tab[0] > 59){ mode_tab[timer_mode_position_count].tab[0] = 0; }
        if (mode_tab[timer_mode_position_count].tab[0] < 0){ mode_tab[timer_mode_position_count].tab[0] = 59; }
      }
      setMinutes();
      break;
    }
    case 2:{
      mode_tab[timer_mode_position_count].tab[1] =  mode_tab[timer_mode_position_count].tab[1] + up_down_count;
      if (timer_mode_position_count == 2){ // interval mode
        if (mode_tab[timer_mode_position_count].tab[1] > 99){ mode_tab[timer_mode_position_count].tab[1] = 1; }
        if (mode_tab[timer_mode_position_count].tab[1] < 1){ mode_tab[timer_mode_position_count].tab[1] = 99; }
      }else{
        if (mode_tab[timer_mode_position_count].tab[1] > 59){ mode_tab[timer_mode_position_count].tab[1] = 0; }
        if (mode_tab[timer_mode_position_count].tab[1] < 0){ mode_tab[timer_mode_position_count].tab[1]  = 59; }
      }
      setSeconds();
      break;
    }
  }
  delay(200);
}

////////////////////////////////// BLINKING
void setUpTimerBlinking(){
     if (count_blinking_7seg_on.isReady()) {
            resetTimerTamps();
            count_blinking_7seg_on.reset();
            count_blinking_7seg_off.reset();
       }else if(count_blinking_7seg_off.isReady() && !(buttonState_UP || buttonState_DOWN)){
           switch(setup_mode_position_count){    
            case 0:{
                  cleanDisplay(0);
                  cleanDisplay(1);
              break;
            }
            case 1:{
                cleanDisplay(2);
                cleanDisplay(3);
              break;
            }
            case 2:{
                cleanDisplay(4);
                cleanDisplay(5);
              break;
            }
          }
          FastLED.show();
      }
}

//////////////////////////////////// UP DOWN MODE DISPLAY
int up_down_message[2];
void upDownTimerMode(){
  cleanDisplay();
  up_down_message[0] = MINUS;
  up_down_message[1] = MINUS;
  if(buttonState_UP){
      up_down_message[0] = U;
      up_down_message[1] = P;
      MODE = 0;
    }
  else if (buttonState_DOWN){
      up_down_message[0] = D;
      up_down_message[1] = N;
      MODE = 1;
    }
  
  displayLeds(up_down_message[0], 0, 0, BRIGHTNESS, 0);
  displayLeds(up_down_message[1], 1, 0, BRIGHTNESS, 0);
  for(int i = 2; i < 6; i++){
    displayLeds(MINUS, i, BRIGHTNESS,0, 0);
  }

  resetTimer();
}

//////////////////////////////////// RESET MODE
void resetTimer(){
  if(MODE == 0){
      SEC_TIMER = 0;
      MIN_TIMER = 0;
    }
  if(MODE == 1){
      SEC_TIMER = SEC_TIMER_TARGET_H1;
      MIN_TIMER = MIN_TIMER_TARGET_H1;
    }
    RD_TIMER = 1;

    resetTimerTamps();
    
    running_flag = false;
    running_finish_flag = false;
    pause_flag = false;
    timer_preparation_10_sec_flag = true;
    pre_start_flag = true;
    setup_timer_flag=true;
    rounds_flag = false;
    interval_flag = false;
}
