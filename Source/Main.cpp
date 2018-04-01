/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

// Don't do this...
//#include "juce_audio_basics/sources/juce_ToneGeneratorAudioSource.h"

// Do this:)
#include "../JuceLibraryCode/JuceHeader.h"

#include "BCR2000Primitives.hpp"

class TGAS : public juce::ToneGeneratorAudioSource
{
public:
  void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
};

void TGAS::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
  //std::cout << "getNextAudioBlock called" << std::endl;
  ToneGeneratorAudioSource::getNextAudioBlock(bufferToFill);
  // Extra process can take place here
}

class BCR2000Callback : public juce::MidiInputCallback
{
public:
	void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
};

// Temp :)
//int eventCountDown = 3;
bool done = false;
juce::AudioDeviceManager* adm = nullptr;
juce::AudioIODevice* aiod = nullptr;
juce::MidiOutput* midiOutput = nullptr;
juce::MidiInput* midiInput = nullptr;
juce::AudioSourcePlayer* asp = nullptr;
BCR2000Callback* cb = nullptr;
BCR2000* bcr = nullptr;
TGAS* tgas = nullptr;

void BCR2000Callback::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
	std::cout << "Incomming message :" << std::endl;
	const juce::uint8* p = message.getRawData();
	for (int i = 0; i < message.getRawDataSize(); i++)
	{
		std::cout << std::hex << static_cast<int>(*(p + i)) << std::endl;
	}

	// First two rotary controls (top left)
	if ((static_cast<int>(*(p + 1))) == 0x01)
	{
	  // Instead of 127.0 one could type 127 and then cast the final result in (loat !-)
	  std::cout << "New amplitude = " << (static_cast<int>(*(p + 2))) / 127.0 << std::endl;
		tgas->setAmplitude((static_cast<int>(*(p + 2))) / 127.0); // No idea... So between 0 & 1
	}

  if ((static_cast<int>(*(p + 1))) == 0x02)
	{
	  std::cout << "New frequency = " << ((static_cast<int>(*(p + 2)) * 4)) + 50 << std::endl;
		tgas->setFrequency(((static_cast<int>(*(p + 2)) * 4)) + 50); // So between 50 & 562
	}

	//if (eventCountDown-- == 0) // Temp as well...
	if ((static_cast<int>(*(p + 1))) == 0x41) // Top left button
	{
	  done = true;
	}

	if ((static_cast<int>(*(p + 1))) == 0x42) // Top left button
	{
	  //tgas->setFrequency(880);
	  tgas->setAmplitude(0.4);
	}
}

//==============================================================================
int main (int argc, char* argv[])
{
	std::cout << "Welcome to hello BCR2000" << std::endl;

	// Output

	std::cout << "Names of all the midi output device (bcr2000)" << std::endl;
	juce::StringArray outputDevices = juce::MidiOutput::getDevices(); // Crash if no midi device at all ?
	std::cout << "Boum ?" << std::endl; // To fix this just add #define JUCE_ALSA_MIDI_NAME "USYNTH" in the user code section of the AppConfig.h !
	int i = 0;
	int lpOutIndex = -1;
	int lpInIndex = -1;
	std::string dummy;

	for (auto s : outputDevices)
	{
			if (s.contains("2000"))
			{
					lpOutIndex = i;
			}

			std::cout << "index " << i++ << " : " << s << std::endl;
	}

	if (lpOutIndex == -1)
	{
			lpOutIndex = 1; // Fine tune this default value !
	}

	std::cout << "About to use " << lpOutIndex << " as index... Press ENTER or CTRL-C to stop !" << std::endl;
	std::getline(std::cin, dummy);
	midiOutput = juce::MidiOutput::openDevice(lpOutIndex);
	bcr = new BCR2000(midiOutput);

	// Input

	std::cout << "Names of all the midi input device (bcr2000)" << std::endl;
	juce::StringArray inputDevices = juce::MidiInput::getDevices();
	i = 0;
	for (auto s : inputDevices)
	{
			if (s.contains("2000"))
			{
					lpInIndex = i;
			}

			std::cout << "index " << i++ << " : " << s << std::endl;
	}

	if (lpInIndex == -1)
	{
			lpInIndex = 1; // Fine tune this default value !
	}

	std::cout << "About to use " << lpInIndex << " as index... Press ENTER or CTRL-C to stop !" << std::endl;
	std::getline(std::cin, dummy);
	cb = new BCR2000Callback();
	if (!cb)
	{
		std::cout << "Oups : cb is null" << std::endl;
		return 0;
	}

	midiInput = juce::MidiInput::openDevice(lpInIndex, cb);
	midiInput->start();

	// Init the minimal audio stack
	adm = new AudioDeviceManager();
	if (!adm)
	{
		std::cout << "Oups : adm is null" << std::endl;
		return 0;
	}
	adm->initialiseWithDefaultDevices(0, 2);
	aiod = adm->getCurrentAudioDevice();
	if (!aiod)
	{
		std::cout << "Oups : aiod is null" << std::endl;
		return 0;
	}
	aiod->open(0, 2, aiod->getCurrentSampleRate(), aiod->getDefaultBufferSize());

	// Get 2 inputs from bcr2000 and push them to setFrequency() and setAmplitude() of the TGAS
	tgas = new TGAS();
	if (!tgas)
	{
		std::cout << "Oups : tgas is null" << std::endl;
		return 0;
	}
	tgas->setFrequency(440.0); // A4
	tgas->setAmplitude(0.8); // Verify this ! [0,1] ?
	tgas->prepareToPlay(aiod->getDefaultBufferSize(), aiod->getCurrentSampleRate());

	asp = new AudioSourcePlayer();
	if (!asp)
	{
		std::cout << "Oups : asp is null" << std::endl;
		return 0;
	}
	asp->setSource(tgas);

	aiod->start(asp);
	while (not done)
	{
	  //TODO ?
	}

	std::cout << "That 's all folks... Press ENTER" << std::endl;
	std::getline(std::cin, dummy); // Or cin >> dummy;

	// Stop all
	aiod->stop();
	std::cout << "Dead stop 0" << std::endl;
	tgas->releaseResources();
	std::cout << "Dead stop 1" << std::endl;
	adm->closeAudioDevice();
	std::cout << "Dead stop 2" << std::endl;
	midiInput->stop();
	std::cout << "Dead stop 3" << std::endl;

	// And also delete all
	delete asp;
	std::cout << "Dead delete 0" << std::endl;
	//This one cause a crash...
	//delete aiod;
	std::cout << "Dead delete 1" << std::endl;
	delete tgas;
	std::cout << "Dead delete 2" << std::endl;
	delete adm;
	std::cout << "Dead delete 3" << std::endl;
	delete midiInput;
	std::cout << "Dead delete 4" << std::endl;
  delete midiOutput;
  std::cout << "Dead delete 5" << std::endl;
	delete cb;
	std::cout << "Dead delete 6" << std::endl;
	delete bcr;
	std::cout << "Dead delete 7" << std::endl;

	return 0;
}
