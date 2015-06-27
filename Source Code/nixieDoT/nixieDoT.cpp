/*
Compile instructions:
[e.g.]
g++ -o binaryFile sourceFile1.cpp [sourceFile2.cpp sourceFileNNN.cpp] -lwiringPi -lpthread -lm
[actual]
g++ -o nixieDoT nixieDoT.cpp nixieTube.cpp nixieDriver.cpp nixieDisplay.cpp -lwiringPi -lpthread -lm -std=c++0x
[optimised]
g++ -o nixieDoT nixieDoT.cpp nixieTube.cpp nixieDriver.cpp nixieDisplay.cpp -lwiringPi -lpthread -lm -std=c++0x -O3
Needed arguments:
-lwiriingPi: Link 'Wiring for Pi' library
-lpthread: Link 'POSIX Threads' for timing functions (e.g. delay(), delayMicroseconds())
-lm: Link 'Mathematics' library (e.g. double pow(double x, double y))
-std=c++0x: Link 'C++0x' thread library (e.g. onvoke member function on new thread)
Optional:
-O3: Turns on ALL compiler optimizations
*/
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <signal.h>
#include "nixieTube.h"
#include "nixieDriver.h"
#include "nixieDisplay.h"

volatile sig_atomic_t stop;
void inthand(int signum) {
    stop = 1;
}

void driverTubeCallback(NixieDriver*driver, NixieTube*tube){ // Driver-Tube 'on' callback
	bool states[4];
	tube->getBCD(states, 4);
	for(int i=0; i<driver->getBCDPinCount(); i++){
		//printf("bcd[%d]=%d,state=%d,",i,driver->getBCDPin(i),states[i]);
		digitalWrite(driver->getBCDPin(i),states[i]?HIGH:LOW);
	}
	//printf("on=%d,digit=%d,anode=%d\n",tube->getOn(),tube->getDigit(),tube->getAnodePin());
	digitalWrite(tube->getAnodePin(),tube->getOn()?HIGH:LOW);
}

int main(void)
{
	wiringPiSetup(); // MUST be at the top of main()!!!
	signal(SIGINT, inthand);
	
	NixieDisplay*display=0;
	NixieDriver*driver=0;
	NixieTube*tube=0;
	
	display=new NixieDisplay();
	
	driver=new NixieDriver();
	driver->addBCDPin(13);
	pinMode(13,OUTPUT);
	driver->addBCDPin(12);
	pinMode(12,OUTPUT);
	driver->addBCDPin(11);
	pinMode(11,OUTPUT);
	driver->addBCDPin(10);
	pinMode(10,OUTPUT);
	driver->setMuxUS(20000);
	driver->setOnCallback(driverTubeCallback);
	driver->setOffCallback(driverTubeCallback);
	display->addDriver(driver);
	
	int anodePins[]={0,1,2,3,4,5};
	float lumins[]={1,1,1,1,1,1};
	int digits[]={1,2,3,4,5,6};
	for(int i=0; i<6; i++){
		tube=new NixieTube();
		tube->setAnodePin(anodePins[i]);
		pinMode(anodePins[i],OUTPUT);
		tube->setLumin(lumins[i]);
		tube->setDigit(digits[i]);
		driver->addTube(tube);
	}
	
	display->startThread(driver);
	
	char strCmd[256];
	while(!stop){
		printf("command:");
		fgets(strCmd,256,stdin);
		if(strCmd[0]=='n'){
			if(strCmd[1]=='0') display->getDriver(0)->getTube(0)->setDigit(0);
			else if(strCmd[1]=='1') display->getDriver(0)->getTube(0)->setDigit(1);
			else if(strCmd[1]=='2') display->getDriver(0)->getTube(0)->setDigit(2);
			else if(strCmd[1]=='3') display->getDriver(0)->getTube(0)->setDigit(3);
			else if(strCmd[1]=='4') display->getDriver(0)->getTube(0)->setDigit(4);
			else if(strCmd[1]=='5') display->getDriver(0)->getTube(0)->setDigit(5);
			else if(strCmd[1]=='6') display->getDriver(0)->getTube(0)->setDigit(6);
			else if(strCmd[1]=='7') display->getDriver(0)->getTube(0)->setDigit(7);
			else if(strCmd[1]=='8') display->getDriver(0)->getTube(0)->setDigit(8);
			else if(strCmd[1]=='9') display->getDriver(0)->getTube(0)->setDigit(9);
		} else if(!strcmp(strCmd,"stop\n")){
			stop=1;
		} else if(strCmd[0]=='m'){
			char str[3];
			str[0]=strCmd[1];
			str[1]=strCmd[2];
			str[2]=strCmd[3];
			printf("hz=%d\n",atoi(str));
			driver->setMuxUS(1000000/atoi(str));
		}
	}
	
	printf("stopping...\n");
	if(display)delete(display);
	for(int i=0; i<6; i++){
		digitalWrite(anodePins[i],LOW);
	}
	return 0;
}
