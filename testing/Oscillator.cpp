/*
  ==============================================================================

    Oscillator.cpp
    Created: 26 Aug 2017 9:13:39am
    Author:  paul

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Oscillator.h"

Oscillator(int xPos, int yPos){
    currentAngle = 0.0;
    angleDelta = 0.0;

    MainContentComponent::addAndMakeVisible (oscFrequencyKnob);
    oscFrequencyKnob.setRange (20, 20000);
    oscFrequencyKnob.setSkewFactorFromMidPoint (1000.0);
    oscFrequencyKnob.addListener (this);

    MainContentComponent::addAndMakeVisible (oscFrequencyLabel);
    oscFrequencyLabel.setText ("OSC FREQ", dontSendNotification);
    oscFrequencyLabel.attachToComponent (&oscFrequencyKnob, true);

    oscFrequencyKnob.setBounds (100, 10, getWidth() - 100, 20);
    oscFrequencyLabel.setBounds (10, 10, 100, 20);
}

~Oscillator()
{
}
