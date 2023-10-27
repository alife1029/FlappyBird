#version 460 core

out vec4 FragColor;

in vec2 v_TexCoords;
in vec4 v_Color;

uniform sampler2D u_Sampler;

void main()
{
    vec4 sampled = texture(u_Sampler, v_TexCoords);
    FragColor = sampled * v_Color;
}
