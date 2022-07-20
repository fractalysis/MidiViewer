#include "FifthsReactiveComponent.h"

using namespace ::juce::gl;

FifthsReactiveComponent::FifthsReactiveComponent(MidiViewerAudioProcessor* processor) {
    this->processor = processor;
}

bool FifthsReactiveComponent::initialize(){
    bool result = true;

    openGLContext->extensions.glGenBuffers (1, &vertexBuffer);
    openGLContext->extensions.glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);
    std::vector<glm::vec2> tris = getTriangle();
    openGLContext->extensions.glBufferData (GL_ARRAY_BUFFER, tris.size() * (int) sizeof (glm::vec2),
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
    openGLContext->extensions.glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
    openGLContext->extensions.glEnableVertexAttribArray (0);

    glDrawArrays(GL_TRIANGLES, 0, getTriangle().size());

    openGLContext->extensions.glDisableVertexAttribArray (0);
}