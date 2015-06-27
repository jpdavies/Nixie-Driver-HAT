//#include <stdio.h> // Temp for 'printf()'
#include <stdlib.h> // realloc(), free()
#include <wiringPi.h>
#include "nixieTube.h"
NixieTube::NixieTube(){
	// Init vars
	anodePin=0;
	spanUS=0;
	onHoldUS=offHoldUS=0;
	lumin=0;
	digit=0;
	isOn=false;
	onCallback=0;
	offCallback=0;
}
NixieTube::~NixieTube(){
}

void NixieTube::setAnodePin(int value){
	if(value>=0){
		anodePin=value;
	}
}
int NixieTube::getAnodePin(){
	return anodePin;
}

void NixieTube::setSpanUS(int value){
	if(value>0){
		spanUS=value;
		calcPWM();
	}
}
int NixieTube::getSpanUS(){
	return spanUS;
}

void NixieTube::setOnHoldUS(int value){
	if(value>0){
		onHoldUS=value;
	}
}
int NixieTube::getOnHoldUS(){
	return onHoldUS;
}
void NixieTube::setOffHoldUS(int value){
	if(value>0){
		offHoldUS=value;
	}
}
int NixieTube::getOffHoldUS(){
	return offHoldUS;
}

void NixieTube::setLumin(float value){
	if(value>0 && value<=1){
		lumin=value;
		calcPWM();
	}
}
float NixieTube::getLumin(){
	return lumin;
}

void NixieTube::setDigit(int value){
	if(value>=0){
		digit=value%10; // Make it BCD-compliant
	}
}
int NixieTube::getDigit(){
	return digit;
}
void NixieTube::getBCD(bool*states, int size){
	if(states && size==4){
		int mask=8;
		for(int i=0; i<size; i++){
			states[i]=(digit&mask)?true:false;
			mask=mask>>1;
		}
	}
}

void NixieTube::setOnCallback(void callback(NixieTube*caller)){
	onCallback=callback;
}
void NixieTube::setOffCallback(void callback(NixieTube*caller)){
	offCallback=callback;
}

void NixieTube::setOn(bool on){
	isOn=on;
	if(on){
		if(onCallback) onCallback(this);
	}else{
		if(offCallback) offCallback(this);
	}
}
void NixieTube::setOn(){
	setOn(true);
}
void NixieTube::setOff(){
	setOn(false);
}
bool NixieTube::getOn(){
	return isOn;
}

void NixieTube::calcPWM(){
	onHoldUS=spanUS*lumin;
	offHoldUS=spanUS-onHoldUS;
}

void NixieTube::runBlock(){
	setOn();
	delayMicroseconds(onHoldUS);
	setOff();
	delayMicroseconds(offHoldUS);
}