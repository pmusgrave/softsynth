#include "Oscillator.h"
#include <iostream>

Oscillator::Oscillator(double minFreq, double maxFreq, double midFreq, String label)
{
    addAndMakeVisible (oscFrequency);
    oscFrequency.setRange (minFreq, maxFreq);
    oscFrequency.setSkewFactorFromMidPoint (midFreq);
    oscFrequency.addListener (this);

    addAndMakeVisible (oscLabel);
    oscLabel.setText (label, dontSendNotification);
    oscLabel.attachToComponent (&oscFrequency, true);
}

Oscillator::~Oscillator()
{
}

void Oscillator::resized()
{
    oscFrequency.setBounds (100, 0, getWidth(), 20);
    oscLabel.setBounds (10, 0, 100, 20);
}

void Oscillator::sliderValueChanged (Slider* slider)
{
    Oscillator::updateAngleDelta();
}

void Oscillator::updateAngleDelta()
{
    const double cyclesPerSample = oscFrequency.getValue() / currentSampleRate;
    angleDelta = cyclesPerSample * 2.0 * double_Pi;
}
