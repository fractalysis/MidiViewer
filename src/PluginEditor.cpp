/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiViewerAudioProcessorEditor::MidiViewerAudioProcessorEditor (MidiViewerAudioProcessor& p)
    : OpenGLEditor (&p), audioProcessor (p), comp(&p)
{
    addAndMakeVisible(comp);

    // Create the svg background
    //std::unique_ptr<XmlElement> svg_root(XmlDocument::parse(BinaryData::fifthstext_svg, BinaryData::fifthstext_svgSize));
    //jassert(svg_root != nullptr);
    //changeColor(svg_root, "#FFFFFF");
    bg = Drawable::createFromImageData(BinaryData::fifthstext_svg, BinaryData::fifthstext_svgSize);
    jassert(bg != nullptr);
    addAndMakeVisible (*bg);

    // Make it resizeable as a square
    getConstrainer()->setFixedAspectRatio(1.0);
    setResizable(true, true);
    setSize (500, 500);
}

MidiViewerAudioProcessorEditor::~MidiViewerAudioProcessorEditor()
{
}

//==============================================================================
void MidiViewerAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::AudioProcessorEditor::paint(g);
    
}

void MidiViewerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto b = bg->getDrawableBounds();

    float bg_margin = 10.0;
    bg->setTransformToFit(Rectangle<float>(bg_margin, bg_margin, (float) getWidth() - bg_margin*2.0f, (float) getHeight() - bg_margin*2.0f), RectanglePlacement::centred);
    bg->setBounds(b.getSmallestIntegerContainer());
}
