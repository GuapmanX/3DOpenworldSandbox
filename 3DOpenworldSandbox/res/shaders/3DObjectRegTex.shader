#shader vertex
#version 330 core
        
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Texcoord;
layout(location = 3) in vec3 a_Normal;
layout(location = 4) in float a_TextureID;

out vec4 v_Color;
out vec2 v_Texcoord;
out float v_TextureID;


//uniform mat4 u_MVP;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
        
void main()
{
    gl_Position = Projection * View * Model * vec4(a_Position,1.0);
    v_Color = a_Color;
    v_Texcoord = a_Texcoord;
    v_TextureID = a_TextureID;
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;
    
in vec4 v_Color;
in vec2 v_Texcoord;
in float v_TextureID;

//uniform sampler2D u_Texture;
uniform sampler2D u_Textures[6];

void main()
{
    int index = int(v_TextureID);
    vec4 texColor = texture(u_Textures[index],v_Texcoord);
    color = texColor;
};