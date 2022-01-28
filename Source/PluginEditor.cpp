/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AuralZenAudioProcessorEditor::AuralZenAudioProcessorEditor (AuralZenAudioProcessor& p)
    : AudioProcessorEditor (&p),audioProcessor (p),
InGainAttachment(audioProcessor.apvts, "InGain", InGainSlider),
MidAttachment(audioProcessor.apvts,"Mid",MidSlider),
SideAttachment(audioProcessor.apvts,"Side",SideSlider),
HPFCutoffAttachment(audioProcessor.apvts, "HPFCutoff", HPFCutoffSlider),
ClippingAttachment(audioProcessor.apvts, "Clipping", ClippingSlider),
ClippingWetAttachment(audioProcessor.apvts, "ClippingWet", ClippingWetSlider),
OutGainAttachment(audioProcessor.apvts, "OutGain", OutGainSlider)

{
    for (auto* comp : getComps())   {
            addAndMakeVisible(comp);
                                    }
    setSize (400, 400);
    InGainLabel.setText("InGain", juce::dontSendNotification);
    InGainLabel.attachToComponent(&InGainSlider, true);
    MidLabel.setText("Mid", juce::dontSendNotification);
    MidLabel.attachToComponent(&MidSlider, true);
    SideLabel.setText("Side", juce::dontSendNotification);
    SideLabel.attachToComponent(&SideSlider, true);
    HPFCutoffLabel.setText("HPFCutoff", juce::dontSendNotification);
    HPFCutoffLabel.attachToComponent(&HPFCutoffSlider, true);
    ClippingLabel.setText("Clipping", juce::dontSendNotification);
    ClippingLabel.attachToComponent(&ClippingSlider, true);
    ClippingWetLabel.setText("ClippingWet", juce::dontSendNotification);
    ClippingWetLabel.attachToComponent(&ClippingWetSlider, true);
    OutGainLabel.setText("OutGain", juce::dontSendNotification);
    OutGainLabel.attachToComponent(&OutGainSlider, true);
}

AuralZenAudioProcessorEditor::~AuralZenAudioProcessorEditor()
{
}

//==============================================================================
void AuralZenAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.setColour(juce::Colours::black);
    InGainSlider.setBounds(80,0,300,50);
    MidSlider.setBounds(80, 50, 300 , 50);
    SideSlider.setBounds(80,100,300,50);
    HPFCutoffSlider.setBounds(80, 150, 300 , 50);
    ClippingSlider.setBounds(80,200,300,50);
    ClippingWetSlider.setBounds(80, 250, 300 , 50);
    OutGainSlider.setBounds(80, 300, 300 , 50);
    
}

void AuralZenAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

std::vector<juce::Component*> AuralZenAudioProcessorEditor::getComps()
{
    return
    {
        &InGainSlider,
        &InGainLabel,
        &MidSlider,
        &MidLabel,
        &SideSlider,
        &SideLabel,
        &HPFCutoffSlider,
        &HPFCutoffLabel,
        &ClippingSlider,
        &ClippingLabel,
        &ClippingWetSlider,
        &ClippingWetLabel,
        &OutGainSlider,
        &OutGainLabel
    };
}
