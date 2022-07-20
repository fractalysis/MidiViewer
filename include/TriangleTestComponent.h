#pragma once

#include "OpenGLComponent.h"

#define GLM_FORCE_CTOR_INIT
#include "glm/glm.hpp"

#include <vector>


using namespace ::juce::gl;


class TriangleTestComponent : public OpenGLComponent
{
public:

    bool initialize() override{
        bool result = true;

        openGLContext->extensions.glGenBuffers (1, &vertexBuffer);
        openGLContext->extensions.glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);
        std::vector<glm::vec2> tris = getTriangle();
        openGLContext->extensions.glBufferData (GL_ARRAY_BUFFER, tris.size() * (int) sizeof (glm::vec2),
                                                       tris.data(), GL_STATIC_DRAW);

        shaderTest.reset(new OpenGLShaderProgram(*openGLContext));
        result &= shaderTest->addVertexShader(getVertexShader());
        result &= shaderTest->addFragmentShader(getFragmentShader());
        result &= shaderTest->link();

        if( !result ){
            shaderTest.reset();
        }

        return result;
    }

    void close() override{
        openGLContext->extensions.glDeleteBuffers (1, &vertexBuffer);
        shaderTest.reset();
    }

    void render() override{
        //glClear(GL_COLOR_BUFFER_BIT);

        openGLContext->extensions.glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);

        //VAO stuff (Not sure if we can make our own?)
        openGLContext->extensions.glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof (glm::vec2), nullptr);
        openGLContext->extensions.glEnableVertexAttribArray (0);

        //Bind shader
        shaderTest->use();
        glDrawArrays(GL_TRIANGLES,0,getTriangle().size());

        openGLContext->extensions.glDisableVertexAttribArray (0);
    }

private:
    GLuint vertexBuffer;
    std::unique_ptr<OpenGLShaderProgram> shaderTest;

    static char* getVertexShader(){
        return "#version 330 core\n"
            "layout (location = 0) in vec2 aPos;"
            "void main(){"
            "   gl_Position = vec4(aPos,0,1);"
            "}";
    }

    static char* getFragmentShader(){
        return "#version 330 core\n"
            "void main(){"
            "   gl_FragColor = vec4(1);"
            "}";
    }

    static std::vector<glm::vec2> getTriangle(){
        return std::vector<glm::vec2>({
            {0.f,0.f},
            {1.f,0.f},
            {1.f,1.f}
        });
    }

};