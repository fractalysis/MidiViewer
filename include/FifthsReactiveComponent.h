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

        Vertex() : position(glm::vec2(0.f)), opacity(0.f) {}
        Vertex(const glm::vec2 pos) : position(pos), opacity(0.f) {}
    };

    struct FifthSection {
        Vertex verts[6]; // 2 triangles

        FifthSection(const GLfloat inner, const GLfloat outer, const GLfloat angle) {
            glm::vec2 polars[6] = {
                glm::vec2(outer, glm::radians(angle-15.f)),
                glm::vec2(outer, glm::radians(angle+15.f)),
                glm::vec2(inner, glm::radians(angle+15.f)),
                glm::vec2(inner, glm::radians(angle+15.f)),
                glm::vec2(inner, glm::radians(angle-15.f)),
                glm::vec2(outer, glm::radians(angle-15.f))
            };

            for(int i=0; i<6; i++) {
                verts[i] = Vertex(glm::vec2(glm::cos(polars[i].y) * polars[i].x, glm::sin(polars[i].y) * polars[i].x));
            }
        }

        void setOpacity(const GLfloat opacity) {
            for(int i=0; i<6; i++) {
                verts[i].opacity = opacity;
            }
        }
    };

    const std::vector<FifthSection> getShape(){
        std::vector<FifthSection> tris;
        // Make the shape once for each key
        for(int i=0; i<12; i++){
            // Outer circle
            tris.push_back(FifthSection(SPLIT_RADIUS, OUTER_RADIUS, i*30.f));
            // Inner circle
            tris.push_back(FifthSection(INNER_RADIUS, SPLIT_RADIUS, i*30.f));
        }
        return tris;
    }

    const size_t MAJOR_NOTE_OFFSETS[12] = {
        // A, Bb, B, C, Db, D, Eb, E, F, Gb, G, Ab
           0, 5, 10, 3, 8,  1, 6, 11, 4, 9,  2, 7
    };
    const size_t MINOR_NOTE_OFFSETS[12] = {
        // A, Bb, B, C, Db, D, Eb, E, F, Gb, G, Ab
           3, 8,  1, 6, 11, 4, 9,  2, 7, 0,  5, 10
    };

    std::vector<FifthSection> tris = getShape();

    void updateTriangles();

    MidiViewerAudioProcessor* processor;
    GLuint vertexBuffer;
    std::unique_ptr<OpenGLShaderProgram> midiShader;
};