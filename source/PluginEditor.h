#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

// ============================================================
// The Editor is the visual window the user sees and interacts with
// ============================================================
class SimpleEQAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    SimpleEQAudioProcessorEditor (SimpleEQAudioProcessor&);
    ~SimpleEQAudioProcessorEditor() override;

    void paint   (juce::Graphics&) override; // Draws the background
    void resized ()               override; // Positions the knobs

private:
    SimpleEQAudioProcessor& audioProcessor;

    // The 7 rotary knobs
    juce::Slider lowShelfFreqSlider,  lowShelfGainSlider;
    juce::Slider peakFreqSlider,      peakGainSlider,     peakQSlider;
    juce::Slider highShelfFreqSlider, highShelfGainSlider;

    // Attachments link knobs to plugin parameters automatically
    using APVTS      = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment lowShelfFreqAtt,  lowShelfGainAtt;
    Attachment peakFreqAtt,      peakGainAtt,     peakQAtt;
    Attachment highShelfFreqAtt, highShelfGainAtt;

    // Helper: returns all sliders as a list (makes looping easier)
    std::vector<juce::Slider*> getSliders();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleEQAudioProcessorEditor)
};