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
    const float OUTER_RADIUS = 1.f;
    const float SPLIT_RADIUS = 0.8f;
    const float INNER_RADIUS = 0.5f;

    struct Vertex {
        glm::vec2 position;
        GLfloat opacity;

        Vertex(const glm::vec2 pos) : position(pos), opacity(0.f) {}
    };

    const std::vector<Vertex> getTriangles(){
        std::vector<Vertex> tris;
        // Make the shape once for each key
        for(int i=0; i<12; i++){
            Vertex tris_polar[] = {
                // Outer circle, counter-clockwise
                glm::vec2(OUTER_RADIUS, glm::radians(i*30.f-15.f)),
                glm::vec2(OUTER_RADIUS, glm::radians(i*30.f+15.f)),
                glm::vec2(SPLIT_RADIUS, glm::radians(i*30.f+15.f)),
                glm::vec2(SPLIT_RADIUS, glm::radians(i*30.f+15.f)),
                glm::vec2(SPLIT_RADIUS, glm::radians(i*30.f-15.f)),
                glm::vec2(OUTER_RADIUS, glm::radians(i*30.f-15.f)),
                // Inner circle, counter-clockwise
                glm::vec2(SPLIT_RADIUS, glm::radians(i*30.f-15.f)),
                glm::vec2(SPLIT_RADIUS, glm::radians(i*30.f+15.f)),
                glm::vec2(INNER_RADIUS, glm::radians(i*30.f+15.f)),
                glm::vec2(INNER_RADIUS, glm::radians(i*30.f+15.f)),
                glm::vec2(INNER_RADIUS, glm::radians(i*30.f-15.f)),
                glm::vec2(SPLIT_RADIUS, glm::radians(i*30.f-15.f)),
            };

            for(int j=0; j<12; j++){
                tris.push_back(Vertex(glm::vec2(
                    tris_polar[j].position.x * glm::cos(tris_polar[j].position.y),
                    tris_polar[j].position.x * glm::sin(tris_polar[j].position.y)
                )));
            }
        }
        return tris;
    }

    const std::vector<Vertex> tris = getTriangles();

    MidiViewerAudioProcessor* processor;
    GLuint vertexBuffer;
    std::unique_ptr<OpenGLShaderProgram> midiShader;
};