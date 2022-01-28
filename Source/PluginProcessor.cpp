/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AuralZenAudioProcessor::AuralZenAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

AuralZenAudioProcessor::~AuralZenAudioProcessor()
{
}

//==============================================================================
const juce::String AuralZenAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AuralZenAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AuralZenAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AuralZenAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AuralZenAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AuralZenAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AuralZenAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AuralZenAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AuralZenAudioProcessor::getProgramName (int index)
{
    return {};
}

void AuralZenAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AuralZenAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    InGainSmooth.reset(sampleRate, 0.001f);
    MidGainSmooth.reset(sampleRate, 0.001f);
    SideGainSmooth.reset(sampleRate, 0.001f);
    ClippingSmooth.reset(sampleRate, 0.001f);
    ClippingWetSmooth.reset(sampleRate, 0.001f);
    OutGainSmooth.reset(sampleRate, 0.001f);
    HPFSmooth.reset(sampleRate,0.001f);
}

void AuralZenAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AuralZenAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AuralZenAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    auto* inDataL = buffer.getWritePointer(0);
    auto* inDataR = buffer.getWritePointer(1);
    auto* outDataL = buffer.getWritePointer(0);
    auto* outDataR = buffer.getWritePointer(1);
    InGainSmooth.setTargetValue(apvts.getRawParameterValue("InGain") -> load());
    MidGainSmooth.setTargetValue(apvts.getRawParameterValue("Mid") -> load());
    SideGainSmooth.setTargetValue(apvts.getRawParameterValue("Side") -> load());
    HPFSmooth.setTargetValue(apvts.getRawParameterValue("HPFCutoff") ->load());
    ClippingSmooth.setTargetValue(apvts.getRawParameterValue("Clipping") -> load());
    ClippingWetSmooth.setTargetValue(apvts.getRawParameterValue("ClippingWet") -> load());
    
    OutGainSmooth.setTargetValue(apvts.getRawParameterValue("OutGain") -> load());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        ClipOffset = 1.f - ClippingWetSmooth.getNextValue();
        HPF.setCutoff(HPFSmooth.getNextValue());
        Mid = (0.5f*(inDataL[i] + inDataR[i]) * MidGainSmooth.getNextValue())*InGainSmooth.getNextValue();
        Side = (0.5f*(inDataL[i] - inDataR[i]) * SideGainSmooth.getNextValue()) * InGainSmooth.getNextValue();
        HighPassStage = + HPF.process(Mid+Side);
        ClippingStage =  ClipOffset * (HighPassStage) + (1.57079632679 *atan((HighPassStage) *ClippingSmooth.getNextValue())) * ClippingWetSmooth.getNextValue();
        outDataL[i] = (Mid + Side + ClippingStage) * OutGainSmooth.getNextValue();
        outDataR[i] = (Mid + Side + ClippingStage) * OutGainSmooth.getNextValue();
    }
}

//==============================================================================
bool AuralZenAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AuralZenAudioProcessor::createEditor()
{
    return new AuralZenAudioProcessorEditor (*this);
}

//==============================================================================
void AuralZenAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos (destData, true);
           apvts.state.writeToStream(mos);
}

void AuralZenAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
               if(tree.isValid() )
               {
                   apvts.replaceState(tree);
               }
}

juce::AudioProcessorValueTreeState::ParameterLayout
AuralZenAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("InGain",
                                                           "InGain",
                                                            0.0f,
                                                            1.f,
                                                            0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Mid",
                                                           "Mid",
                                                            0.0f,
                                                            5.f,
                                                            0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Side",
                                                           "Side",
                                                            0.0f,
                                                            5.f,
                                                            0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("HPFCutoff",
                                                           "HPFCutoff",
                                                            0.1f,
                                                            1.f,
                                                            0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Clipping",
                                                           "Clipping",
                                                            1.f,
                                                            10.f,
                                                            5.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("ClippingWet",
                                                           "ClippingWet",
                                                            .5f,
                                                            1.f,
                                                            .5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("OutGain",
                                                           "OutGain",
                                                            0.0f,
                                                            1.f,
                                                            0.5f));
    return layout;
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AuralZenAudioProcessor();
}
