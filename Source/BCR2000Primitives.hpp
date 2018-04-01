
#ifndef BCR2000_PRIMITIVES
#define BCR2000_PRIMITIVES

#include "../JuceLibraryCode/JuceHeader.h" // Must be in first position

class BCR2000
{
private:
  int m_messageChainCounter;
	mutable juce::MidiOutput* m_out;
	void sendMessage(const std::string &s);

public:
	BCR2000(juce::MidiOutput* out) : m_out(out) { init(); }

	void setValue(int i);
	int getValue();

	//static const int BCL_LINE_START_TAG = 0x20;

	void init();
};

#endif // BCR2000_PRIMITIVES
