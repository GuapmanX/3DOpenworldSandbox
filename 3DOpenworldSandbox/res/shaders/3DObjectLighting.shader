#shader vertex
#version 330 core
        
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Texcoord;
layout(location = 3) in vec3 a_Normal;

out vec4 v_Color;
out vec3 v_3DTexcoord;
out vec3 v_Normal;
out vec3 v_FragPos;

//uniform mat4 u_MVP;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
        
void main()
{
    //gl_Position = u_MVP * vec4(a_Position,1.0);
    gl_Position = Projection * View * Model * vec4(a_Position,1.0);
    v_FragPos = vec3(Model * vec4(a_Position, 1.0));
    v_Normal = mat3(transpose(inverse(Model))) * a_Normal; //a_Normal;
    //v_Normal = a_Normal;
    v_Color = a_Color;
    v_3DTexcoord = a_Position;
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec3 v_3DTexcoord;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform samplerCube u_Texture;
uniform float u_LightDistance;
uniform vec3 u_LightPosition;
uniform vec4 u_LightColor;
uniform vec3 u_CameraPosition;


void main()
{
    vec3 lightColor = vec3(u_LightColor.r,u_LightColor.g,u_LightColor.b);
    vec4 objectColor = texture(u_Texture,normalize(v_3DTexcoord));


    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPosition - v_FragPos);


    float specaularStrength = 0.5;
    vec3 viewDir = normalize(u_CameraPosition - v_FragPos);
    vec3 reflectDir = reflect(-lightDir,v_Normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0),32);
    vec3 specular = specaularStrength * spec * lightColor;


    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    //AMBIENT//
    float ambientStrength = u_LightDistance;
    vec3 ambient = 0.1 * lightColor;
    vec3 result = (ambient + diffuse + specular) * vec3(objectColor.r,objectColor.g,objectColor.b); //vec4(ambient,1.0) * objectColor;
    //vec4 result = vec4(ambient,1.0) * objectColor;
    //////////


    color = vec4(result,1.0);
};