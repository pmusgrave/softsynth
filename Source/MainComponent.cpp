/*
 JUCE doesn't split most auto-generated components into separate header files
 by default, so I'm leaving Maincomponent.cpp as a single file. Everything else
  is split into separate files for readability
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <cmath>

class MainContentComponent   : public AudioAppComponent,
                               public Slider::Listener
{
public:
    //==============================================================================
    MainContentComponent()
    :   currentSampleRate (0.0),
        currentAngle (0.0),
        angleDelta (0.0),
        currentLFOAngle (0.0),
        LFOAngleDelta (0.0),
        noiseLevel (0.0)
    {
        setSize (800, 600);
        setAudioChannels (0, 2);

        // oscillator
        addAndMakeVisible (oscFrequencyKnob);
        oscFrequencyKnob.setRange (20, 20000);
        oscFrequencyKnob.setSkewFactorFromMidPoint (1000.0);
        oscFrequencyKnob.addListener (this);

        addAndMakeVisible (oscFrequencyLabel);
        oscFrequencyLabel.setText ("OSC FREQ", dontSendNotification);
        oscFrequencyLabel.attachToComponent (&oscFrequencyKnob, true);

        // LFO
        addAndMakeVisible (lfoFrequencyKnob);
        lfoFrequencyKnob.setRange (0, 200);
        lfoFrequencyKnob.setSkewFactorFromMidPoint (100);
        lfoFrequencyKnob.addListener (this);

        addAndMakeVisible (lfoFrequencyLabel);
        lfoFrequencyLabel.setText ("LFO FREQ", dontSendNotification);
        lfoFrequencyLabel.attachToComponent (&lfoFrequencyKnob, true);

        // filter cutoff
        addAndMakeVisible (filterCutoffKnob);
        filterCutoffKnob.setRange (20.0, 20000.0);
        filterCutoffKnob.setSkewFactorFromMidPoint (1000.0);
        filterCutoffKnob.addListener (this);

        addAndMakeVisible (lfoFrequencyLabel);
        filterCutoffLabel.setText ("CUTOFF", dontSendNotification);
        filterCutoffLabel.attachToComponent (&filterCutoffKnob, true);

        // filter resonance
        addAndMakeVisible (filterResonanceKnob);
        filterResonanceKnob.setRange (0.1, 100);
        filterResonanceKnob.setSkewFactorFromMidPoint (50.0);
        filterResonanceKnob.addListener (this);

        addAndMakeVisible (lfoFrequencyLabel);
        filterResonanceLabel.setText ("RES", dontSendNotification);
        filterResonanceLabel.attachToComponent (&filterResonanceKnob, true);

        // ADSR Attack
        addAndMakeVisible (envAttackKnob);
        envAttackKnob.setRange (50.0, 5000.0);
        envAttackKnob.setSkewFactorFromMidPoint (500.0);
        envAttackKnob.addListener (this);

        addAndMakeVisible (lfoFrequencyLabel);
        envAttackLabel.setText ("ATTACK", dontSendNotification);
        envAttackLabel.attachToComponent (&envAttackKnob, true);

        // ADSR Decay
        addAndMakeVisible (envDecayKnob);
        envDecayKnob.setRange (50.0, 5000.0);
        envDecayKnob.setSkewFactorFromMidPoint (500.0);
        envDecayKnob.addListener (this);

        addAndMakeVisible (lfoFrequencyLabel);
        envDecayLabel.setText ("DECAY", dontSendNotification);
        envDecayLabel.attachToComponent (&envDecayKnob, true);

        // ADSR Sustain
        addAndMakeVisible (envSustainKnob);
        envSustainKnob.setRange (50.0, 5000.0);
        envSustainKnob.setSkewFactorFromMidPoint (500.0);
        envSustainKnob.addListener (this);

        addAndMakeVisible (lfoFrequencyLabel);
        envSustainLabel.setText ("SUSTAIN", dontSendNotification);
        envSustainLabel.attachToComponent (&envSustainKnob, true);

        // ADSR Release
        addAndMakeVisible (envReleaseKnob);
        envReleaseKnob.setRange (50.0, 5000.0);
        envReleaseKnob.setSkewFactorFromMidPoint (500.0);
        envReleaseKnob.addListener (this);

        addAndMakeVisible (lfoFrequencyLabel);
        envReleaseLabel.setText ("RELEASE", dontSendNotification);
        envReleaseLabel.attachToComponent (&envReleaseKnob, true);

        // white noise level
        addAndMakeVisible (noiseKnob);
        noiseKnob.setRange (0.0, 1.0);
        noiseKnob.setSkewFactorFromMidPoint (0.5);
        noiseKnob.addListener (this);

        addAndMakeVisible (lfoFrequencyLabel);
        noiseLabel.setText ("NOISE", dontSendNotification);
        noiseLabel.attachToComponent (&noiseKnob, true);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        String message;
        message << "Preparing to play audio...\n";
        message << " samplesPerBlockExpected = " << samplesPerBlockExpected << "\n";
        message << " sampleRate = " << sampleRate;
        currentSampleRate = sampleRate;
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
                const float currentSample = (float) std::sin (currentAngle + currentLFOAngle);
                currentAngle += angleDelta + currentLFOAngle;
                currentLFOAngle += LFOAngleDelta;

                float noise = (random.nextFloat() * 0.25f - 0.125f) * noiseLevel;
                buffer[sample] = ((currentSample) * level) + noise;
            }
        }
    }

    void updateAngleDelta()
    {
        const double cyclesPerSample = oscFrequencyKnob.getValue() / currentSampleRate;
        angleDelta = cyclesPerSample * 2.0 * double_Pi;
    }

    void updateLFOAngleDelta()
    {
        const double cyclesPerSample = lfoFrequencyKnob.getValue() / currentSampleRate;
        LFOAngleDelta = cyclesPerSample * 2.0 * double_Pi;
    }

    void updateNoiseLevel()
    {
        noiseLevel = noiseKnob.getValue();
    }

    void releaseResources() override
    {
        Logger::getCurrentLogger()->writeToLog ("Releasing audio resources");
    }

    void sliderValueChanged (Slider* slider) override
    {
        if (slider == &oscFrequencyKnob)
        {
            if (currentSampleRate > 0.0)
                updateAngleDelta();
        }

        if (slider == &lfoFrequencyKnob)
        {
            if (currentSampleRate > 0.0)
                updateLFOAngleDelta();
        }

        if (slider == &noiseKnob)
        {
            if (currentSampleRate > 0.0)
                updateNoiseLevel();
        }
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

        // You can add your drawing code here!
    }

    void resized() override
    {
        oscFrequencyKnob.setBounds (100, 10, getWidth() - 100, 20);
        lfoFrequencyKnob.setBounds (100, 30, getWidth() - 100, 20);
        filterCutoffKnob.setBounds (100, 50, getWidth() - 100, 20);
        filterResonanceKnob.setBounds (100, 70, getWidth() - 100, 20);
        envAttackKnob.setBounds (100, 90, getWidth() - 100, 20);
        envDecayKnob.setBounds (100, 110, getWidth() - 100, 20);
        envSustainKnob.setBounds (100, 130, getWidth() - 100, 20);
        envReleaseKnob.setBounds (100, 150, getWidth() - 100, 20);
        noiseKnob.setBounds (100, 170, getWidth() - 100, 20);

        oscFrequencyLabel.setBounds (10, 10, 100, 20);
        lfoFrequencyLabel.setBounds (10, 30, 100, 20);
        filterCutoffLabel.setBounds (10, 50, 100, 20);
        filterResonanceLabel.setBounds (10, 70, 100, 20);
        envAttackLabel.setBounds (10, 90, 100, 20);
        envDecayLabel.setBounds (10, 110, 100, 20);
        envSustainLabel.setBounds (10, 130, 100, 20);
        envReleaseLabel.setBounds (10, 150, 100, 20);
        noiseLabel.setBounds (10, 170, 100, 20);
    }


private:
    Random random;

    Slider oscFrequencyKnob;
    Slider lfoFrequencyKnob;
    Slider filterCutoffKnob;
    Slider filterResonanceKnob;
    Slider envAttackKnob;
    Slider envDecayKnob;
    Slider envSustainKnob;
    Slider envReleaseKnob;
    Slider noiseKnob;

    Label oscFrequencyLabel;
    Label lfoFrequencyLabel;
    Label filterCutoffLabel;
    Label filterResonanceLabel;
    Label envAttackLabel;
    Label envDecayLabel;
    Label envSustainLabel;
    Label envReleaseLabel;
    Label noiseLabel;

    double currentSampleRate, currentAngle, angleDelta, currentLFOAngle, LFOAngleDelta;
    double noiseLevel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif
