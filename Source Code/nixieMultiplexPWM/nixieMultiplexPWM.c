/*
Compile instructions:
gcc -o binaryFile sourceFile.c -lwiringPi -lpthread -std=99 -lm
Needed arguments:
-lwiriingPi: Link 'Wiring for Pi' library
-lpthread: Link 'POSIX Threads' for timing functions (e.g. delay(), delayMicroseconds())
-std=99: Permits (amongst other features) inline variable declaration in for-loops (e.g. for(int i=0; i<loops; i++))
-lm: Link 'Mathematics' library (e.g. double pow(double x, double y))
*/
#include <math.h>
#include <stdio.h>
#include <wiringPi.h>
int main(void)
{
	wiringPiSetup(); // MUST be at the top of main()!!!
	//Nixie Anode pins
	int nixieAnodePins[]={0,1,2,3,4,5};
	int nixieAnodeCount=sizeof(nixieAnodePins)/sizeof(nixieAnodePins[0]);
	for(int i=0; i<nixieAnodeCount; i++){ // Set Nixie select pins
		pinMode(nixieAnodePins[i], OUTPUT);
	}
	//BCD pins
	int nixieBCDPins[]={10,11,12,13};
	int nixieBCDCount=sizeof(nixieBCDPins)/sizeof(nixieBCDPins[0]);
	for(int i=0; i<nixieBCDCount; i++){
		pinMode(nixieBCDPins[i], OUTPUT);
	}
	int curNumber=0;
	while(1){ // Loop forever
		for(int i=0; i<nixieAnodeCount; i++){ // Loop through Nixies
			// Set Number
			int curBit=0; // Current bit position
			for(int j=0; j<nixieBCDCount; j++){
				curBit=(int)pow((double)2,(double)j);
				printf("curNumber=%d,curBit=%d,curNumber&curBit=%d,nixieBCDPins[%d]=%d\n",
					curNumber,curBit,curNumber&curBit,j,nixieBCDPins[j]);
			}
			// Turn Nixie ON
			// Turn Nixie OFF
		}
		// Increment current number
		curNumber++;
		curNumber%=10;
		delay(1000);
	}
	return 0;
}
