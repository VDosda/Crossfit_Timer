/*PROTO
 * int ledMatrixDigit[6][7] = {
   { 0,  1,  2,  3,  4,  5,  6},  
   { 8,  7,  11, 10, 9, 13, 12},
   {17, 16, 20, 19, 18, 22, 21},
   {23, 24, 25, 26, 27, 28, 29},
   {32, 33, 34, 35, 36, 37, 38},
   {40, 39, 43, 42, 41, 45, 44},
};*/ 
int ledMatrixDigit[6][7] = {
   { 0,  1,  2,  3,  4,  5,  6},  
   { 7,  8,  9, 10, 11, 12, 13},
   {16, 17, 18, 19, 20, 21, 22},
   {23, 24, 25, 26, 27, 28, 29},
   {32, 33, 34, 35, 36, 37, 38},
   {39, 40, 41, 42, 43, 44, 45},
};
int ledMatrixDots[2][2]= {
   {14, 15},
   {31, 30},
};
#define digitDataLeds_TAB_SIZE    40
DigitDataLeds digitDataLeds[digitDataLeds_TAB_SIZE] = {
{'0',{0, 1, 2, 4, 5, 6}, 6},
{'1',{5, 6}, 2},
{'2',{0, 2, 3, 4, 6}, 5},
{'3',{2, 3, 4, 5, 6}, 5},
{'4',{1, 3, 5, 6}, 4},
{'5',{1, 2, 3, 4, 5}, 5},
{'6',{0, 1, 2, 3, 4, 5}, 6},
{'7',{2, 5, 6}, 3},
{'8',{0, 1, 2, 3, 4, 5, 6}, 7},
{'9',{1, 2, 3, 4, 5, 6}, 6},

{'A', {0, 1, 2, 3, 5, 6}, 6},
{'B', {0, 1, 3, 4, 5}, 5},
{'C', {0, 1, 2, 4}, 4},
{'D', {0, 3, 4, 5, 6}, 5},
{'E', {0, 1, 2, 3, 4}, 5},
{'F', {0, 1, 2, 3}, 4},
{'G', {0, 1, 2, 3, 4, 5, 6}, 7},
{'H', {0, 1, 3, 5, 6}, 5},
{'I', {5, 6}, 2},
{'J', {0, 1, 2, 3, 4, 5, 6}, 7},
{'K', {0, 1, 2, 3, 4, 5, 6}, 7},
{'L', {0, 1, 4}, 3},
{'M', {0, 1, 2, 5, 6}, 5},
{'N', {0, 3, 5}, 3},
{'O', {0, 1, 2, 4, 5, 6}, 6},
{'P', {0, 1, 2, 3, 6}, 5},
{'Q', {0, 1, 2, 3, 4, 5, 6}, 7},
{'R', {0, 3}, 2},
{'S', {0, 1, 2, 3, 4, 5, 6}, 7},
{'T', {0, 1, 3, 4}, 4},
{'U', {0, 1, 4, 5, 6}, 5},
{'V', {0, 1, 2, 3, 4, 5, 6}, 7},
{'W', {0, 1, 2, 3, 4, 5, 6}, 7},
{'X', {0, 1, 2, 3, 4, 5, 6}, 7},
{'Y', {0, 1, 2, 3, 4, 5, 6}, 7},
{'Z', {0, 1, 2, 3, 4, 5, 6}, 7},

{' ', {}, 0},
{'-', {3}, 1}
};

TabData letterTabToDisplay;

//////////////////////////////////////////////////////  FUNCTIONS ////////////////////////////////////////////////////////

void cleanDisplay(){
    for(int i=0; i<6; i++){
      for(int j=0; j<7; j++){
         strip.setPixelColor(ledMatrixDigit[i][j], strip.Color(0, 0, 0));
      }
  }
  dots(0, false);
  dots(1, false);
}

void cleanDigit(int digit){
  for(int i=0; i<7; i++){
     strip.setPixelColor(ledMatrixDigit[digit][i], strip.Color(0, 0, 0));
  }
}

void resetTimerTamps(){
     for(int i=0; i<6; i++){ //reset all digit to display time
        unit_of_time_display_tamp[i]=-1;
      }
      mode_display_tamp="  ";
    }

void dots(int positionDots, bool state){
  if(state){
    strip.setPixelColor(ledMatrixDots[positionDots][0], strip.Color(200/BRIGHTNESS, 0, 0));
    strip.setPixelColor(ledMatrixDots[positionDots][1], strip.Color(200/BRIGHTNESS, 0, 0));
  }else{
    strip.setPixelColor(ledMatrixDots[positionDots][0], strip.Color(0, 0, 0));
    strip.setPixelColor(ledMatrixDots[positionDots][1], strip.Color(0, 0, 0));
  }
  strip.show();
}


