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

// NB : using this class would have work as well
// https://docs.juce.com/master/classSynthesiser.html
// ==> It is entire MIDI synth

class MultipleToneGeneratorAudioSource : public juce::AudioSource
{
//private:
  // Care... This is not two pointers
  //juce::ToneGeneratorAudioSource* m_tgas1, m_tgas2;
  // Good way :
  //juce::ToneGeneratorAudioSource* m_tgas1;
  //juce::ToneGeneratorAudioSource* m_tgas2;

  // Code largely inspired from modules/juce_audio_basics/sources/juce_ToneGeneratorAudioSource.cpp
  // But too lazy to make use of an array & and struct

// No lust for writing that much mutators...
public:
	double frequency1, sampleRate1;
	double currentPhase1, phasePerSample1;
	double amplitude1;

	double frequency2, sampleRate2;
	double currentPhase2, phasePerSample2;
	double amplitude2;

	double frequency3, sampleRate3;
	double currentPhase3, phasePerSample3;
	double amplitude3;

	double frequency4, sampleRate4;
	double currentPhase4, phasePerSample4;
	double amplitude4;

	bool muteosc1, muteosc2, muteosc3, muteosc4;
	double timeshift1, timeshift2, timeshift3, timeshift4;

public:
  MultipleToneGeneratorAudioSource();
  ~MultipleToneGeneratorAudioSource();

	/** Implementation of the AudioSource method. */
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override {}

	/** Implementation of the AudioSource method. */
	void releaseResources() override {}

	/** Implementation of the AudioSource method. */
	void getNextAudioBlock(const AudioSourceChannelInfo&) override;
};

class TGAS : public juce::ToneGeneratorAudioSource
{
public:
  void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
};

// Call back executed in the audio thread...
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

bool done = false;
bool useSimpleSource = false; // Allow a fallback on ToneGeneratorAudioSource
juce::AudioDeviceManager* adm = nullptr;
juce::AudioIODevice* aiod = nullptr;
juce::MidiOutput* midiOutput = nullptr;
juce::MidiInput* midiInput = nullptr;
juce::AudioSourcePlayer* asp = nullptr;
BCR2000Callback* cb = nullptr;
BCR2000* bcr = nullptr;
TGAS* tgas = nullptr;
MultipleToneGeneratorAudioSource* mtgas = nullptr;

MultipleToneGeneratorAudioSource::MultipleToneGeneratorAudioSource()
{
  // Source https://en.wikipedia.org/wiki/Piano_key_frequencies
	frequency1 = 440.0;
	sampleRate1 = 44100.0;
	currentPhase1 = 0.0;
	phasePerSample1 = 0.0;
	amplitude1 = 0.5f;

	frequency2 = 494.0;
	sampleRate2 = 44100.0;
	currentPhase2 = 0.0;
	phasePerSample2 = 0.0;
	amplitude2 = 0.5f;

	frequency3 = 523.0;
	sampleRate3 = 44100.0;
	currentPhase3 = 0.0;
	phasePerSample3 = 0.0;
	amplitude3 = 0.5f;

  frequency4 = 587.0;
	sampleRate4 = 44100.0;
	currentPhase4 = 0.0;
	phasePerSample4 = 0.0;
	amplitude4 = 0.5f;

	timeshift1 = 0.0;
	timeshift2 = 0.0;
	timeshift3 = 0.0;
	timeshift4 = 0.0;

	muteosc1 = true;
	muteosc2 = true;
	muteosc3 = true;
	muteosc4 = true;
}

MultipleToneGeneratorAudioSource::~MultipleToneGeneratorAudioSource()
{
}

