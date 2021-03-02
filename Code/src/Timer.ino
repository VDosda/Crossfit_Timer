void clockRTC(){
    TimerDisplay("", 0, RTC.getHour(h12Flag, pmFlag), RTC.getMinute(), true, true, false, false, true);
    if (RTC.getSecond() != previous_RTC_time_clock){
      previous_RTC_time_clock = RTC.getSecond();
      timer_dots.reset();
    }
    if (timer_dots.isReady() && !timer_dots_flag) {
        dots(1, false);
        timer_dots_flag=true;
    }else if(!timer_dots.isReady() && timer_dots_flag){
        dots(1, true);
        timer_dots_flag=false;
    }
}

void buzzer(){
  if (!timer_buzzer_long.isReady() || !timer_buzzer_short.isReady()){
    tone(BUZZER, 500);
    delay(100);
  }else{
    noTone(BUZZER);
  }
}


void setMillisecondFromRTC(){
  if (RTC.getSecond() != previous_RTC_time){
    interval = millis() - current_time_for_RTC;
    previous_RTC_time = RTC.getSecond();
    current_time_for_RTC = millis();
  }
}

void TimerPreparationCountDown(){
    if(setup_timer_flag){
      TimerDisplay("", 0, 0, 10, true, true, true, false, true);
      SEC_TIMER = 10;
      previousMillis=millis();
      setup_timer_flag=false;
    }
    if(SEC_TIMER > 0){    
       TimerCount(-1);
      /*if ( SEC_TIMER == 9 ){ // clean "0" digit for 09 to 01 decount
          cleanDigit(4);
          strip.show();
       }*/
       if (SEC_TIMER < 3 && SEC_TIMER > 0){
          if (sec_tampon_buzzer != SEC_TIMER){
            sec_tampon_buzzer=SEC_TIMER;
            timer_buzzer_short.reset();
          }
       }
       TimerDisplay("", 0, 0, SEC_TIMER, true, true, true, false, true);
    } else{
      timer_preparation_10_sec_flag=false;
      setup_timer_flag=true;
    }
}

unsigned long pause_diff_time;
bool pause_save_millis_flag;
void TimerCount(int downUp){

       unsigned long currentMillis = millis();
       if (pause_flag && pause_save_millis_flag){
          pause_diff_time = currentMillis - previousMillis;
          pause_save_millis_flag = false;
       }else if (pause_flag){
          previousMillis = currentMillis - pause_diff_time;
       }else{
           if ((unsigned long)(currentMillis - previousMillis) >= interval) {
              SEC_TIMER = SEC_TIMER + downUp;
              previousMillis = currentMillis;
           }
           if (SEC_TIMER > 59 || SEC_TIMER < 0) {
              downUp == 1 ? SEC_TIMER = 0 : SEC_TIMER = 59;
              MIN_TIMER = MIN_TIMER + downUp;
           }
           if (MIN_TIMER > 59 || MIN_TIMER < 0) {
              downUp == 1 ? MIN_TIMER = 0 : MIN_TIMER = 59;
              H_TIMER = H_TIMER + downUp;
           }
           if (H_TIMER > 24 || H_TIMER < 0) {
              H_TIMER = 0;
           }
           pause_save_millis_flag = true;
       }
}

