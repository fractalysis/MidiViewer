#pragma once

#include <JuceHeader.h>

#include "OpenGLComponent.h"


class OpenGLEditor : public juce::AudioProcessorEditor,
                     public OpenGLRenderer
{
public:

    OpenGLEditor(juce::AudioProcessor* p) : juce::AudioProcessorEditor(p) {
        setOpaque (true);

        if (auto* peer = getPeer())
            peer->setCurrentRenderingEngine (0);

        openGLContext.setRenderer(this);
        openGLContext.attachTo(*this);
        openGLContext.setContinuousRepainting(true);
    }

    ~OpenGLEditor(){
        openGLContext.detach();
    }

    void newOpenGLContextCreated() override{
        const Array<Component*> all_children = getChildren();

        for (Component* child : all_children) {
            if( OpenGLComponent* comp = dynamic_cast<OpenGLComponent*>( child ) ){

                comp->setContext(&openGLContext);
                comp->initialize();

            }
        }
    }

    void openGLContextClosing() override{
        const Array<Component*> all_children = getChildren();

        for (Component* child : all_children) {
            if( OpenGLComponent* comp = dynamic_cast<OpenGLComponent*>( child ) ){

                comp->close();

            }
        }
    }

    void renderOpenGL() override{
        jassert (OpenGLHelpers::isContextActive());

        const Array<Component*> all_children = getChildren(); // Make sure this is ordered please

        for (Component* child : all_children) {
            if( OpenGLComponent* comp = dynamic_cast<OpenGLComponent*>( child ) ){

                comp->render(); // Does this handle z-indices correctly?

            }
        }
    }


protected:
    OpenGLContext openGLContext;

};