void MultipleToneGeneratorAudioSource::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
  // Not the way to go...
	//bufferToFill.buff.clear(); // Or bufferToFill.clearActiveBufferRegion();
	//bufferToFill.buff.addFrom(); // We don't have access to the buffers of m_tgas1 & m_tgas2 !

	/*
	for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
	{
		bufferToFill.buff.addSample(i, startSample, currentSample);
	}
	*/

	// Original implementation of setFrequency reset the phasePerSample...
	if (phasePerSample1 == 0.0)
	{
		phasePerSample1 = MathConstants<double>::twoPi / (sampleRate1 / frequency1);
	}
	if (phasePerSample2 == 0.0)
	{
		phasePerSample2 = MathConstants<double>::twoPi / (sampleRate2 / frequency2);
	}
	if (phasePerSample3 == 0.0)
	{
		phasePerSample3 = MathConstants<double>::twoPi / (sampleRate3 / frequency3);
	}
	if (phasePerSample4 == 0.0)
	{
		phasePerSample4 = MathConstants<double>::twoPi / (sampleRate4 / frequency4);
	}

	for (int i = 0; i < bufferToFill.numSamples; ++i)
	{
	  //TODO: Next step is to handle per osc envelope !
		const float sample1 = amplitude1 * ((float) std::sin(currentPhase1 + timeshift1));
		currentPhase1 += phasePerSample1;
		const float sample2 = amplitude2 * ((float) std::sin(currentPhase2 + timeshift2));
		currentPhase2 += phasePerSample2;
		const float sample3 = amplitude3 * ((float) std::sin(currentPhase3 + timeshift3));
		currentPhase3 += phasePerSample3;
		const float sample4 = amplitude4 * ((float) std::sin(currentPhase4 + timeshift4));
		currentPhase4 += phasePerSample4;

		float sample = 0.0;
		if (not muteosc1)
		{
		  sample += sample1;
		}
		if (not muteosc2)
		{
		  sample += sample2;
		}
		if (not muteosc3)
		{
		  sample += sample3;
		}
		if (not muteosc4)
		{
		  sample += sample4;
		}
		for (int j = bufferToFill.buffer->getNumChannels(); --j >= 0;)
		{
			bufferToFill.buffer->setSample (j, bufferToFill.startSample + i, sample);
		}
	}
}

