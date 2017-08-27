#ifndef OSCILLATOR_H_INCLUDED
#define OSCILLATOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class Oscillator : public Component,
                   public Slider::Listener
{
    public:
        Oscillator(double currentSampleRate, String label);
        ~Oscillator();
        //void paint(Graphics &g) override;
        void sliderValueChanged (Slider* slider) override;
        void resized() override;
        void updateAngleDelta();
        double currentAngle, angleDelta;

    private:
        double currentSampleRate;
        Slider oscFrequency;
        Label oscLabel;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscillator)
};

#endif
