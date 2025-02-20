#shader vertex
#version 340 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

void main() {
   gl_Position = vec4(position);
   v_TexCoord = texCoord;
};
#shader fragment
#version 340 core
layout (location = 0) out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
void main() {
    vec4 texColor = texture(u_Texture, v_TexCoord);
    FragColor = texColor;
};