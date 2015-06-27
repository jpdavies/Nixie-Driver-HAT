#ifndef NixieDisplay_h
#define NixieDisplay_h
#include "nixieDriver.h"
class NixieDisplay{
	void driverThread(NixieDriver*driver);
	NixieDriver**drivers;
	int driverCount;
public:
	NixieDisplay();
	~NixieDisplay();
	int addDriver(NixieDriver*driver);
	NixieDriver*getDriver(int index);
	void startThread(NixieDriver*driver);
};
#endif