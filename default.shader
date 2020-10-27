#shader vertex
#version 330 core
layout(location = 0) in vec4 pos;
out vec3 relativePos;
uniform mat4 MVP;

void main()
{
	relativePos = (MVP * pos).xyz + vec3(0.5, 0, 0);
	relativePos.x *= 2.0;
	gl_Position = MVP * pos;
};

#shader fragment
#version 330 core
in vec3 relativePos;
layout(location = 0) out vec4 color;
uniform vec4 u_color;

void main()
{
	color = u_color * pow((1.0 - length(relativePos)), 0.5);
};