#ifndef OSCILLATOR_H_INCLUDED
#define OSCILLATOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class Oscillator : public Component,
                   public Slider::Listener
{
    public:
        Oscillator(double minFreq, double maxFreq, double midFreq, String label);
        ~Oscillator();
        double currentAngle, angleDelta;
        double currentSampleRate;
        //void paint(Graphics &g) override;
        void sliderValueChanged (Slider* slider) override;
        void resized() override;
        void updateAngleDelta();

    private:
        Slider oscFrequency;
        Label oscLabel;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscillator)
};

#endif
