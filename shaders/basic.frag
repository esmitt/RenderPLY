#version 330

uniform vec4 vec4Color;

layout(location = 0) out vec4 vFragColor;

void main(void)
{
	vFragColor = vec4Color;
}