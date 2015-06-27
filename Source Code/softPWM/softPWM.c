#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
int main(void)
{
  wiringPiSetup(); // MUST be at the top of main()!!!
  float pwmHz=50; // Pulses per second
  float pwmDuty=50; // % on time per pulse
  float pwmRange=10000/pwmHz; // 10000 == one second
  float fadeMin=0;
  float fadeMax=100;
  float fadeCurr=0; // Variable PWM duty %
  float fadeMillis=4000;
  float fadeDelay=fadeMillis/(fadeMax-fadeMin); // 1000 == one second
  float fadeTO=millis()+fadeDelay;
  pinMode(0,OUTPUT);
  softPwmCreate(0,pwmRange*(fadeCurr/100),pwmRange);
  while(1){ // Loop forever
    while(fadeCurr<fadeMax){ // Fade up to 100% over 1 second
      if(fadeTO<millis()){
        fadeTO=millis()+fadeDelay;
        fadeCurr++;
        softPwmWrite(0,pwmRange*((fadeCurr-fadeMin)/(fadeMax-fadeMin)));
      }
    }
    while(fadeCurr>fadeMin){ // Fade down to 0% over 1 second
      if(fadeTO<millis()){
        fadeTO=millis()+fadeDelay;
        fadeCurr--;
        softPwmWrite(0,pwmRange*((fadeCurr-fadeMin)/(fadeMax-fadeMin)));
      }
    }
  }
  return 0;
}
