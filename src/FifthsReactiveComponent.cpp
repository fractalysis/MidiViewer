#include "FifthsReactiveComponent.h"

using namespace ::juce::gl;

FifthsReactiveComponent::FifthsReactiveComponent(MidiViewerAudioProcessor* processor) {
    this->processor = processor;
}

bool FifthsReactiveComponent::initialize(){
    bool result = true;

    openGLContext->extensions.glGenBuffers (1, &vertexBuffer);
    //openGLContext->extensions.glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);
    //openGLContext->extensions.glBufferData (GL_ARRAY_BUFFER, tris.size() * (int) sizeof (Vertex),
    //                                                tris.data(), GL_STATIC_DRAW);

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
    // test
    glClear(GL_COLOR_BUFFER_BIT);

    // Update the opacity using midi data rq
    updateTriangles();
    openGLContext->extensions.glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);
    openGLContext->extensions.glBufferData (GL_ARRAY_BUFFER, tris.size() * (int) sizeof (FifthSection),
                                                    tris.data(), GL_STATIC_DRAW);

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
    glDrawArrays(GL_TRIANGLES, 0, tris.size() * 6); // 6 vertices per section

    // Clean up VAO
    openGLContext->extensions.glDisableVertexAttribArray (0);
    openGLContext->extensions.glDisableVertexAttribArray (1);
}

// This function decides what the opacities of the chords are based on the notes being pressed
void FifthsReactiveComponent::updateTriangles(){
    // Clear tris
    for(auto& shape : tris){
        shape.setOpacity(0.f);
    }

    // Some fancy stuff to figure out what chords are being played? idk lol
    struct Chord {
        float root;
        float third;
        float fifth;

        Chord(): root(0.f), third(0.f), fifth(0.f) {}
        float clamp(float val, float min, float max){
            return std::max(min, std::min(max, val));
        }
        float intensity() {
            return 0.3f*clamp(root,0,1) + 0.4f*clamp(third,0,1) + 0.2f*clamp(fifth,0,1);
        }
    } major_positives[12], major_negatives[12], minor_positives[12], minor_negatives[12];

    for(int i=0; i<processor->NUM_MIDI_NOTES; i++){
        float note_velocity = processor->notes[i].load().velocity;
        if(note_velocity > 0.05f){
            int note_index = (i+3)%12; // So A is 0, Bb is 1, etc.

            major_positives[note_index].root += note_velocity;
            minor_positives[note_index].root += note_velocity;
            major_positives[(note_index+8)%12].third += note_velocity;
            minor_positives[(note_index+9)%12].third += note_velocity;
            major_positives[(note_index+5)%12].fifth += note_velocity;
            minor_positives[(note_index+5)%12].fifth += note_velocity;

            major_negatives[(note_index+6)%12].root += note_velocity;
            minor_negatives[(note_index+6)%12].root += note_velocity;
            major_negatives[(note_index+9)%12].third += note_velocity;
            minor_negatives[(note_index+8)%12].third += note_velocity;
            major_negatives[(note_index+11)%12].fifth += note_velocity;
            minor_negatives[(note_index+11)%12].fifth += note_velocity;
        }
    }

    for(int i=0; i<12; i++){
        tris[ 2*MAJOR_NOTE_OFFSETS[i] ].setOpacity( major_positives[i].intensity()-major_negatives[i].intensity() );
        tris[ 2*MINOR_NOTE_OFFSETS[i]+1 ].setOpacity( minor_positives[i].intensity()-minor_negatives[i].intensity() );
    }
}