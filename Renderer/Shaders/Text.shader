#shader vertex
#version 400 core
layout (location = 0) in vec2 pos_texCoord;

out vec2 v_TexCoord;

uniform mat4 u_Transform;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_Transform * vec4(pos_texCoord.xy, 0.0, 1.0);
    v_TexCoord = pos_texCoord.xy;
};
#shader fragment
#version 400 core
in vec2 v_TexCoord;
out vec4 color;

uniform sampler2D text;
uniform vec3 u_textColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, v_TexCoord).r);
    color = vec4(u_textColor, 1.0) * sampled;
};