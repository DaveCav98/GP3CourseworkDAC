#version 420

in vec3 color;
in vec2 TexCoord;
out vec4 fragColor;

uniform sampler2D ourTexture;

//uniform vec3 lightColor;

in vec3 lColor;

void main()
{
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lColor;

    vec3 result = ambient * color;
	//fragColor = texture(ourTexture, TexCoord) * vec4(result, 1.0);
	//fragColor = vec4(result, 1.0);
	fragColor = vec4(1,0,0,1);
}
