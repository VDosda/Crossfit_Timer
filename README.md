# CrossFit Timer overview
<img src="Images/V2/Crossfit_timer_V2.gif"/>

## About this project

Hi !<br/> 
This project was to build a Crossfit Timer from scratch.<br/> 
I had the idea during the lockdown (COVID-19) when I was doing sport at home. A Crossfit Timer is better than a phone or a computer and a real one is pretty much expensive.<br/> 
Modes I did : count up, count down, interval, the possibility to set rounds and a basic clock mode.


## Table of contents

-   [Making](https://github.com/VDosda/Crossfit_Timer#making)
-   [Electronic](https://github.com/VDosda/Crossfit_Timer#electronic)
-   [Code](https://github.com/VDosda/Crossfit_Timer#code)
-   [Documentation](https://github.com/VDosda/Crossfit_Timer#documentation)

## Making

### Softwares :
- Fusion 360
- Cura

### 3D Printer :
- [Alfawise u20](https://fr.gearbest.com/3d-printers-3d-printer-kits/pp_1841229.html?vip=80634280&gclid=CjwKCAiAi_D_BRApEiwASslbJ_neQlq8nd4rkhrzwSbTG8Y6fiAYh2CJZcF7eu1_qxLPm5mZDWg6XRoCLWgQAvD_BwE)
- [Black AmazonBasics PLA Filament,1.75mm](https://www.amazon.fr/AmazonBasics-PLA-Printer-Filament-1-75mm/dp/B07FW5BZKC)

### Other :
- [Black Car Window Foils Tint](https://www.aliexpress.com/item/4000479123439.html?spm=a2g0s.9042311.0.0.27426c37jiLaNj)
- [Translucent paper](https://www.google.com/search?q=translucent+paper&tbm=isch&sxsrf=ALeKk03G3xllpZQCuisPQwcTahbYbgK15Q:1610404205705&source=lnms&sa=X&ved=0ahUKEwigvoCV95TuAhXATxUIHVI7BMoQ_AUIqggoAQ&biw=1434&bih=804)
- [M3 Inserts Brass](https://www.aliexpress.com/item/32842866575.html?spm=a2g0s.9042311.0.0.27424c4d6tyr0m)
- [M3 Screw](https://www.aliexpress.com/item/10000181324125.html?spm=a2g0s.9042311.0.0.27424c4d6tyr0m)
- [Brass Standoff Spacer PCB](https://www.aliexpress.com/item/32852244734.html?spm=a2g0s.9042311.0.0.27424c4d6tyr0m)


## Electronic

### Softwares :
- EasyEDA
- Fritzing

### PCB:
- [JLCPCB](https://jlcpcb.com/?gclid=CjwKCAiAi_D_BRApEiwASslbJ3z_7JnghO51ybtPCfLTmgLIY_EPknub1aJ6jTnIsRFJ7isGv1mDUhoCed0QAvD_BwE)
<img src="Images/Electronic/EasyEDA/Arduino_Buttons.png" width="300"/>
<img src="Images/Electronic/EasyEDA/Led_Digit_Panel.png" width="300"/>

### Components
- Arduino nano
- Real Time Clock (RTC)
- Tactile Push Button
- Resistors
- Capacitors
- Leds SK6812
- Buzzer
- Rechargeable Battery Lipo cells
- Lipo Charger Module
- 5V DC-DC Step-Up Power Module
- Slide switch

<img src="Images/Electronic/Fritzing/Crossfit_Timer_fritzing.jpg" width="700"/>


## Code

### Softwares :
- Arduino IDE

### Architecture 
I splited the code in four parts/files

| Main code            | Display                   |Set-up                 | Timer                           | 
|----------------------|---------------------------|-----------------------|---------------------------------|
|Button functions      | Leds matrix               | Set-up mode functions | Count up/down/interval functions
|Setup function        | Digit and Alphabet matrix |                       | 10 secondes countdown function
|Main loop             | Timer display functions   |                       | Buzzer gestion
|                      |                           |                       | Clock mode


[Code and more details]()



## Documentation
There is five buttons ( beginning from the right )

| Button |ROLE                |Timer is set                       |Set-up mode    | Timer is running | Timer is pause  | Clock mode       |
|--------|--------------------|-----------------------------------|---------------|------------------|-----------------|------------------|
|First   | START / PAUSE / OK | Start                             | Ok            | Pause            | Start           | Nothing
|Second  | SETUP TIME         | Setup mode                        | Change digits | Nothing          | Nothing         | Setup mode
|Third   | UP                 | Up count mode                     | Count up      | Nothing          | Nothing         | Nothing
|Fourth  | DOWN               | Down count mode                   | Count Down    | Nothing          | Nothing         | Nothing
|Fifth   | RESET / CLOCK MODE | Nothing or after 3 sec clock mode | Nothing       | Reset            | Reset           | After 3 sec timer mode