void BCR2000Callback::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
	std::cout << "Incoming message :" << std::endl;
	const juce::uint8* p = message.getRawData();
	for (int i = 0; i < message.getRawDataSize(); i++)
	{
		std::cout << std::hex << static_cast<int>(*(p + i)) << std::endl;
	}

	// byte 0 seems to be always "B0" :-)
	int byte1 = static_cast<int>(*(p + 1)); // Control
	int byte2 = static_cast<int>(*(p + 2)); // Value

	if (useSimpleSource)
	{
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
		  tgas->setFrequency(220);
			tgas->setAmplitude(0.4);
		}
	}
	else
	{
	  // Buttons

	  if ((static_cast<int>(*(p + 1))) == 0x41)
		{
			mtgas->muteosc1 = !mtgas->muteosc1;
			std::cout << "New muteosc1 = " << mtgas->muteosc1 << std::endl;
		}
	  if ((static_cast<int>(*(p + 1))) == 0x42)
		{
			mtgas->muteosc2 = !mtgas->muteosc2;
			std::cout << "New muteosc2 = " << mtgas->muteosc2 << std::endl;
		}
	  if ((static_cast<int>(*(p + 1))) == 0x43)
		{
			mtgas->muteosc3 = !mtgas->muteosc3;
			std::cout << "New muteosc3 = " << mtgas->muteosc3 << std::endl;
		}
	  if ((static_cast<int>(*(p + 1))) == 0x44)
		{
			mtgas->muteosc4 = !mtgas->muteosc4;
			std::cout << "New muteosc4 = " << mtgas->muteosc4 << std::endl;
		}

		if ((static_cast<int>(*(p + 1))) == 0x48) // Last button first row !
		{
			done = true;
		}

		// Knobs

		// First row

		if ((static_cast<int>(*(p + 1))) == 0x01)
		{
			std::cout << "New frequency1 = " << ((static_cast<int>(*(p + 2)) * 4)) + 50 << std::endl;
			mtgas->frequency1 = (((static_cast<int>(*(p + 2)) * 4)) + 50); // So between 50 & 562
			mtgas->phasePerSample1 = 0.0;
		}

		// No impact ?
		if ((static_cast<int>(*(p + 1))) == 0x02)
		{
			std::cout << "New sampleRate1 = " << (44100.0 / ((static_cast<int>(*(p + 2))) / 127.0)) << std::endl;
			mtgas->sampleRate1 = (44100.0 / ((static_cast<int>(*(p + 2))) / 127.0));
		}

		// Meaning less ?
		if ((static_cast<int>(*(p + 1))) == 0x03)
		{
			std::cout << "New  currentPhase1 = " << (MathConstants<double>::twoPi / ((static_cast<int>(*(p + 2))) / 127.0)) << std::endl;
			mtgas->currentPhase1 = (MathConstants<double>::twoPi / ((static_cast<int>(*(p + 2))) / 127.0));
		}

		if ((static_cast<int>(*(p + 1))) == 0x04)
		{
			std::cout << "New  phasePerSample1 = " << (MathConstants<double>::twoPi / ((static_cast<int>(*(p + 2))) / 127.0)) << std::endl;
			mtgas->phasePerSample1 = (MathConstants<double>::twoPi / ((static_cast<int>(*(p + 2))) / 127.0));
		}

		if ((static_cast<int>(*(p + 1))) == 0x05)
		{
			std::cout << "New amplitude1 = " << (static_cast<int>(*(p + 2))) / 127.0 << std::endl;
			mtgas->amplitude1 = (static_cast<int>(*(p + 2))) / 127.0;
			//mtgas->phasePerSample1 = 0.0;
		}

		if ((static_cast<int>(*(p + 1))) == 0x06)
		{
			std::cout << "New timeshift1 = " << (static_cast<int>(*(p + 2))) << std::endl;
			mtgas->timeshift1 = (static_cast<int>(*(p + 2)));
		}

		// Second row

		if (byte1 == 0x51)
		{
			std::cout << "New frequency2 = " << (byte2 * 4) + 50 << std::endl;
			mtgas->frequency2 = ((byte2 * 4)) + 50;
			mtgas->phasePerSample2 = 0.0;
		}

		// No impact ?
		if (byte1 == 0x52)
		{
			std::cout << "New sampleRate2 = " << (44100.0 / (byte2 / 127.0)) << std::endl;
			mtgas->sampleRate2 = (44100.0 / (byte2 / 127.0));
		}

		// Meaning less ?
		if (byte1 == 0x53)
		{
			std::cout << "New  currentPhase2 = " << (MathConstants<double>::twoPi / (byte2 / 127.0)) << std::endl;
			mtgas->currentPhase2 = (MathConstants<double>::twoPi / (byte2 / 127.0));
		}

		if (byte1 == 0x54)
		{
			std::cout << "New  phasePerSample2 = " << (MathConstants<double>::twoPi / (byte2 / 127.0)) << std::endl;
			mtgas->phasePerSample2 = (MathConstants<double>::twoPi / (byte2 / 127.0));
		}

		if (byte1 == 0x55)
		{
			std::cout << "New amplitude2 = " << byte2 / 127.0 << std::endl;
			mtgas->amplitude2 = byte2 / 127.0;
			//mtgas->phasePerSample1 = 0.0;
		}

		if (byte1 = 0x56)
		{
			std::cout << "New timeshift2 = " << byte2 << std::endl;
			mtgas->timeshift2 = (byte2);
		}

		// Third row

		// and the fourth row !
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

	mtgas = new MultipleToneGeneratorAudioSource();
	if (!mtgas)
	{
		std::cout << "Oups : mtgas is null" << std::endl;
		return 0;
	}

	asp = new AudioSourcePlayer();
	if (!asp)
	{
		std::cout << "Oups : asp is null" << std::endl;
		return 0;
	}
	if (useSimpleSource)
	{
	  asp->setSource(tgas);
	  tgas->prepareToPlay(aiod->getDefaultBufferSize(), aiod->getCurrentSampleRate());
	}
	else
	{
	  asp->setSource(mtgas);
	  mtgas->prepareToPlay(aiod->getDefaultBufferSize(), aiod->getCurrentSampleRate());
	}

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
	mtgas->releaseResources();
	std::cout << "Dead stop 2" << std::endl;
	adm->closeAudioDevice();
	std::cout << "Dead stop 3" << std::endl;
	midiInput->stop();
	std::cout << "Dead stop 4" << std::endl;

	// And also delete all
	delete asp;
	std::cout << "Dead delete 0" << std::endl;
	//This one cause a crash...
	//delete aiod;
  std::cout << "Dead delete 1" << std::endl;
	delete mtgas;
	std::cout << "Dead delete 2" << std::endl;
	delete tgas;
	std::cout << "Dead delete 3" << std::endl;
	delete adm;
	std::cout << "Dead delete 4" << std::endl;
	delete midiInput;
	std::cout << "Dead delete 5" << std::endl;
  delete midiOutput;
  std::cout << "Dead delete 6" << std::endl;
	delete cb;
	std::cout << "Dead delete 7" << std::endl;
	delete bcr;
	std::cout << "Dead delete 8" << std::endl;

	return 0;
}
