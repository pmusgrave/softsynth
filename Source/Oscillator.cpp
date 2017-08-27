#include "Oscillator.h"

Oscillator::Oscillator(double sampleRate, String label)
{
    currentSampleRate = sampleRate;

    addAndMakeVisible (oscFrequency);
    oscFrequency.setRange (20, 20000);
    oscFrequency.setSkewFactorFromMidPoint (1000.0);
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
    oscFrequency.setBounds (100, 0, getWidth() - 100, 20);
    oscLabel.setBounds (0, 0, 100, 20);
}

void Oscillator::sliderValueChanged (Slider* slider)
{
    if (slider == &oscFrequency)
    {
        if (currentSampleRate > 0.0)
            Oscillator::updateAngleDelta();
    }
}

void Oscillator::updateAngleDelta()
{
    const double cyclesPerSample = oscFrequency.getValue() / currentSampleRate;
    angleDelta = cyclesPerSample * 2.0 * double_Pi;
}
