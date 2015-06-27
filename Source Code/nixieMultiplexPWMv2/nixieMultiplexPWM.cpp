/*
Compile instructions:
[e.g.]
g++ -o binaryFile sourceFile1.cpp [sourceFile2.cpp sourceFileNNN.cpp] -lwiringPi -lpthread -lm
[actual]
g++ -o nixieMultiplexPWM nixieMultiplexPWM.cpp nixieTube.cpp nixieDriver.cpp -lwiringPi -lpthread -lm
[optimised]
g++ -o nixieMultiplexPWM nixieMultiplexPWM.cpp nixieTube.cpp nixieDriver.cpp -lwiringPi -lpthread -lm -O3
Needed arguments:
-lwiriingPi: Link 'Wiring for Pi' library
-lpthread: Link 'POSIX Threads' for timing functions (e.g. delay(), delayMicroseconds())
-lm: Link 'Mathematics' library (e.g. double pow(double x, double y))
Optional:
-O3: Turns on ALL compiler optimizations
*/
#include <math.h>
#include <stdio.h>
#include <wiringPi.h>
#include "nixieTube.h"
int main(void)
{
	wiringPiSetup(); // MUST be at the top of main()!!!
	NixieTube*nixieTube=new NixieTube();
	nixieTube->setAnodePin(0);
	nixieTube->addBCDPin(10);
	nixieTube->addBCDPin(11);
	nixieTube->addBCDPin(12);
	nixieTube->addBCDPin(13);
	nixieTube->setPWMHz(300);
	nixieTube->setBrightness(25);
	int incVal=0, incHold=1000;
	unsigned int incTO=millis();
	while(1){
		if(incTO<millis()){
			incTO=millis()+incHold;
			nixieTube->setDigit(incVal++);
		}
		nixieTube->runPWM();
	}
	delete nixieTube;
	return 0;
}
