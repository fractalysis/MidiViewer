/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"
#include "OpenGLEditor.h"

#include "TriangleTestComponent.h"

//==============================================================================
/**
*/
class MidiViewerAudioProcessorEditor  : public OpenGLEditor
{
public:
    MidiViewerAudioProcessorEditor (MidiViewerAudioProcessor&);
    ~MidiViewerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MidiViewerAudioProcessor& audioProcessor;

    std::unique_ptr<Drawable> bg;
    TriangleTestComponent comp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiViewerAudioProcessorEditor)
};
