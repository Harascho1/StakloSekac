#shader vertex
#version 400 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_Transform;

void main()
{
    gl_Position = u_Transform * vec4(aPos, 1.0f);
}
#shader fragment
#version 400 core
layout (location = 0) out vec4 FragColor;
uniform vec4 u_Color;
void main() {
    FragColor = u_Color;
}