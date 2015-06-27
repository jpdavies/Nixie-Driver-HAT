#ifndef NixieTube_h
#define NixieTube_h
class NixieTube{
	int anodePin; // Anode pin assignment
	int spanUS; // On...Off duration
	int onHoldUS, offHoldUS; // On...Off hold times
	float lumin; // 'Luminosity'/Brightness: 0==0%, 1==100%
	int digit; // Current digit to display
	bool isOn;
	void (*onCallback)(NixieTube*caller); // Callback function pointer for setOn(bool on)
	void (*offCallback)(NixieTube*caller); // Callback function pointer for setOn(bool on)
	void calcPWM();
public:
	NixieTube();
	~NixieTube();
	void setAnodePin(int value);
	int getAnodePin();
	void setSpanUS(int value);
	int getSpanUS();
	void setOnHoldUS(int value);
	int getOnHoldUS();
	void setOffHoldUS(int value);
	int getOffHoldUS();
	void setLumin(float value);
	float getLumin();
	void setDigit(int value);
	int getDigit();
	void getBCD(bool*states, int size);
	void setOnCallback(void callback(NixieTube*caller));
	void setOffCallback(void callback(NixieTube*caller));
	void setOn(bool on);
	void setOn();
	void setOff();
	bool getOn();
	void runBlock();
};
#endif