void intervalTimerUP(){
   if(pre_start_flag){
        pre_start_flag=false;
        rounds_target_display_flag = (RD_TIMER_TARGET == 1 && ((MIN_TIMER_TARGET_H1 + SEC_TIMER_TARGET_H1) == 0 || (MIN_TIMER_TARGET_H2 + SEC_TIMER_TARGET_H2) == 0))? true : false;
        TimerDisplay("UP", RD_TIMER, MIN_TIMER, SEC_TIMER, !rounds_target_display_flag, rounds_target_display_flag, false, false, false);
    }
    if(!timer_preparation_10_sec_flag && running_flag){
       if(setup_timer_flag){
              setup_timer_flag=false;
              resetTimerTamps();
              rounds_target_display_flag = (RD_TIMER_TARGET == 1 && ((MIN_TIMER_TARGET_H1 + SEC_TIMER_TARGET_H1) == 0 || (MIN_TIMER_TARGET_H2 + SEC_TIMER_TARGET_H2) == 0))? true : false;
              previousMillis = millis();
              timer_buzzer_long.reset();
          }
        if(!(RD_TIMER == RD_TIMER_TARGET && rounds_flag)){
              TimerCount(1);
              if (rounds_flag){
                RD_TIMER = RD_TIMER + 1;
                timer_buzzer_long.reset();
                rounds_flag = false;
              }
              if( MIN_TIMER == MIN_TIMER_TARGET_H1 && SEC_TIMER > SEC_TIMER_TARGET_H1 && !interval_flag ){
                MIN_TIMER = 0;
                SEC_TIMER = 1;
                interval_flag = true;
              }
              if ( MIN_TIMER == MIN_TIMER_TARGET_H2 && SEC_TIMER > SEC_TIMER_TARGET_H2 && interval_flag ){
                MIN_TIMER = 0;
                SEC_TIMER = 1;
                interval_flag = false;
                rounds_flag = true;
              }
              if(!(RD_TIMER == RD_TIMER_TARGET && rounds_flag)){ // at the end for a better display of numbers 
                TimerDisplay("UP", RD_TIMER, MIN_TIMER, SEC_TIMER, !rounds_target_display_flag, rounds_target_display_flag, false, false, false);
              }else{
                dots(1, true);
              }
          }else if(!pause_flag){
            running_flag = false;
            running_finish_flag = true;
            timer_buzzer_long.reset();
          }
      }else if(running_flag){
         TimerPreparationCountDown();
      }
}


void intervalTimerDOWN(){
     if(pre_start_flag){
        pre_start_flag=false;
        rounds_target_display_flag = (RD_TIMER_TARGET == 1 && ((MIN_TIMER_TARGET_H1 + SEC_TIMER_TARGET_H1) == 0 || (MIN_TIMER_TARGET_H2 + SEC_TIMER_TARGET_H2) == 0))? true : false;
        TimerDisplay("DN", RD_TIMER, MIN_TIMER, SEC_TIMER, !rounds_target_display_flag, rounds_target_display_flag, false, false, false);
     }
     if(!timer_preparation_10_sec_flag && running_flag){
       if(setup_timer_flag){
            setup_timer_flag=false;
            resetTimerTamps();
            rounds_target_display_flag = (RD_TIMER_TARGET == 1 && ((MIN_TIMER_TARGET_H1 + SEC_TIMER_TARGET_H1) == 0 || (MIN_TIMER_TARGET_H2 + SEC_TIMER_TARGET_H2) == 0))? true : false;
            MIN_TIMER = MIN_TIMER_TARGET_H1;
            SEC_TIMER = SEC_TIMER_TARGET_H1;
            previousMillis = millis();
            timer_buzzer_long.reset();
        }
        if(!(RD_TIMER == RD_TIMER_TARGET && rounds_flag)){
            TimerCount(-1);
            if (rounds_flag){
              RD_TIMER = RD_TIMER + 1;
              timer_buzzer_long.reset();
              rounds_flag = false;
            }
            if( MIN_TIMER == 0 && SEC_TIMER == 0 && !interval_flag ){
              MIN_TIMER = MIN_TIMER_TARGET_H2;
              SEC_TIMER = SEC_TIMER_TARGET_H2;
              interval_flag = true;
            }
            if ( MIN_TIMER == 0 && SEC_TIMER == 0 && interval_flag ){
              MIN_TIMER = MIN_TIMER_TARGET_H1;
              SEC_TIMER = SEC_TIMER_TARGET_H1;
              interval_flag = false;
              rounds_flag = true;
            }
            if(!(RD_TIMER == RD_TIMER_TARGET && rounds_flag)){ // at the end for a better display of numbers 
              TimerDisplay("DN", RD_TIMER, MIN_TIMER, SEC_TIMER, !rounds_target_display_flag, rounds_target_display_flag, false, false, false);
            }else{
              dots(1, true);
              TimerDisplay("DN", RD_TIMER, 0, 0, !rounds_target_display_flag, rounds_target_display_flag, false, false, false);
            }
        }else if(!pause_flag){
            running_flag = false;
            running_finish_flag = true;
            timer_buzzer_long.reset();
        }
      }else if(running_flag){
         TimerPreparationCountDown();
      }
}
