//#include <stdio.h> // Temp for 'printf()'
#include <stdlib.h> // realloc(), free()
#include <wiringPi.h>
#include "nixieDisplay.h"

void NixieDisplay::driverThread(NixieDriver*driver){
	while(1){
		if(driver) driver->runBlock();
	}
}

NixieDisplay::NixieDisplay(){
	drivers=0;
	driverCount=0;
}
NixieDisplay::~NixieDisplay(){
	if(drivers){
		for(int i=0; i<driverCount; i++){
			if(drivers[i]){ // Remove objects from the heap,
				delete drivers[i];
				drivers[i]=0;
			}
		}
		free(drivers); // Then remove pointer array from the heap.
	}
	driverCount=0;
}

int NixieDisplay::addDriver(NixieDriver*driver){
	NixieDriver**tmp=0;
	if(driver){
		driverCount++;
		tmp = (NixieDriver**) realloc(drivers,driverCount*sizeof(NixieDriver*));
		if(tmp){
			drivers=tmp;
			drivers[driverCount-1]=driver;
		}else{
			driverCount--;
		}
	}
	return tmp?driverCount:0;
}
NixieDriver*NixieDisplay::getDriver(int index){
	NixieDriver*driver=0;
	if(drivers && drivers[index]) driver=drivers[index];
	return driver;
}

void NixieDisplay::startThread(NixieDriver*driver){
	std::thread*threadRef=new std::thread(&NixieDisplay::driverThread,this,driver);
	driver->setThreadRef(threadRef);
}