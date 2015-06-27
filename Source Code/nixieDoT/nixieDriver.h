#ifndef NixieDriver_h
#define NixieDriver_h
#include <thread> // Driver thread support
#include "nixieTube.h"
class NixieDriver{
	int*bcdPins; // BCD pin assignment
	int bcdPinCount;
	NixieTube**tubes;
	int tubeCount;
	int muxUS; // Milliseconds per driver
	bool isOn;
	void (*onCallback)(NixieDriver*driverCaller, NixieTube*tubeCaller); // Callback function pointer for setOn(bool on)
	void (*offCallback)(NixieDriver*driverCaller, NixieTube*tubeCaller); // Callback function pointer for setOn(bool on)
	std::thread*threadRef; // Driver thread reference
public:
	NixieDriver();
	~NixieDriver();
	int addBCDPin(int pin);
	int getBCDPin(int index);
	int getBCDPinCount();
	int addTube(NixieTube*tube);
	NixieTube*getTube(int index);
	int getTubeCount();
	void setMuxUS(int us);
	int getMuxUS();
	void setOnCallback(void callback(NixieDriver*driverCaller, NixieTube*tubeCaller));
	void setOffCallback(void callback(NixieDriver*driverCaller, NixieTube*tubeCaller));
	void setOn(NixieTube*tube, bool on);
	void setOn(NixieTube*tube);
	void setOff(NixieTube*tube);
	std::thread*getThreadRef();
	void setThreadRef(std::thread*ref);
	void runBlock();
};
#endif