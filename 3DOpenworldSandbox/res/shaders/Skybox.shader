#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 2) in vec2 a_Texcoord;

out vec2 v_Texcoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	v_Texcoord = a_Texcoord;
	gl_Position = Projection * View * Model * vec4(a_Position,1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_Texcoord;

uniform sampler2D u_Texture;

void main()
{
	vec4 tex_color = texture(u_Texture, v_Texcoord);
	color = tex_color;
}