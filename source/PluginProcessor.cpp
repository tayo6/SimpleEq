#include "PluginProcessor.h"
#include "PluginEditor.h"

// ============================================================
// Reads all current knob values and returns them as one object
// ============================================================
ChainSettings getChainSettings (juce::AudioProcessorValueTreeState& apvts)
{
    ChainSettings settings;

    settings.lowShelfFreq      = apvts.getRawParameterValue ("LowShelfFreq")->load();
    settings.lowShelfGainDb    = apvts.getRawParameterValue ("LowShelfGain")->load();
    settings.peakFreq          = apvts.getRawParameterValue ("PeakFreq")->load();
    settings.peakGainDb        = apvts.getRawParameterValue ("PeakGain")->load();
    settings.peakQ             = apvts.getRawParameterValue ("PeakQ")->load();
    settings.highShelfFreq     = apvts.getRawParameterValue ("HighShelfFreq")->load();
    settings.highShelfGainDb   = apvts.getRawParameterValue ("HighShelfGain")->load();

    return settings;
}

// ============================================================
// Constructor: sets up stereo input and output
// ============================================================
SimpleEQAudioProcessor::SimpleEQAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
}

SimpleEQAudioProcessor::~SimpleEQAudioProcessor() {}

// ============================================================
// Standard plugin info — DAWs use these
// ============================================================
const juce::String SimpleEQAudioProcessor::getName() const        { return JucePlugin_Name; }
bool SimpleEQAudioProcessor::acceptsMidi() const                  { return false; }
bool SimpleEQAudioProcessor::producesMidi() const                 { return false; }
bool SimpleEQAudioProcessor::isMidiEffect() const                 { return false; }
double SimpleEQAudioProcessor::getTailLengthSeconds() const       { return 0.0; }
int SimpleEQAudioProcessor::getNumPrograms()                      { return 1; }
int SimpleEQAudioProcessor::getCurrentProgram()                   { return 0; }
void SimpleEQAudioProcessor::setCurrentProgram (int)              {}
const juce::String SimpleEQAudioProcessor::getProgramName (int)   { return {}; }
void SimpleEQAudioProcessor::changeProgramName (int, const juce::String&) {}

// ============================================================
// prepareToPlay: called once before audio starts playing
// We set up the filters here with the sample rate info
// ============================================================
void SimpleEQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // ProcessSpec tells JUCE what kind of audio is coming
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels      = 1; // Each chain handles one channel (mono)
    spec.sampleRate       = sampleRate;

    leftChain.prepare (spec);
    rightChain.prepare (spec);

    // Set filter coefficients for the first time
    updateFilters();
}

void SimpleEQAudioProcessor::releaseResources() {}

bool SimpleEQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // We only support stereo in and stereo out
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
    return true;
}

// ============================================================
// processBlock: the most important function
// The DAW calls this constantly with chunks of audio
// We apply the EQ filters to the audio here
// ============================================================
void SimpleEQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                            juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    // Make sure knob changes are applied in real-time
    updateFilters();

    // Wrap the buffer in an AudioBlock for the DSP module to use
    juce::dsp::AudioBlock<float> block (buffer);

    // Split left and right channels
    auto leftBlock  = block.getSingleChannelBlock (0);
    auto rightBlock = block.getSingleChannelBlock (1);

    // Process each channel through its own filter chain
    juce::dsp::ProcessContextReplacing<float> leftContext  (leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext (rightBlock);

    leftChain.process  (leftContext);
    rightChain.process (rightContext);
}

// ============================================================
// Creates the editor (the visual UI window)
// ============================================================
juce::AudioProcessorEditor* SimpleEQAudioProcessor::createEditor()
{
    return new SimpleEQAudioProcessorEditor (*this);
}

bool SimpleEQAudioProcessor::hasEditor() const { return true; }

// ============================================================
// Save/Load plugin state (so knob positions are remembered)
// ============================================================
void SimpleEQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos (destData, true);
    apvts.state.writeToStream (mos);
}

void SimpleEQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData (data, sizeInBytes);
    if (tree.isValid())
        apvts.replaceState (tree);
}

// ============================================================
// Define all the knobs (parameters) the plugin will have
// Each parameter has: ID, display name, range, and default value
// ============================================================
juce::AudioProcessorValueTreeState::ParameterLayout
    SimpleEQAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // --- LOW SHELF ---
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "LowShelfFreq", "Low Shelf Freq",
        juce::NormalisableRange<float> (20.0f, 1000.0f, 1.0f, 0.5f), // skew 0.5 = log feel
        200.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "LowShelfGain", "Low Shelf Gain",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.5f, 1.0f),
        0.0f));

    // --- MID PEAK ---
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "PeakFreq", "Peak Freq",
        juce::NormalisableRange<float> (20.0f, 20000.0f, 1.0f, 0.5f),
        1000.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "PeakGain", "Peak Gain",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.5f, 1.0f),
        0.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "PeakQ", "Peak Q",
        juce::NormalisableRange<float> (0.1f, 10.0f, 0.05f, 1.0f),
        1.0f));

    // --- HIGH SHELF ---
    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "HighShelfFreq", "High Shelf Freq",
        juce::NormalisableRange<float> (1000.0f, 20000.0f, 1.0f, 0.5f),
        8000.0f));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        "HighShelfGain", "High Shelf Gain",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.5f, 1.0f),
        0.0f));

    return layout;
}

// ============================================================
// Filter update functions
// These calculate the math coefficients for each EQ band
// and apply them to both left and right chains
// ============================================================
void SimpleEQAudioProcessor::updateLowShelfFilter (const ChainSettings& settings)
{
    auto coefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf (
        getSampleRate(),
        settings.lowShelfFreq,
        0.707f, // standard Q for a shelf filter
        juce::Decibels::decibelsToGain (settings.lowShelfGainDb));

    *leftChain .get<ChainPositions::LowShelf>().coefficients = *coefficients;
    *rightChain.get<ChainPositions::LowShelf>().coefficients = *coefficients;
}

void SimpleEQAudioProcessor::updatePeakFilter (const ChainSettings& settings)
{
    auto coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter (
        getSampleRate(),
        settings.peakFreq,
        settings.peakQ,
        juce::Decibels::decibelsToGain (settings.peakGainDb));

    *leftChain .get<ChainPositions::Peak>().coefficients = *coefficients;
    *rightChain.get<ChainPositions::Peak>().coefficients = *coefficients;
}

void SimpleEQAudioProcessor::updateHighShelfFilter (const ChainSettings& settings)
{
    auto coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf (
        getSampleRate(),
        settings.highShelfFreq,
        0.707f,
        juce::Decibels::decibelsToGain (settings.highShelfGainDb));

    *leftChain .get<ChainPositions::HighShelf>().coefficients = *coefficients;
    *rightChain.get<ChainPositions::HighShelf>().coefficients = *coefficients;
}

void SimpleEQAudioProcessor::updateFilters()
{
    auto settings = getChainSettings (apvts);
    updateLowShelfFilter  (settings);
    updatePeakFilter      (settings);
    updateHighShelfFilter (settings);
}

// ============================================================
// This line is required — it's the entry point for the plugin
// The DAW calls this to create an instance of your plugin
// ============================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleEQAudioProcessor();
}