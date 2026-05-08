#include "PluginProcessor.h"
#include "PluginEditor.h"

// ============================================================
// Constructor: set up all the knobs and link them to parameters
// ============================================================
SimpleEQAudioProcessorEditor::SimpleEQAudioProcessorEditor (SimpleEQAudioProcessor& p)
    : AudioProcessorEditor (&p),
      audioProcessor (p),
      // Each attachment links a slider to a parameter by ID string
      lowShelfFreqAtt  (p.apvts, "LowShelfFreq",  lowShelfFreqSlider),
      lowShelfGainAtt  (p.apvts, "LowShelfGain",  lowShelfGainSlider),
      peakFreqAtt      (p.apvts, "PeakFreq",       peakFreqSlider),
      peakGainAtt      (p.apvts, "PeakGain",       peakGainSlider),
      peakQAtt         (p.apvts, "PeakQ",          peakQSlider),
      highShelfFreqAtt (p.apvts, "HighShelfFreq",  highShelfFreqSlider),
      highShelfGainAtt (p.apvts, "HighShelfGain",  highShelfGainSlider)
{
    // Make all sliders rotary knobs with a value box below
    for (auto* slider : getSliders())
    {
        slider->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        slider->setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20);
        addAndMakeVisible (slider);
    }

    setSize (700, 280);
}

SimpleEQAudioProcessorEditor::~SimpleEQAudioProcessorEditor() {}

// ============================================================
// paint: draws the background and text labels
// ============================================================
void SimpleEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Dark background
    g.fillAll (juce::Colour (30, 30, 30));

    int w = getWidth();
    int bandW = w / 3;

    // Draw coloured band background panels
    g.setColour (juce::Colour (40, 60, 90).withAlpha (0.6f));
    g.fillRect (0, 0, bandW, getHeight());

    g.setColour (juce::Colour (40, 90, 60).withAlpha (0.6f));
    g.fillRect (bandW, 0, bandW, getHeight());

    g.setColour (juce::Colour (90, 70, 30).withAlpha (0.6f));
    g.fillRect (bandW * 2, 0, bandW, getHeight());

    // Band titles
    g.setFont (juce::Font (14.0f, juce::Font::bold));

    g.setColour (juce::Colours::lightblue);
    g.drawFittedText ("LOW SHELF", 0, 8, bandW, 20,
                       juce::Justification::centred, 1);

    g.setColour (juce::Colours::lightgreen);
    g.drawFittedText ("MID PEAK", bandW, 8, bandW, 20,
                       juce::Justification::centred, 1);

    g.setColour (juce::Colours::lightyellow);
    g.drawFittedText ("HIGH SHELF", bandW * 2, 8, bandW, 20,
                       juce::Justification::centred, 1);

    // Knob labels
    g.setFont (11.0f);
    g.setColour (juce::Colours::white.withAlpha (0.7f));

    int labelY = getHeight() - 18;

    g.drawFittedText ("Freq",  10,           labelY, 80, 16, juce::Justification::centred, 1);
    g.drawFittedText ("Gain",  bandW / 2,    labelY, 80, 16, juce::Justification::centred, 1);

    g.drawFittedText ("Freq",  bandW + 5,              labelY, 80, 16, juce::Justification::centred, 1);
    g.drawFittedText ("Gain",  bandW + bandW / 3,      labelY, 80, 16, juce::Justification::centred, 1);
    g.drawFittedText ("Q",     bandW + (bandW / 3) * 2, labelY, 80, 16, juce::Justification::centred, 1);

    g.drawFittedText ("Freq",  bandW * 2 + 10,        labelY, 80, 16, juce::Justification::centred, 1);
    g.drawFittedText ("Gain",  bandW * 2 + bandW / 2, labelY, 80, 16, juce::Justification::centred, 1);
}

// ============================================================
// resized: positions each knob on screen
// ============================================================
void SimpleEQAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop (30); // Space for band title labels

    int w      = bounds.getWidth();
    int h      = bounds.getHeight() - 20; // Leave room for knob labels at bottom
    int bandW  = w / 3;
    int halfBW = bandW / 2;
    int thirdBW = bandW / 3;

    // --- LOW SHELF (left third) ---
    lowShelfFreqSlider .setBounds (0,      30, halfBW,  h);
    lowShelfGainSlider .setBounds (halfBW, 30, halfBW,  h);

    // --- MID PEAK (middle third) ---
    peakFreqSlider .setBounds (bandW,               30, thirdBW, h);
    peakGainSlider .setBounds (bandW + thirdBW,     30, thirdBW, h);
    peakQSlider    .setBounds (bandW + thirdBW * 2, 30, thirdBW, h);

    // --- HIGH SHELF (right third) ---
    highShelfFreqSlider .setBounds (bandW * 2,           30, halfBW, h);
    highShelfGainSlider .setBounds (bandW * 2 + halfBW,  30, halfBW, h);
}

std::vector<juce::Slider*> SimpleEQAudioProcessorEditor::getSliders()
{
    return {
        &lowShelfFreqSlider,  &lowShelfGainSlider,
        &peakFreqSlider,      &peakGainSlider,     &peakQSlider,
        &highShelfFreqSlider, &highShelfGainSlider
    };
}