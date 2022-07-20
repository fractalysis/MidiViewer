#include "FifthsReactiveComponent.h"

using namespace ::juce::gl;

FifthsReactiveComponent::FifthsReactiveComponent(MidiViewerAudioProcessor* processor) {
    this->processor = processor;
}

bool FifthsReactiveComponent::initialize(){
    bool result = true;

    openGLContext->extensions.glGenBuffers (1, &vertexBuffer);
    openGLContext->extensions.glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);
    openGLContext->extensions.glBufferData (GL_ARRAY_BUFFER, tris.size() * (int) sizeof (Vertex),
                                                    tris.data(), GL_STATIC_DRAW);

    midiShader.reset(new OpenGLShaderProgram(*openGLContext));
    String vertexShader = String(BinaryData::additive_vs, BinaryData::additive_vsSize);
    String fragmentShader = String(BinaryData::additive_fs, BinaryData::additive_fsSize);
    result &= midiShader->addVertexShader(vertexShader);
    result &= midiShader->addFragmentShader(fragmentShader);
    result &= midiShader->link();

    if( !result ){
        midiShader.reset();
    }

    return result;
}

void FifthsReactiveComponent::close(){
    openGLContext->extensions.glDeleteBuffers (1, &vertexBuffer);
    midiShader.reset();
}

void FifthsReactiveComponent::render() {
    openGLContext->extensions.glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);

    //VAO stuff (Not sure if we can make our own?)
    // Position
    openGLContext->extensions.glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    openGLContext->extensions.glEnableVertexAttribArray (0);
    // Opacity
    openGLContext->extensions.glVertexAttribPointer (1, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) sizeof(glm::vec2));
    openGLContext->extensions.glEnableVertexAttribArray (1);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw
    midiShader->use();
    glDrawArrays(GL_TRIANGLES, 0, tris.size());

    // Clean up
    openGLContext->extensions.glDisableVertexAttribArray (0);
    openGLContext->extensions.glDisableVertexAttribArray (1);
}