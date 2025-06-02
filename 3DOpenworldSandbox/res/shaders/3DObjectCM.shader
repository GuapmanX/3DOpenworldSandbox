#shader vertex
#version 330 core
        
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Texcoord;

out vec4 v_Color;
out vec3 v_3DTexcoord;

uniform mat4 u_MVP;
        
void main()
{
    gl_Position = u_MVP * vec4(a_Position,1.0);
    v_Color = a_Color;
    v_3DTexcoord = a_Position;
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec3 v_3DTexcoord;

uniform samplerCube u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture,normalize(v_3DTexcoord));
    color = texColor;
};