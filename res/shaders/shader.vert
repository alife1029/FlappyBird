#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoords;
layout (location = 2) in vec4 a_Color;

out vec2 v_TexCoords;
out vec4 v_Color;

uniform mat4 u_ViewProjMatrix;

void main()
{
	v_TexCoords = a_TexCoords;
	v_Color = a_Color;

	gl_Position = u_ViewProjMatrix * vec4(a_Position, 1.0);
}
