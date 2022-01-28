/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "HighPassFilter.h"
//==============================================================================
/**
*/
class AuralZenAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    AuralZenAudioProcessor();
    ~AuralZenAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
           juce::AudioProcessorValueTreeState apvts {*this, nullptr, "Parameters", createParameterLayout()};
private:
    Filter HPF;
    juce::LinearSmoothedValue<float> InGainSmooth {0.f};
    juce::LinearSmoothedValue<float> MidGainSmooth {0.f};
    juce::LinearSmoothedValue<float> SideGainSmooth {0.f};
    juce::LinearSmoothedValue<float> ClippingSmooth {0.f};
    juce::LinearSmoothedValue<float> HPFSmooth {0.f};
    juce::LinearSmoothedValue<float> ClippingWetSmooth {0.f};
    juce::LinearSmoothedValue<float> OutGainSmooth {0.f};
    float Mid = 0, Side = 0, ClippingStage = 0, HighPassStage = 0;
    float ClipOffset = 0;
    float outputL = 0, outputR = 0;
    
     //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AuralZenAudioProcessor)
};
