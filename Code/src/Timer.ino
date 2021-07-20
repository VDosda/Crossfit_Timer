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

void buzzerOn(int millisecond){
    buzzer.beep(millisecond);
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
      previousMillis = millis();
      setup_timer_flag = false;
    }
    if(SEC_TIMER > 0){    
      TimerCount(-1);
      
      if ( SEC_TIMER == 9 ){ // clean "0" display for 09 to 01 decount
          cleanDisplay(4);
          hide_fourth_display_flag = true;
       }
       if (SEC_TIMER < 4 && SEC_TIMER > 0){
          if (sec_tampon_buzzer != SEC_TIMER){
            sec_tampon_buzzer = SEC_TIMER;
            buzzerOn(500);
          }
       }
       if (SEC_TIMER == 0){
          hide_fourth_display_flag = false;
       }
       TimerDisplay("", 0, 0, SEC_TIMER, true, true, true, false, true);
    } else{
      timer_preparation_10_sec_flag = false;
      setup_timer_flag = true;
      
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

int tabTimeMapMS[2];
void timeMapMS(int Min, int Sec, int diffSec){
    
    Sec=Sec+diffSec;
    if (Sec < 0 and Min > 0){
      Sec = 59;
      Min = Min-1;
    }else if ( Sec < 0 and Min == 0){
      //Sec = 0;
    }else if (Sec > 59 and Min < 99){
      Sec = 0;
      Min = Min+1;
    }else if (Sec > 59 and Min == 99){
      Sec = 59;
    }
  tabTimeMapMS[0] = Min;
  tabTimeMapMS[1] = Sec;
}


bool H1_null_flag;
bool H2_null_flag;
void intervalTimerUP(){
   if(pre_start_flag){
        pre_start_flag=false;
        rounds_target_display_flag = (RD_TIMER_TARGET == 1 && ((MIN_TIMER_TARGET_H1 + SEC_TIMER_TARGET_H1) == 0 || (MIN_TIMER_TARGET_H2 + SEC_TIMER_TARGET_H2) == 0))? true : false;
        TimerDisplay("UP", RD_TIMER, MIN_TIMER, SEC_TIMER, !rounds_target_display_flag, rounds_target_display_flag, false, false, false);
    }
    if(!timer_preparation_10_sec_flag && running_flag){
       if(setup_timer_flag){
              setup_timer_flag=false;
              
              H1_null_flag = (MIN_TIMER_TARGET_H1 + SEC_TIMER_TARGET_H1) == 0;
              H2_null_flag = (MIN_TIMER_TARGET_H2 + SEC_TIMER_TARGET_H2) == 0;
               
              if(H1_null_flag && !H2_null_flag){                          //if there is only one interval set otherwize set H1 
                timeMapMS(MIN_TIMER_TARGET_H2, SEC_TIMER_TARGET_H2, -1);
                interval_flag = true;
              }else if(!H1_null_flag && H2_null_flag){
                timeMapMS(MIN_TIMER_TARGET_H1, SEC_TIMER_TARGET_H1, -1);
                interval_flag = true;
              }else{
                 timeMapMS(MIN_TIMER_TARGET_H1, SEC_TIMER_TARGET_H1, -1);
              }
              
              resetTimerTamps();
              rounds_target_display_flag = (RD_TIMER_TARGET == 1 && ( H1_null_flag || H2_null_flag ))? true : false;
              previousMillis = millis();
              buzzerOn(1000);
          }
        if(!(RD_TIMER == RD_TIMER_TARGET && rounds_flag)){
              TimerCount(1);
              if (rounds_flag){
                RD_TIMER = RD_TIMER + 1;
                buzzerOn(1000); //buzz each round
                rounds_flag = false;
              }
              
              if( (MIN_TIMER == tabTimeMapMS[0] && SEC_TIMER > tabTimeMapMS[1]) || (SEC_TIMER == 0 && tabTimeMapMS[1] == 59 && MIN_TIMER == tabTimeMapMS[0] + 1 )){
                MIN_TIMER = 0;
                SEC_TIMER = 0;
                
                if(interval_flag){
                  rounds_flag = true;
                }
                if(!H1_null_flag && !H2_null_flag){
                  interval_flag = !interval_flag;
                  interval_flag ? timeMapMS(MIN_TIMER_TARGET_H2, SEC_TIMER_TARGET_H2, -1) : timeMapMS(MIN_TIMER_TARGET_H1, SEC_TIMER_TARGET_H1, -1);
                }
                buzzerOn(800); //buzz each intervall
              }

              if(!(RD_TIMER == RD_TIMER_TARGET && rounds_flag)){ // at the end for a better display of numbers 
                TimerDisplay("UP", RD_TIMER, MIN_TIMER, SEC_TIMER, !rounds_target_display_flag, rounds_target_display_flag, false, false, false);
              }else{
                dots(1, true);
              }
              
          }else if(!pause_flag){
            //To finish on what it has been set
            if (MIN_TIMER_TARGET_H2+SEC_TIMER_TARGET_H2==0){
                TimerDisplay("", RD_TIMER, MIN_TIMER_TARGET_H1, SEC_TIMER_TARGET_H1, false, false, false, false, false);
            }else{
               TimerDisplay("", RD_TIMER, MIN_TIMER_TARGET_H2, SEC_TIMER_TARGET_H2, false, false, false, false, false);
            }
            running_flag = false;
            running_finish_flag = true;
            buzzerOn(1000);
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
            buzzerOn(1000);
        }
        if(!(RD_TIMER == RD_TIMER_TARGET && rounds_flag)){
            TimerCount(-1);
            if (rounds_flag){
              RD_TIMER = RD_TIMER + 1;
              buzzerOn(1000); //buzz each round
              rounds_flag = false;
            }
            if( MIN_TIMER == 0 && SEC_TIMER == 0 && !interval_flag ){
              MIN_TIMER = MIN_TIMER_TARGET_H2;
              SEC_TIMER = SEC_TIMER_TARGET_H2;
              interval_flag = true;
              buzzerOn(800); //buzz each intervall
            }
            if ( MIN_TIMER == 0 && SEC_TIMER == 0 && interval_flag ){
              MIN_TIMER = MIN_TIMER_TARGET_H1;
              SEC_TIMER = SEC_TIMER_TARGET_H1;
              interval_flag = false;
              rounds_flag = true;
              buzzerOn(800); //buzz each intervall
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
            buzzerOn(1000);
        }
      }else if(running_flag){
         TimerPreparationCountDown();
      }
}
