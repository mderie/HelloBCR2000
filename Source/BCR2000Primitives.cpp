
#include "BCR2000Primitives.hpp"

void BCR2000::init()
{
	// Nothing ?
}

void BCR2000::sendMessage(const std::string &s)
{
  // Modern C++ :) http://www.cplusplus.com/reference/vector/vector/emplace_back/
  //std::string ss;
  //ss.emplace_back('c');

  //TODO: split s in pieces (of 512 bytes) if too long and use m_messageChainCounter for MSB/LSB

  std::string fullCommand;
  fullCommand.push_back(0xF0);
  fullCommand.push_back(0x00);
  fullCommand.push_back(0x20);
  fullCommand.push_back(0x32);
  fullCommand.push_back(0x7F); // DeviceId = Any
  fullCommand.push_back(0x7F); // Model = Any
  fullCommand.push_back(0x20);
  fullCommand.push_back(0x00); //TODO:MSB
  fullCommand.push_back(0x00); //TODO:LSB
  fullCommand += s;
  fullCommand.push_back(0x7F);

  m_out->sendMessageNow(juce::MidiMessage(fullCommand.c_str(), fullCommand.size(), 0));
}

void BCR2000::setValue(int i)
{
	m_out->sendMessageNow(juce::MidiMessage(0x00, 0x00, i));
}

int BCR2000::getValue()
{
	m_out->sendMessageNow(juce::MidiMessage(0x00, 0x00, 0x00));
	int result = 0;
	return result;
}
