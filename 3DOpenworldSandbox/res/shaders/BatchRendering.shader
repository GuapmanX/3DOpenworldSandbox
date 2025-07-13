#shader vertex
#version 330 core
        
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in int a_TexIndex;

out vec2 v_TexCoord;
out vec4 v_CustomColor;
flat out int v_TexIndex;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
    v_CustomColor = a_Color;
    v_TexIndex = a_TexIndex;
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_CustomColor;
flat in int v_TexIndex;

uniform sampler2D u_Textures[2];

void main()
{
    color = texture(u_Textures[v_TexIndex], v_TexCoord);
};