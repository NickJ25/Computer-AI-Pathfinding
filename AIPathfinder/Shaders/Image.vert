#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 imgModel;
uniform mat4 imgProj;
uniform mat4 imgView;

void main()
{
//(imgProj * imgView * imgRotation) * 
	vec4 newPosition = (imgProj * imgView * imgModel) * vec4(position, 1.0f);
    gl_Position = vec4(newPosition.xyz, 1.0f);
}