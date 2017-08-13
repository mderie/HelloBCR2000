
#ifndef BCR2000_PRIMITIVES
#define BCR2000_PRIMITIVES

#include "../JuceLibraryCode/JuceHeader.h" // Must be in first position

class BCR2000
{
private:
	mutable juce::MidiOutput* m_out; // Actually we just need a three bytes sender...

public:
	BCR2000(juce::MidiOutput* out) : m_out(out) { init(); }

	/*
	static const int CL_NONE = 0x00; //TODO: Rename all color code with CL prefix !

	static const int RED_LOW = 0x01;
	static const int RED_MED = 0x02;
	static const int RED_HIGH = 0x03;

	static const int GREEN_LOW = (0x10 * 0x01);
	static const int GREEN_MED = (0x10 * 0x02);
	static const int GREEN_HIGH = (0x10 * 0x03);

	// There are two leds behind each pad !
	static const int AMBER_LOW = GREEN_LOW + RED_LOW;
	static const int AMBER_HIGH = GREEN_HIGH + RED_HIGH;
	static const int YELLOW = GREEN_HIGH + RED_MED; // Strange...
	static const int MIXED = GREEN_MED + RED_MED;

	// All modes are mutually exclusive... Write one SetMode() method or three separate ones ?
	static const int NORMAL_MODE = 0x0C;
	static const int FLASH_MODE = 0x08;
	static const int DBUFFER_MODE = 0x00;

	static const int BTN_1 = 0x68;
	static const int BTN_2 = 0x69;
	static const int BTN_3 = 0x6A;
	static const int BTN_4 = 0x6B;
	static const int BTN_5 = 0x6C;
	static const int BTN_6 = 0x6D;
	static const int BTN_7 = 0x6E;
	static const int BTN_8 = 0x6F;

	static const int BTN_A = 0x08;
	static const int BTN_B = 0x18;
	static const int BTN_C = 0x28;
	static const int BTN_D = 0x38;
	static const int BTN_E = 0x48;
	static const int BTN_F = 0x58;
	static const int BTN_G = 0x68;
	static const int BTN_H = 0x78;
	*/

	void init();
};

#endif // BCR2000_PRIMITIVES
