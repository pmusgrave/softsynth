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
        LFODepth (50),
        noiseLevel (0.0)
    {
        setSize (800, 100);
        setAudioChannels (0, 2);

        // oscillator
        addAndMakeVisible (oscFrequencyKnob);
        oscFrequencyKnob.setRange (20, 20000);
        oscFrequencyKnob.setSkewFactorFromMidPoint (1000.0);
        oscFrequencyKnob.addListener (this);

        addAndMakeVisible (oscFrequencyLabel);
        oscFrequencyLabel.setText ("OSC1 FREQ", dontSendNotification);
        oscFrequencyLabel.attachToComponent (&oscFrequencyKnob, true);

        // oscillator 2
        addAndMakeVisible (osc2FrequencyKnob);
        osc2FrequencyKnob.setRange (20, 20000);
        osc2FrequencyKnob.setSkewFactorFromMidPoint (1000.0);
        osc2FrequencyKnob.addListener (this);

        addAndMakeVisible (osc2FrequencyLabel);
        osc2FrequencyLabel.setText ("OSC2 FREQ", dontSendNotification);
        osc2FrequencyLabel.attachToComponent (&osc2FrequencyKnob, true);

        // LFO
        addAndMakeVisible (lfoFrequencyKnob);
        lfoFrequencyKnob.setRange (0, 200);
        lfoFrequencyKnob.setSkewFactorFromMidPoint (100);
        lfoFrequencyKnob.addListener (this);

        addAndMakeVisible (lfoFrequencyLabel);
        lfoFrequencyLabel.setText ("LFO FREQ", dontSendNotification);
        lfoFrequencyLabel.attachToComponent (&lfoFrequencyKnob, true);

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
                const float currentSample = (float) (std::sin (currentAngle + (LFODepth * std::sin(currentLFOAngle)))) + std::sin(currentAngle2 + (LFODepth * std::sin(currentLFOAngle)));
                currentAngle += angleDelta;
                currentAngle2 += angleDelta2;
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

    void updateAngleDelta2()
    {
        const double cyclesPerSample = osc2FrequencyKnob.getValue() / currentSampleRate;
        angleDelta2 = cyclesPerSample * 2.0 * double_Pi;
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

        else if (slider == &osc2FrequencyKnob)
        {
            if (currentSampleRate > 0.0)
                updateAngleDelta2();
        }

        else if (slider == &lfoFrequencyKnob)
        {
            if (currentSampleRate > 0.0)
                updateLFOAngleDelta();
        }

        else if (slider == &noiseKnob)
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
        osc2FrequencyKnob.setBounds (100, 30, getWidth() - 100, 20);
        lfoFrequencyKnob.setBounds (100, 50, getWidth() - 100, 20);
        noiseKnob.setBounds (100, 70, getWidth() - 100, 20);

        oscFrequencyLabel.setBounds (10, 10, 100, 20);
        osc2FrequencyLabel.setBounds (10, 30, 100, 20);
        lfoFrequencyLabel.setBounds (10, 50, 100, 20);
        noiseLabel.setBounds (10, 70, 100, 20);
    }


private:
    Random random;

    IIRFilter lpf;

    Slider oscFrequencyKnob;
    Slider osc2FrequencyKnob;
    Slider lfoFrequencyKnob;

    Slider noiseKnob;

    Label oscFrequencyLabel;
    Label osc2FrequencyLabel;
    Label lfoFrequencyLabel;

    Label noiseLabel;

    double currentSampleRate, currentAngle, angleDelta;
    double currentAngle2, angleDelta2;
    double currentLFOAngle, LFOAngleDelta, LFODepth;
    double noiseLevel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif
