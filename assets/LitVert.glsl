#version 420

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;
uniform mat4 MVP;
uniform vec3 objectColor;
//uniform vec3 lightColor;
out vec4 color;
out vec2 TexCoord;
//out vec3 lColor;

void main()
{
	gl_Position = MVP * vec4(aPos, 1.0);
	color = vec4(objectColor, 1.0); //* vec4(lightColor,1.0);
	TexCoord = aTexCoord;
	//lColor = lightColor;
}