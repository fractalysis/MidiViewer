#pragma once

#include <JuceHeader.h>


// A component made for an OpenGLEditor; it receives a context, and its initialize, close,
//   and render functions are called automatically by the editor
class OpenGLComponent : public Component
{
public:

    void setContext(OpenGLContext* context){
        openGLContext = context;
    }

    virtual bool initialize() =0;
    virtual void close() =0;
    virtual void render() =0;

protected:
    OpenGLContext* openGLContext;
};