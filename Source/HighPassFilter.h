/*
  ==============================================================================

    HighPassFilter.h
    Created: 28 Jan 2022 8:01:47pm
    Author:  Zeno  Loesch

  ==============================================================================
*/

#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
class Filter {
public:
    enum FilterMode {
        FILTER_MODE_LOWPASS = 0,
        FILTER_MODE_HIGHPASS,
        FILTER_MODE_BANDPASS,
        kNumFilterModes
    };
    Filter() :
    cutoff(0.99),
    resonance(0.0),
    mode(FILTER_MODE_HIGHPASS),
    cutoffMod(0.0),
    buf0(0.0),
    buf1(0.0),
    buf2(0.0),
    buf3(0.0)
    {
        calculateFeedbackAmount();
    };
    double process(double inputValue);
    inline void setCutoff(double newCutoff) { cutoff = newCutoff; calculateFeedbackAmount(); };
    inline void setCutoffMod(double newCutoffMod) {
        cutoffMod = newCutoffMod;
        calculateFeedbackAmount();
    }
    inline void setResonance(double newResonance) { resonance = newResonance; calculateFeedbackAmount(); };
    inline void setFilterMode(FilterMode newMode) { mode = newMode; }
    inline double getCalculatedCutoff() const {
        return fmax(fmin(cutoff + cutoffMod, 0.99), 0.01);
    };
private:
    double cutoff;
    double resonance;
    FilterMode mode;
    double cutoffMod;
    double feedbackAmount;
    inline void calculateFeedbackAmount() {
        feedbackAmount = resonance + resonance/(1.0 - getCalculatedCutoff());
    }
    double buf0;
    double buf1;
    double buf2;
    double buf3;
    
};