void displayLeds(String message, int px, int r, int g, int b){
  message.toUpperCase();
  letterTabToDisplay.tabSize=message.length();
  for(int i=0; i<message.length(); i++){
     for(int j=0; j<digitDataLeds_TAB_SIZE; j++){
        if (message.charAt(i)==digitDataLeds[j].digit){
           letterTabToDisplay.address[i]=j;
        }
     }
  }
  for(int i=0; i<letterTabToDisplay.tabSize; i++){
      if((i+px)<6 && (i+px)>=0) {
        for(int j=0; j<digitDataLeds[letterTabToDisplay.address[i]].tabSize; j++){
          strip.setPixelColor(ledMatrixDigit[i+px][digitDataLeds[letterTabToDisplay.address[i]].ledsAddress[j]] , strip.Color(r, g, b));
        }
      }
    }
  strip.show();
}

void TimerDisplay(char * mode, int r, int h, int m, int s,  bool hideModeFlag, bool hideRoundsFlag, bool hideHoursFlag, bool hideMinutesFlag, bool hideSecondsFlag, bool hideDotsFlag){

//---------------------------------------------------------DIGIT 0 1
  if(!hideModeFlag && hideHoursFlag && hideRoundsFlag){
      if (mode_display_tamp != mode){
          cleanDigit(0);
          cleanDigit(1);
          dots(0, false);
          displayLeds(mode, 0, 0, 200/BRIGHTNESS, 0);
          mode_display_tamp = mode;
      }
  }
  if(!hideHoursFlag && hideRoundsFlag && hideModeFlag){
       unit_of_time_display[0]=abs(h/10);
       unit_of_time_display[1]=h-abs(h/10)*10;
      
      dots(0, true);
      
      if ( unit_of_time_display_tamp[0]!= unit_of_time_display[0]){
          cleanDigit(0);
          displayLeds(String( unit_of_time_display[0]), 0, 200/BRIGHTNESS, 0, 0);
          unit_of_time_display_tamp[0]= unit_of_time_display[0];
      }
      if ( unit_of_time_display_tamp[1]!= unit_of_time_display[1]){   
          cleanDigit(1);
          displayLeds(String( unit_of_time_display[1]), 1, 200/BRIGHTNESS, 0, 0);
          unit_of_time_display_tamp[1]= unit_of_time_display[1];
      }
  }

  if(!hideRoundsFlag && hideHoursFlag && hideModeFlag){
      unit_of_time_display[0]=abs(r/10);
      unit_of_time_display[1]=r-abs(r/10)*10;
      
      dots(0, false);
      
      if ( unit_of_time_display_tamp[0]!= unit_of_time_display[0]){
          cleanDigit(0);
          displayLeds(String( unit_of_time_display[0]), 0, 0, 200/BRIGHTNESS, 0);
          unit_of_time_display_tamp[0]= unit_of_time_display[0];
      }
      if ( unit_of_time_display_tamp[1]!= unit_of_time_display[1]){   
          cleanDigit(1);
          displayLeds(String( unit_of_time_display[1]), 1, 0, 200/BRIGHTNESS, 0);
          unit_of_time_display_tamp[1]= unit_of_time_display[1];
      }
  }

  if(hideModeFlag && hideHoursFlag && hideRoundsFlag){
    cleanDigit(0);
    cleanDigit(1);
    dots(0, false);
  }
//---------------------------------------------------------DIGIT 2 3
  if(!hideMinutesFlag){
     unit_of_time_display[2]=abs(m/10);
     unit_of_time_display[3]=m-abs(m/10)*10;
    
    if ( unit_of_time_display_tamp[2]!= unit_of_time_display[2]){
        cleanDigit(2);
        displayLeds(String( unit_of_time_display[2]), 2, 200/BRIGHTNESS, 0, 0);
        unit_of_time_display_tamp[2]= unit_of_time_display[2];
    }
    if ( unit_of_time_display_tamp[3]!= unit_of_time_display[3]){
        cleanDigit(3);
        displayLeds(String( unit_of_time_display[3]), 3, 200/BRIGHTNESS, 0, 0);
        unit_of_time_display_tamp[3]= unit_of_time_display[3];
    }
  }else {
    cleanDigit(2);
    cleanDigit(3);
    dots(1, false);
  }
//---------------------------------------------------------DIGIT 4 5  
   unit_of_time_display[4]=abs(s/10);
   unit_of_time_display[5]=s-abs(s/10)*10;
  if(!hideSecondsFlag){
    if ( unit_of_time_display_tamp[4]!= unit_of_time_display[4]){
        cleanDigit(4);
        displayLeds(String( unit_of_time_display[4]), 4, 200/BRIGHTNESS, 0, 0);
        unit_of_time_display_tamp[4]= unit_of_time_display[4];
    }
    if ( unit_of_time_display_tamp[5]!= unit_of_time_display[5]){
        cleanDigit(5);
        displayLeds(String( unit_of_time_display[5]), 5, 200/BRIGHTNESS, 0, 0);
        unit_of_time_display_tamp[5]= unit_of_time_display[5];
        if(!hideDotsFlag){
          timer_dots.reset();
        } 
    }
  }else {
    cleanDigit(4);
    cleanDigit(5);
    dots(1, false);
  }
  if(!hideDotsFlag){
      if (timer_dots.isReady() && !timer_dots_flag) {
          dots(1, false);
           timer_dots_flag=true;
      }else if(!timer_dots.isReady() && timer_dots_flag){
          dots(1, true);
          timer_dots_flag=false;
      }
  }
}
