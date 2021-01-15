void setUpDisplay(){
  if(timer_mode_position_count == 2){//RD mode
        TimerDisplay(mode_tab[timer_mode_position_count].mode, 0, 0, 0, mode_tab[timer_mode_position_count].time_m_s[1], false, true,  true, true, false, true);
        dots(1, false);
  }else{
        TimerDisplay(mode_tab[timer_mode_position_count].mode, 0,  0, mode_tab[timer_mode_position_count].time_m_s[0], mode_tab[timer_mode_position_count].time_m_s[1], false, true, true, false, false, true);
        dots(1, true);
  }
}

void setMinutes(){ 
if(timer_mode_position_count == 0 ){
       MIN_TIMER_TARGET_H1 = mode_tab[timer_mode_position_count].time_m_s[0];
    }
   if(timer_mode_position_count == 1 ){
       MIN_TIMER_TARGET_H2 = mode_tab[timer_mode_position_count].time_m_s[0]; 
    }
    if(timer_mode_position_count == 3 ){
       RTC.setHour(mode_tab[timer_mode_position_count].time_m_s[0]); 
    }
  }

void setSeconds(){
  if(timer_mode_position_count == 0 ){
       SEC_TIMER_TARGET_H1 = mode_tab[timer_mode_position_count].time_m_s[1];
    }
   if(timer_mode_position_count == 1 ){
       SEC_TIMER_TARGET_H2 = mode_tab[timer_mode_position_count].time_m_s[1]; 
    }
    if(timer_mode_position_count == 2 ){
      RD_TIMER_TARGET = mode_tab[timer_mode_position_count].time_m_s[1];
    }
    if(timer_mode_position_count == 3 ){
       RTC.setMinute(mode_tab[timer_mode_position_count].time_m_s[1]); 
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
        cleanDigit(2);
        cleanDigit(3);
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
      mode_tab[timer_mode_position_count].time_m_s[0] = mode_tab[timer_mode_position_count].time_m_s[0] + up_down_count;
      if (timer_mode_position_count == 3){ // interval mode
        if (mode_tab[timer_mode_position_count].time_m_s[0] > 24){ mode_tab[timer_mode_position_count].time_m_s[0] = 1; }
        if (mode_tab[timer_mode_position_count].time_m_s[0] < 0){ mode_tab[timer_mode_position_count].time_m_s[0] = 24; }
      }else{
        if (mode_tab[timer_mode_position_count].time_m_s[0] > 59){ mode_tab[timer_mode_position_count].time_m_s[0] = 0; }
        if (mode_tab[timer_mode_position_count].time_m_s[0] < 0){ mode_tab[timer_mode_position_count].time_m_s[0] = 59; }
      }
      setMinutes();
      break;
    }
    case 2:{
      mode_tab[timer_mode_position_count].time_m_s[1] =  mode_tab[timer_mode_position_count].time_m_s[1] + up_down_count;
      if (timer_mode_position_count == 2){ // interval mode
        if (mode_tab[timer_mode_position_count].time_m_s[1] > 99){ mode_tab[timer_mode_position_count].time_m_s[1] = 1; }
        if (mode_tab[timer_mode_position_count].time_m_s[1] < 1){ mode_tab[timer_mode_position_count].time_m_s[1] = 99; }
      }else{
        if (mode_tab[timer_mode_position_count].time_m_s[1] > 59){ mode_tab[timer_mode_position_count].time_m_s[1] = 0; }
        if (mode_tab[timer_mode_position_count].time_m_s[1] < 0){ mode_tab[timer_mode_position_count].time_m_s[1]  = 59; }
      }
      setSeconds();
      break;
    }
  }
  delay(200);
}

////////////////////////////////// BLINKING
void setUpTimerBlinking(){
     if (count_blinking_digit_on.isReady()) {
            resetTimerTamps();
            count_blinking_digit_on.reset();
            count_blinking_digit_off.reset();
       }else if(count_blinking_digit_off.isReady() && !(buttonState_UP || buttonState_DOWN)){
           switch(setup_mode_position_count){    
            case 0:{
                  cleanDigit(0);
                  cleanDigit(1);
              break;
            }
            case 1:{
                cleanDigit(2);
                cleanDigit(3);
              break;
            }
            case 2:{
                cleanDigit(4);
                cleanDigit(5);
              break;
            }
          }
          strip.show(); 
      }
}

//////////////////////////////////// UP DOWN MODE DISPLAY
char* up_down_message;
void upDownTimerMode(){
  cleanDisplay();
  up_down_message = "--";
  if(buttonState_UP){up_down_message = "UP"; MODE = 0;}
  else if (buttonState_DOWN){up_down_message = "DN"; MODE = 1;}
  displayLeds(up_down_message, 0, 0, 200/BRIGHTNESS, 0);
  displayLeds("----", 2, 200/BRIGHTNESS,0, 0);
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
