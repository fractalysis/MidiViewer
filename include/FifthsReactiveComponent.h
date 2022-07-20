#pragma once

#include "OpenGLComponent.h"
#include "PluginProcessor.h"

#define GLM_FORCE_CTOR_INIT
#include "glm/glm.hpp"

#include <vector>

class FifthsReactiveComponent : public OpenGLComponent
{
public:

    FifthsReactiveComponent(MidiViewerAudioProcessor* processor);

    bool initialize() override;
    void close() override;
    void render() override;

private:
    static std::vector<glm::vec2> getTriangle(){
        return std::vector<glm::vec2>({
            {0.f,0.f},
            {1.f,0.f},
            {1.f,1.f}
        });
    }

    MidiViewerAudioProcessor* processor;
    GLuint vertexBuffer;
    std::unique_ptr<OpenGLShaderProgram> midiShader;
};