#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aColor;




uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 wNormal;
out vec3 FragPos;

out vec2 TexCoord;
out vec4 Color;


void main()
{
	wNormal =  mat3(transpose(inverse(model))) * aNormal;;
	vec4 pos = model * vec4(aPos,1.0);
	
	FragPos = pos.xyz / pos.w;
	TexCoord = aTexCoord;
	Color = aColor;
	gl_Position = projection * view * vec4(FragPos, 1.0);
}