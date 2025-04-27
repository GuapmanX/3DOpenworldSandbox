#shader vertex
#version 330 core
        
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_TexIndex;

out vec2 v_TexCoord;
out vec4 v_CustomColor;
flat out float v_TexIndex;

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
flat in float v_TexIndex;

uniform vec4 u_Color;
uniform sampler2D u_Textures[2];

void main()
{
    int index = int(v_TexIndex);
    //color = texture(u_Textures[index], v_TexCoord);

    if (v_TexIndex == 0) 
	    color = texture(u_Textures[0], v_TexCoord);
    else
	    color = texture(u_Textures[1], v_TexCoord);
};