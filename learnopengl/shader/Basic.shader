#shader vertex
#version 330 core 
layout(location = 0) in vec3 position;
void main() 
{
gl_Position = vec4(position.xyz, 1.0);
};

#shader pixel
#version 330 core 
out vec4 pixelColor;
void main() 
{
	pixelColor = vec4(1.0,0.0,0.0, 1.0);
};