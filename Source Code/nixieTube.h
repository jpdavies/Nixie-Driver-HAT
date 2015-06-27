#ifndef NixieTube_h
#define NixieTube_h
class NixieTube{
	bool isOn;
	float brightness; // 0.0=0%, 1.0=100%
	int digit; // Current digit to display
	int anodePin; // Anode pin assignment
	int pwmHz; // PWM frequency
	int onHold, offHold; // On/Off hold times
	unsigned int onTO, offTO; // On/Off time-outs
	int*bcdPins; // BCD pin assignment
	int bcdPinCount;
	bool*bcdPinStates;
	void calcPWM();
public:
	NixieTube();
	~NixieTube();
	void setDigit(int value);
	int getDigit();
	void setBCDPinStates(bool*states);
	bool getBCDPinState(int index);
	void setBrightness(int value);
	void setBrightness(float value);
	float getBrightness();
	void setAnodePin(int pin);
	int getAnodePin();
	int addBCDPin(int pin);
	int getBCDPin(int index);
	int getBCDPinCount();
	void setOn(bool on);
	bool getOn();
	void setPWMHz(int hz);
	int getPWMHz();
	void runPWM();
};
#endif