#include "ModernLookAndFeel.h"

void ModernLookAndFeel::drawRotarySlider(
    juce::Graphics& g,
    int x,
    int y,
    int width,
    int height,
    float sliderPos,
    float rotaryStartAngle,
    float rotaryEndAngle,
    juce::Slider&)
{
    auto bounds = juce::Rectangle<float>(
        (float)x,
        (float)y,
        (float)width,
        (float)height);

    auto radius = juce::jmin(bounds.getWidth(),
                             bounds.getHeight()) * 0.5f - 8.0f;

    auto centre = bounds.getCentre();

    auto rx = centre.x - radius;
    auto ry = centre.y - radius;
    auto rw = radius * 2.0f;

    // ====================================
    // SHADOW
    // ====================================

    g.setColour(juce::Colours::black.withAlpha(0.20f));

    g.fillEllipse(rx + 4.0f,
                  ry + 5.0f,
                  rw,
                  rw);

    // ====================================
    // KNOB BODY
    // ====================================

    juce::ColourGradient grad(
        juce::Colour(35, 40, 55),
        centre.x,
        centre.y - radius,

        juce::Colour(15, 20, 30),
        centre.x,
        centre.y + radius,

        false);

    g.setGradientFill(grad);

    g.fillEllipse(rx, ry, rw, rw);

    // ====================================
    // OUTER RING
    // ====================================

    g.setColour(juce::Colour(70, 75, 90));

    g.drawEllipse(
        rx,
        ry,
        rw,
        rw,
        2.0f);

    // ====================================
    // VALUE ARC
    // ====================================

    auto angle = rotaryStartAngle +
                 sliderPos *
                 (rotaryEndAngle - rotaryStartAngle);

    juce::Path valueArc;

    valueArc.addCentredArc(
        centre.x,
        centre.y,
        radius - 5.0f,
        radius - 5.0f,
        0.0f,
        rotaryStartAngle,
        angle,
        true);

    g.setColour(
        juce::Colour(0, 210, 255));

    g.strokePath(
        valueArc,
        juce::PathStrokeType(
            6.0f,
            juce::PathStrokeType::curved,
            juce::PathStrokeType::rounded));

    // ====================================
    // INNER GLOW
    // ====================================

    g.setColour(
        juce::Colour(0,210,255)
            .withAlpha(0.10f));

    g.fillEllipse(
        rx + 15.0f,
        ry + 15.0f,
        rw - 30.0f,
        rw - 30.0f);
}