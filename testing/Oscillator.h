/*
  ==============================================================================

    Oscillator.h
    Created: 26 Aug 2017 9:13:38am
    Author:  paul

  ==============================================================================
*/

#ifndef OSCILLATOR_H_INCLUDED
#define OSCILLATOR_H_INCLUDED

class Oscillator : public Component,
                   public Slider,
                   public Label
{
    public:
        Oscillator(int xPos, int yPos);
        ~Oscillator();
        Slider oscFrequencyKnob;
        Label oscFrequencyLabel;
        void updateAngleDelta();

    private:
        double currentSampleRate, currentAngle, angleDelta;
};

    #endif
