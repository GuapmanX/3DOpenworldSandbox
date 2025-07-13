#shader vertex
#version 330 core
        
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Texcoord;
layout(location = 3) in vec3 a_Normal;
layout(location = 4) in int a_TextureID;

out vec4 v_Color;
out vec2 v_Texcoord;
flat out int v_TextureID;


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
flat in int v_TextureID;

//uniform sampler2D u_Texture;
uniform sampler2D u_Textures[6];

void main()
{
    //vec4 texColor;

    /*if(v_TextureID == 0)
        texColor = texture(u_Textures[0],v_Texcoord);
    if(v_TextureID == 1)
        texColor = texture(u_Textures[1],v_Texcoord);
    if(v_TextureID == 2)
        texColor = texture(u_Textures[2],v_Texcoord);
    if(v_TextureID == 3)
        texColor = texture(u_Textures[3],v_Texcoord);
    if(v_TextureID == 4)
        texColor = texture(u_Textures[4],v_Texcoord);
    if(v_TextureID == 5)
        texColor = texture(u_Textures[5],v_Texcoord);*/


    vec4 texColor = texture(u_Textures[v_TextureID],v_Texcoord);
    color = texColor;
};