#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "Oscillator.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <cmath>

class MainContentComponent   : public AudioAppComponent,
                               public Slider::Listener
{
public:
    MainContentComponent()
    {
        setSize (800, 100);
        setAudioChannels (0, 2);

        addAndMakeVisible(osc1);
        addAndMakeVisible(osc2);
        addAndMakeVisible(lfo);

        // white noise level
        addAndMakeVisible (noiseKnob);
        noiseKnob.setRange (0.0, 1.0);
        noiseKnob.setSkewFactorFromMidPoint (0.5);
        noiseKnob.addListener (this);

        noiseLabel.setText ("NOISE", dontSendNotification);
        noiseLabel.attachToComponent (&noiseKnob, true);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    void paint (Graphics& g) override
    {
        resized();
    }

    void resized() override
    {
        osc1.setBounds (0, 10, getWidth(), 20);
        osc2.setBounds (0, 30, getWidth(), 20);
        lfo.setBounds (0, 50, getWidth(), 20);

        noiseKnob.setBounds (100, 70, getWidth() - 100, 20);
        noiseLabel.setBounds (0, 70, 100, 20);
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        String message;
        message << "Preparing to play audio...\n";
        message << " samplesPerBlockExpected = " << samplesPerBlockExpected << "\n";
        message << " sampleRate = " << sampleRate;
        currentSampleRate = sampleRate;
        osc1.currentSampleRate = sampleRate;
        osc2.currentSampleRate = sampleRate;
        lfo.currentSampleRate = sampleRate;
        Logger::getCurrentLogger()->writeToLog (message);
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
        {
            const float level = 0.25f;
            float* const buffer = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);

            for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                const float currentSample = (float) std::sin (osc1.currentAngle + (LFODepth * std::sin(lfo.currentAngle))) + std::sin(osc2.currentAngle + (LFODepth * std::sin(lfo.currentAngle)));
                osc1.currentAngle += osc1.angleDelta;
                osc2.currentAngle += osc2.angleDelta;
                lfo.currentAngle += lfo.angleDelta;

                float noise = (random.nextFloat() * 0.25f - 0.125f) * noiseLevel;
                buffer[sample] = ((currentSample) * level) + noise;
            }
        }
    }

    void releaseResources() override
    {
        Logger::getCurrentLogger()->writeToLog ("Releasing audio resources");
    }

    void sliderValueChanged (Slider* slider) override
    {
        if (slider == &noiseKnob)
        {
            if (currentSampleRate > 0.0)
                noiseLevel = noiseKnob.getValue();
        }
    }

private:
    Random random;

    Oscillator osc1 {20, 20000, 1000, "OSC1"};
    Oscillator osc2 {20, 20000, 1000, "OSC2"};
    Oscillator lfo {0.1, 200, 50, "LFO"};

    Slider noiseKnob;

    Label noiseLabel;

    double currentSampleRate = 0;
    double LFODepth = 50;
    double noiseLevel = 0;;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif
