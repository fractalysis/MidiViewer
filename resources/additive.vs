#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in int chord;
layout (location = 2) in int chord_type;

out float opacity;

uniform float midi_velocities[128];

float get_chord_opacity(int ch, bool major) {
   float note_strengths[12] = float[12](0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

   for(int i=0; i<128; i++){
      note_strengths[i%12] = clamp( note_strengths[i%12] + midi_velocities[i], 0, 1 );
   }

   float weights[12] = float[12](
       0.3, // Root
      -0.2, // Minor Second
       0.0, // Major Second
       0.0, // Minor Third
       0.0, // Major Third
       0.0, // Fourth
      -0.3, // Tritone
       0.2, // Fifth
       0.0, // Minor Sixth
       0.0, // Major Sixth
       0.0, // Minor Seventh
       0.0  // Major Seventh
   );

   if( major ){
      weights[3] = -4.0;
      weights[4] = 4.0;
   }
   else{
      weights[3] = 4.0;
      weights[4] = -4.0;
   }

   float sum = 0.0;
   for(int i=0; i<12; i++){
      sum += weights[i] * note_strengths[i];
   }

   return sum;
}

float get_root_opacity(){
   float root = 0;
   for(int i=chord; i<128; i++){
      if( i%12 == chord ){
         root += midi_velocities[i];
      }
   }
   return root;
}

void main(){
   gl_Position = vec4(aPos,0,1);
   opacity = clamp(get_root_opacity(), 0, 1);
}