#shader vertex
#version 400 core
layout (location = 0) in vec3 aPos;
void main() {
   gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
};
#shader fragment
#version 400 core
layout (location = 0) out vec4 FragColor;
uniform vec4 u_Color;
void main() { 
   FragColor = u_Color;
};