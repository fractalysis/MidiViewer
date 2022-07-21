#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in float aOpacity;

out float opacity;

void main(){
   gl_Position = vec4(aPos,0,1);
   opacity = clamp(aOpacity, 0, 1);
}