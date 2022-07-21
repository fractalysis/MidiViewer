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

    const int MAJOR_NOTE_OFFSETS[12] = {
        // A, Bb, B, C, Db, D, Eb, E, F, Gb, G, Ab
           9,  2, 7,  0, 5, 10, 3, 8, 1, 6, 11, 4, 
    };
    const int MINOR_NOTE_OFFSETS[12] = {
        // A, Bb, B, C, Db, D, Eb, E, F, Gb, G, Ab
           6, 11, 4, 9,  2, 7, 0,  5, 10, 3, 8,  1, 
    };

    struct Vertex {
        glm::vec2 position;
        GLbyte chord; // 0 = C, 1 = Db, 2 = E, etc.
        GLbyte chord_type; // 0 = minor, 1 = major

        Vertex() : position(glm::vec2(0.f)), chord(0), chord_type(0) {}
        Vertex(const glm::vec2 pos, int chord, bool major) : position(pos), chord((GLbyte) chord), chord_type((GLbyte) major) {}
        bool isMajor() const { return chord_type == 1; }
    };

    std::vector<Vertex> getShape(){
        std::vector<Vertex> tris;
        // Make the shape once for each key
        for(int i=0; i<12; i++){
            glm::vec2 major[6] = {
                glm::vec2(OUTER_RADIUS, glm::radians(i*30.f-15.f)),
                glm::vec2(OUTER_RADIUS, glm::radians(i*30.f+15.f)),
                glm::vec2(SPLIT_RADIUS, glm::radians(i*30.f+15.f)),
                glm::vec2(SPLIT_RADIUS, glm::radians(i*30.f+15.f)),
                glm::vec2(SPLIT_RADIUS, glm::radians(i*30.f-15.f)),
                glm::vec2(OUTER_RADIUS, glm::radians(i*30.f-15.f))
            };

            for(int j=0; j<6; j++) {
                tris.push_back(Vertex(
                    glm::vec2(glm::cos(major[j].y) * major[j].x, glm::sin(major[j].y) * major[j].x),
                    MAJOR_NOTE_OFFSETS[i], true
                ));
            }

            glm::vec2 minor[6]= {
                glm::vec2(SPLIT_RADIUS, glm::radians(i*30.f-15.f)),
                glm::vec2(SPLIT_RADIUS, glm::radians(i*30.f+15.f)),
                glm::vec2(INNER_RADIUS, glm::radians(i*30.f+15.f)),
                glm::vec2(INNER_RADIUS, glm::radians(i*30.f+15.f)),
                glm::vec2(INNER_RADIUS, glm::radians(i*30.f-15.f)),
                glm::vec2(SPLIT_RADIUS, glm::radians(i*30.f-15.f))
            };

            for(int j=0; j<6; j++) {
                tris.push_back(Vertex(
                    glm::vec2(glm::cos(minor[j].y) * minor[j].x, glm::sin(minor[j].y) * minor[j].x),
                    MINOR_NOTE_OFFSETS[i], false
                ));
            }

        }
        return tris;
    }

    std::vector<Vertex> tris = getShape();

    //void updateTriangles();

    MidiViewerAudioProcessor* processor;
    GLuint vertexBuffer;
    std::unique_ptr<OpenGLShaderProgram> midiShader;
};