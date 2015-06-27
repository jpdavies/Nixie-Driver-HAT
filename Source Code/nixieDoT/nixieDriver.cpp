#include <stdlib.h> // realloc(), free()
#include <wiringPi.h>
#include "nixieDriver.h"
NixieDriver::NixieDriver(){
	bcdPins=0; // BCD pin assignment
	bcdPinCount=0;
	tubes=0;
	tubeCount=0;
	muxUS=0; // Milliseconds per driver
	isOn=0;
	onCallback=0; // Callback function pointer for setOn(bool on)
	offCallback=0; // Callback function pointer for setOn(bool on)
	threadRef=0; // Driver thread reference
}
NixieDriver::~NixieDriver(){
	if(tubes){
		for(int i=0; i<tubeCount; i++){
			if(tubes[i]){
				delete tubes[i]; // Remove objects from the heap,
				tubes[i]=0;
			}
		}
		free(tubes); // Then remove pointer array from the heap.
	}
	tubeCount=0;
	if(bcdPins){
		free(bcdPins); // Remove pointer array from the heap.
	}
	bcdPinCount=0;
}

int NixieDriver::addBCDPin(int pin){
	int*tmpPins=0;
	if(pin>=0){
		bcdPinCount++;
		tmpPins = (int*) realloc(bcdPins,bcdPinCount*sizeof(int));
		if(tmpPins){ // Success (re)allocating memory.
			bcdPins=tmpPins;
			bcdPins[bcdPinCount-1]=pin;
		}else{ // Failure (re)allocating memory.
			bcdPinCount--;
		}
	}
	return tmpPins?bcdPinCount:0; // 0 == Error (re)allocating memory.
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

int NixieDriver::addTube(NixieTube*tube){
	NixieTube**tmp=0;
	if(tube){
		tubeCount++;
		tmp = (NixieTube**) realloc(tubes,tubeCount*sizeof(NixieTube*));
		if(tmp){ // Success (re)allocating memory.
			tubes=tmp;
			tubes[tubeCount-1]=tube;
			setMuxUS(muxUS); // Update multiplex timings
		}else{ // Failure (re)allocating memory.
			tubeCount--;
		}
	}
	return tmp?tubeCount:0;
}
NixieTube*NixieDriver::getTube(int index){
	NixieTube*tube=0;
	if(tubes && tubes[index]) tube=tubes[index];
	return tube;
}
int NixieDriver::getTubeCount(){
	return tubeCount;
}

void NixieDriver::setMuxUS(int us){
	if(us>0){
		muxUS=us;
		int spanUS=(int)((float)muxUS/(float)tubeCount);
		if(tubes){
			// Tube on-time is a fraction of all tubes in the multiplex
			for(int i=0; i<tubeCount; i++){
				if(tubes[i]) tubes[i]->setSpanUS(spanUS);
			}
		}
	}
}
int NixieDriver::getMuxUS(){
	return muxUS;
}

void NixieDriver::setOnCallback(void callback(NixieDriver*driverCaller, NixieTube*tubeCaller)){
	onCallback=callback;
}
void NixieDriver::setOffCallback(void callback(NixieDriver*driverCaller, NixieTube*tubeCaller)){
	offCallback=callback;
}

void NixieDriver::setOn(NixieTube*tube, bool on){
	if(tube) tube->setOn(on);
	if(on){
		if(onCallback) onCallback(this, tube);
	}else{
		if(offCallback) offCallback(this, tube);
	}
}
void NixieDriver::setOn(NixieTube*tube){
	setOn(tube, true);
}
void NixieDriver::setOff(NixieTube*tube){
	setOn(tube, false);
}

std::thread*NixieDriver::getThreadRef(){
	return threadRef;
}
void NixieDriver::setThreadRef(std::thread*ref){
	threadRef=ref;
}

void NixieDriver::runBlock(){
	for(int i=0; i<tubeCount; i++){
		NixieTube*tube=getTube(i);
		if(tube){
			setOn(tube);
			delayMicroseconds(tube->getOnHoldUS());
			setOff(tube);
			delayMicroseconds(tube->getOffHoldUS());
		}
	}
}
