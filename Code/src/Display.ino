int ledMatrix7seg[6][7] = {
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

#define C   10
#define D   11
#define H   12
#define L   13
#define N   14
#define P   15
#define R   16
#define T   17
#define U   18
#define MINUS   19
#define dataLeds7seg_TAB_SIZE    20
char dataLeds7seg[dataLeds7seg_TAB_SIZE] = {
  0x77, //0    1110111
  0x60, //1    1100000
  0x5D, //2    1011101
  0x7C, //3    1111100
  0x6A, //4    1101010
  0x3E, //5    0111110
  0x3F, //6    0111111
  0x64, //7    1100100
  0x7F, //8    1111111
  0x7E, //9    1111110
  0x17, //C    0010111
  0x79, //D    1111001
  0x6B, //H    1101011
  0x13, //L    0010011
  0x29, //N    0101001
  0x4F, //P    1001111
  0x09, //R    0001001
  0x1B, //T    0011011
  0x73, //U    1110011
  0x08, //-    0001000
};

charInt2 modeMap[6] = {
  {"H1", {H,1}},
  {"H2", {H,2}},
  {"RD", {R,D}},
  {"CL", {C,L}},
  {"UP", {U,P}},
  {"DN", {D,N}},
};

//////////////////////////////////////////////////////  FUNCTIONS ////////////////////////////////////////////////////////

void fastLedUtil(int led, int r, int g, int b){
  leds_7_seg[led] = CRGB(r, g, b);
}

void cleanDisplay(){
    for(int i=0; i<6; i++){
      for(int j=0; j<7; j++){
         fastLedUtil(ledMatrix7seg[i][j], 0, 0, 0);
      }
  }
  dots(0, false);
  dots(1, false);
}

void cleanDisplay(int num_display){
  for(int i=0; i<7; i++){
     fastLedUtil(ledMatrix7seg[num_display][i], 0, 0, 0);
  }
}

void resetTimerTamps(){
     for(int i=0; i<6; i++){ //reset all displays to display time
        unit_of_time_display_tamp[i]=-1;
      }
      mode_display_tamp="  ";
    }

void dots(int positionDots, bool state){
  if(state){
    fastLedUtil(ledMatrixDots[positionDots][0], BRIGHTNESS, 0, 0);
    fastLedUtil(ledMatrixDots[positionDots][1], BRIGHTNESS, 0, 0);
  }else{
    fastLedUtil(ledMatrixDots[positionDots][0], 0, 0, 0);
    fastLedUtil(ledMatrixDots[positionDots][1], 0, 0, 0);
  }
  FastLED.show();
}


void displayLeds(int value, int px, int r, int g, int b){
  for(int i=0; i<7; i++){
      if((dataLeds7seg[value] & (1 << i)) >> i){
        fastLedUtil(ledMatrix7seg[px][i], r, g, b);
      }
      else{
        fastLedUtil(ledMatrix7seg[px][i], 0, 0, 0);
      }
   }
  FastLED.show();
}

void timerMode(int display_1, int display_2){
     displayLeds(display_1, 0, 0, BRIGHTNESS, 0);
     displayLeds(display_2, 1, 0, BRIGHTNESS, 0);
  FastLED.show();
}


void displayGestion(int position7seg, int r, int g, int b){
     displayLeds(unit_of_time_display[position7seg], position7seg, r, g, b);
     unit_of_time_display_tamp[position7seg]= unit_of_time_display[position7seg];
}

int entityRHMS[4];
bool hideEntityMRHMSD[6];
void TimerDisplay(char * mode, int rounds, int minutes, int seconds, bool hideModeFlag, bool hideRoundsFlag, bool hideMinutesFlag, bool hideSecondsFlag, bool hideDotsFlag){

//---------------------------------------------------------DISPLAY 0 1
  if(!hideModeFlag && hideRoundsFlag){
      if (mode_display_tamp != mode){
          dots(0, false);
          for(int i=0; i<6; i++){
            if (modeMap[i].mode == mode){
              displayLeds(modeMap[i].tab[0], 0, 0, BRIGHTNESS, 0);
              displayLeds(modeMap[i].tab[1], 1, 0, BRIGHTNESS, 0);
            }
          }
          mode_display_tamp = mode;
      }
  }
  
  if(!hideRoundsFlag && hideModeFlag){
      unit_of_time_display[0]=abs(rounds/10);
      unit_of_time_display[1]=rounds-abs(rounds/10)*10;
      
      dots(0, false);
      
      if ( unit_of_time_display_tamp[0]!= unit_of_time_display[0]){
          displayGestion(0, 0, BRIGHTNESS, 0);
      }
      if ( unit_of_time_display_tamp[1]!= unit_of_time_display[1]){   
          displayGestion(1, 0, BRIGHTNESS, 0);
      }
  }

  if(hideModeFlag && hideRoundsFlag){
    cleanDisplay(0);
    cleanDisplay(1);
    dots(0, false);
  }
//---------------------------------------------------------DISPLAY 2 3
  if(!hideMinutesFlag){
     unit_of_time_display[2]=abs(minutes/10);
     unit_of_time_display[3]=minutes-abs(minutes/10)*10;
    
    if ( unit_of_time_display_tamp[2]!= unit_of_time_display[2]){
        displayGestion(2, BRIGHTNESS, 0, 0);
    }
    if ( unit_of_time_display_tamp[3]!= unit_of_time_display[3]){
        displayGestion(3, BRIGHTNESS, 0, 0);
    }
  }else {
    cleanDisplay(2);
    cleanDisplay(3);
    dots(1, false);
  }
//---------------------------------------------------------DISPLAY 4 5  
   unit_of_time_display[4]=abs(seconds/10);
   unit_of_time_display[5]=seconds-abs(seconds/10)*10;
  if(!hideSecondsFlag){
    if ( unit_of_time_display_tamp[4]!= unit_of_time_display[4]){
        displayGestion(4, BRIGHTNESS, 0, 0);
    }
    if ( unit_of_time_display_tamp[5]!= unit_of_time_display[5]){
        displayGestion(5, BRIGHTNESS, 0, 0);
        if(!hideDotsFlag){
          timer_dots.reset();
        } 
    }
  }else {
    cleanDisplay(4);
    cleanDisplay(5);
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
