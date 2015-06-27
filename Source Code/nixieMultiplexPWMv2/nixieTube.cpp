#include <math.h> // pow()
//#include <stdio.h> // Temp for 'printf()'
#include <stdlib.h> // realloc(), free()
#include <wiringPi.h>
#include "nixieTube.h"
NixieTube::NixieTube(){
	// Init vars
	isOn=false;
	brightness=0;
	digit=0;
	anodePin=0;
	pwmHz=0;
	onHold=offHold=0;
	onTO=offTO=0;
	bcdPins=0;
	bcdPinCount=0;
	bcdPinStates=0;
	// Init default settings
	setPWMHz(50);
	setBrightness(100);
	setDigit(0);
	setOn(true);
}
NixieTube::~NixieTube(){
	if(bcdPins){
		free(bcdPins); // Remove pointer array from the heap.
	}
	bcdPinCount=0;
}

void NixieTube::setDigit(int value){
	/*
	Sets 'value' to be BCD compliant,
	Sets 'digit' to 'value',
	Sets 'bcdPinStates' array,
	digitalWrites to bcdPins.
	*/
	if(value>=0){
		digit=value%10; // Make it BCD-compliant
		int bit=0;
		for(int i=0; i<bcdPinCount; i++){
			bit=(int)pow((double)2,(double)i);
			bcdPinStates[i]=digit&bit;
			digitalWrite(bcdPins[i],bcdPinStates[i]);
		}
	}
}
int NixieTube::getDigit(){
	return digit;
}
void NixieTube::setBCDPinStates(bool*states){
	/*
	Calculates value and passes to setDigit()
	*/
	int statesCount=sizeof(states)/sizeof(states[0]);
	if(statesCount==bcdPinCount){
		int bit=0, value=0;
		for(int i=0; i<statesCount; i++){
			bit=(int)pow((double)2,(double)i);
			value+=states[i]*bit;
		}
		setDigit(value);
	}
}
bool NixieTube::getBCDPinState(int index){
	return bcdPinStates[index];
}

void NixieTube::setBrightness(int value){
	if(value>0 && value<=100){
		setBrightness((float)value/(float)100);
	}
}
void NixieTube::setBrightness(float value){
	if(value>0 && value<=1){
		brightness=value;
		calcPWM();
	}
}
float NixieTube::getBrightness(){
	return brightness;
}

void NixieTube::setAnodePin(int pin){
	if(pin>=0){
		anodePin=pin;
		pinMode(pin,OUTPUT);
		digitalWrite(pin,LOW);
	}
}
int NixieTube::getAnodePin(){
	return anodePin;
}

int NixieTube::addBCDPin(int pin){
	int*tmpPins=0;
	bool*tmpStates=0;
	if(pin>=0){
		bcdPinCount++;
		tmpPins = (int*) realloc(bcdPins,bcdPinCount*sizeof(int));
		tmpStates = (bool*) realloc(bcdPinStates,bcdPinCount*sizeof(bool));
		if(tmpPins && tmpStates){ // Success (re)allocating memory.
			bcdPins=tmpPins;
			bcdPins[bcdPinCount-1]=pin;
			bcdPinStates=tmpStates;
			bcdPinStates[bcdPinCount-1]=false;
			pinMode(pin,OUTPUT);
			digitalWrite(pin,LOW);
		}else{ // Failure (re)allocating memory.
			bcdPinCount--;
		}
	}
	return (tmpPins && tmpStates)?bcdPinCount:0; // 0 == Error (re)allocating memory.
}
int NixieTube::getBCDPin(int index){
	int tmp=-1;
	if(0<=index && index<bcdPinCount){
		tmp=bcdPins[index];
	}
	return tmp; // -1 == Error getting pin.
}
int NixieTube::getBCDPinCount(){
	return bcdPinCount;
}

void NixieTube::setOn(bool on){
	isOn=on;
}
bool NixieTube::getOn(){
	return isOn;
}

void NixieTube::setPWMHz(int hz){
	if(hz>0){
		pwmHz=hz;
		calcPWM();
	}
}
int NixieTube::getPWMHz(){
	return pwmHz;
}

void NixieTube::calcPWM(){
	int pwmMS=(float)1000/(float)pwmHz; // Max 1000Hz
	onHold=pwmMS*brightness;
	offHold=pwmMS-onHold;
}

void NixieTube::setOnHold(int value){
	onHold=value;
}
int NixieTube::getOnHold(){
	return onHold;
}
void NixieTube::setOffHold(int value){
	offHold=value;
}
int NixieTube::getOffHold(){
	return offHold;
}

void NixieTube::setOnTO(unsigned int value){
	onTO=value;
}
unsigned int NixieTube::getOnTO(){
	return onTO;
}
void NixieTube::setOffTO(unsigned int value){
	offTO=value;
}
unsigned int NixieTube::getOffTO(){
	return offTO;
}

void NixieTube::runPWM(){
	/*
	Run Nixie with brightness set by software PWM
	*/
	if(isOn){ // Turn off
		if(onTO<millis()){ // Wait for On Timeout
			offTO=millis()+offHold;
			setOn(false);
		}
	}else{ // Turn on
		if(offTO<millis()){ // Wait for Off Timeout
			onTO=millis()+onHold;
			setOn(true);
		}
	}
}