/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/

struct HorizontalSliderNoLabel : juce::Slider
{
    HorizontalSliderNoLabel() :juce::Slider(juce::Slider::LinearHorizontal,
                                        juce::Slider::TextEntryBoxPosition::NoTextBox)
    {
        
    }
};


class AuralZenAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AuralZenAudioProcessorEditor (AuralZenAudioProcessor&);
    ~AuralZenAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AuralZenAudioProcessor& audioProcessor;
    HorizontalSliderNoLabel InGainSlider, MidSlider, SideSlider, HPFCutoffSlider,
    ClippingSlider, ClippingWetSlider, OutGainSlider;
    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;
    
    Attachment InGainAttachment, MidAttachment, SideAttachment, HPFCutoffAttachment,
    ClippingAttachment, ClippingWetAttachment, OutGainAttachment;
    
    
    std::vector<juce::Component*> getComps();
    juce::Label InGainLabel, MidLabel, SideLabel, HPFCutoffLabel, ClippingLabel,
    ClippingWetLabel, OutGainLabel;
    
    
   
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AuralZenAudioProcessorEditor)
};
