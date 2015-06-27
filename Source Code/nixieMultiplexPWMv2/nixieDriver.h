#ifndef NixieDriver_h
#define NixieDriver_h
#include "nixieTube.h"
class NixieDriver{
	bool isOn;
	float brightness; // 0.0=0%, 1.0=100%
	int muxHz; // Multiplex frequency
	int muxMS; // Multiplex milliseconds
	int nixieMS; // Milliseconds per Nixie
	int mux_nixieMS; // muxMS-nixieMS
	int*bcdPins; // BCD pin assignment
	int bcdPinCount;
	bool*bcdPinStates;
	NixieTube**nixies;
	int nixieCount;
	void calcPWM();
public:
	NixieDriver();
	~NixieDriver();
	int addBCDPin(int pin);
	int getBCDPin(int index);
	int getBCDPinCount();
	int addNixieTube(NixieTube*nixie);
	NixieTube*getNixieTube(int index);
	void setDigit(int index, int digit);
	void setBCDPinStates(int index,bool*states);
	bool getBCDPinState(int nixieIndex, int pinIndex);
	void setMuxHz(int hz);
	int getMuxHz();
	void setBrightness(int index, int value);
	void setBrightness(int index, float value);
	float getBrightness(int index);
	void setOn(int index, bool on);
	bool getOn(int index);
	void run();
};
#endif