#pragma once
#include <JuceHeader.h>

// ============================================================
// These lines define our 3-band EQ filter chain
// Filter = one EQ band
// MonoChain = 3 filters in a row (Low, Mid, High) for one channel
// ============================================================
using Filter = juce::dsp::IIR::Filter<float>;
using MonoChain = juce::dsp::ProcessorChain<Filter, Filter, Filter>;

// The order of bands in our chain (0, 1, 2)
enum ChainPositions
{
    LowShelf  = 0,
    Peak      = 1,
    HighShelf = 2
};

// A simple struct to hold all 7 EQ parameter values at once
struct ChainSettings
{
    float lowShelfFreq  { 200.0f },  lowShelfGainDb  { 0.0f };
    float peakFreq      { 1000.0f }, peakGainDb      { 0.0f }, peakQ { 1.0f };
    float highShelfFreq { 8000.0f }, highShelfGainDb { 0.0f };
};

// Helper function: reads current knob values from the plugin
ChainSettings getChainSettings (juce::AudioProcessorValueTreeState& apvts);

// ============================================================
// The main plugin class
// AudioProcessor = the brain that processes audio
// ============================================================
class SimpleEQAudioProcessor : public juce::AudioProcessor
{
public:
    SimpleEQAudioProcessor();
    ~SimpleEQAudioProcessor() override;

    // Called by the DAW before audio starts
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    // Called by the DAW for every block of audio — THIS is where EQ happens
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    // Save and load plugin state
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Creates all the knob parameters (Freq, Gain, Q)
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // apvts = AudioProcessorValueTreeState
    // This manages all the plugin's knob values
    juce::AudioProcessorValueTreeState apvts
    {
        *this, nullptr, "Parameters", createParameterLayout()
    };

private:
    // Two separate filter chains — one for left audio, one for right
    MonoChain leftChain, rightChain;

    // These functions calculate and apply the math for each EQ band
    void updateLowShelfFilter  (const ChainSettings& settings);
    void updatePeakFilter      (const ChainSettings& settings);
    void updateHighShelfFilter (const ChainSettings& settings);
    void updateFilters();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleEQAudioProcessor)
};