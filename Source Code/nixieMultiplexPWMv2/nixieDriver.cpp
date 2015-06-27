#include <math.h> // pow()
#include <stdlib.h> // realloc(), free()
#include <wiringPi.h>
#include "nixieDriver.h"
NixieDriver::NixieDriver(){
	isOn=false;
	brightness=0; // 0.0=0%, 1.0=100%
	muxHz=0; // PWM frequency
	bcdPins=0; // BCD pin assignment
	bcdPinCount=0;
	bcdPinStates=0;
	nixies=0;
	nixieCount=0;
}
NixieDriver::~NixieDriver(){
	if(nixies){
		for(int i=0; i<nixieCount; i++){
			delete nixies[i]; // Remove objects from the heap,
		}
		free(nixies); // Then remove pointer array from the heap.
	}
	nixieCount=0;
	if(bcdPins){
		free(bcdPins); // Remove pointer array from the heap.
	}
	bcdPinCount=0;
}

int NixieDriver::addBCDPin(int pin){
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
int NixieDriver::getBCDPin(int index){
	int tmp=-1;
	if(0<=index && index<bcdPinCount){
		tmp=bcdPins[index];
	}
	return tmp; // -1 == Error getting pin.
}
int NixieDriver::getBCDPinCount(){
	return bcdPinCount;
}

int NixieDriver::addNixieTube(NixieTube*nixie){
	NixieTube**tmp=0;
	if(nixie){
		nixieCount++;
		tmp = (NixieTube**) realloc(nixies,nixieCount*sizeof(NixieTube*));
		if(tmp){ // Success (re)allocating memory.
			nixies=tmp;
			nixies[nixieCount-1]=nixie;
			// Set global parameters
			nixie->setPWMHz(muxHz*nixieCount);
		}else{ // Failure (re)allocating memory.
			nixieCount--;
		}
	}
	return tmp?nixieCount:0;
}
NixieTube*NixieDriver::getNixieTube(int index){
	NixieTube*nixie=0;
	if(nixies && nixies[index]) nixie=nixies[index];
	return nixie;
}

void NixieDriver::setDigit(int index, int value){
	if(nixies && nixies[index]) nixies[index]->setDigit(value);
}
void NixieDriver::setBCDPinStates(int index, bool*states){
	if(nixies && nixies[index]) nixies[index]->setBCDPinStates(states);
}
bool NixieDriver::getBCDPinState(int nixieIndex, int pinIndex){
	bool state=false;
	if(nixies && nixies[nixieIndex]) state=nixies[nixieIndex]->getBCDPinState(pinIndex);
	return state;
}

void NixieDriver::setMuxHz(int hz){
	if(hz>0){
		muxHz=hz;
		muxMS=(float)1000/(float)muxHz; // Max (1000/nixieCount)Hz
		nixieMS=muxMS/nixieCount;
		mux_nixieMS=muxMS-nixieMS;
		if(nixies){
			// Nixie on-time is a fraction of all nixies in the multiplex
			int nixieHz=muxHz*nixieCount;
			for(int i=0; i<nixieCount; i++){
				if(nixies[i]) nixies[i]->setPWMHz(nixieHz);
			}
		}
	}
}
int NixieDriver::getMuxHz(){
	return muxHz;
}

void NixieDriver::setBrightness(int index, int value){
	if(nixies && nixies[index]) nixies[index]->setBrightness(value);
}
void NixieDriver::setBrightness(int index, float value){
	if(nixies && nixies[index]) nixies[index]->setBrightness(value);
}
float NixieDriver::getBrightness(int index){
	float brightness=-1;
	if(nixies && nixies[index]) brightness=nixies[index]->getBrightness();
	return brightness;
}

void NixieDriver::setOn(int index, bool on){
	if(nixies && nixies[index]){
		nixies[index]->setOn(on);
		//digitalWrite(nixies[index]->getAnodePin(),on);
		/*Handling of Nixie GPIO 'on' state done in Multiplexing (NixieDriver::run()?)
		*/
	}
}
bool NixieDriver::getOn(int index){
	bool on=false;
	if(nixies && nixies[index]) on=nixies[index]->getOn();
	return on;
}

void NixieDriver::run(){
	for(int curr=0; curr<nixieCount; curr++){
		int next=(curr+1)%nixieCount;
		if(nixies[curr]->getOn()){ // Turn off
			if(nixies[curr]->getOnTO()<millis()){
				// Set next nixie's On period
				nixies[next]->setOnTO(nixies[curr]->getOffTO()+nixieMS);
				// Update current nixie's Off period
				nixies[curr]->setOffTO(millis()+nixies[curr]->getOffHold()+mux_nixieMS);
				nixies[curr]->setOn(false);
			}
		}else{ // Turn on
			if(nixies[curr]->getOffTO()<millis()){ // Wait for Off Timeout
				nixies[curr]->setOnTO(millis()+nixies[curr]->getOnHold());
				nixies[curr]->setOn(true);
			}
		}
	}
}
