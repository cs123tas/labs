#version 330 core

layout(location = 0) in vec3 OS_position;
layout(location = 1) in vec3 OS_normal;
layout(location = 2) in vec3 OS_color;
layout(location = 5) in vec2 OS_texture;

//layout(location = 0) in vec3 OS_position;
//layout(location = 5) in vec2 OS_texture;

//out vec2 uv;

uniform mat4 model, view, projection;

out vec3 WS_position; // world-space position
out vec3 WS_normal;   // world-space normal
out vec3 WS_color;
out vec2 WS_texture;
out float isColor;
//struct Light {
//    vec3 direction;
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};

//uniform Light light;
//vec3 calculateLighting(vec3 Normal, vec3 FragPos) {
//    // Ambient lighting
//    vec3 ambient = light.ambient;

//    // Diffuse lighting
//    vec3 norm = normalize(Normal);
//    vec3 lightDir = normalize(-light.direction);
//    float diff = max(dot(lightDir, norm), 0.0);
//    vec3 diffuse = light.diffuse * diff;

//    // Specular lighting
//    float specularStrength = 0.5;
//    vec3 viewDir = normalize(OS_position - FragPos);
//    vec3 reflectDir = reflect(-lightDir, Normal);

//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
//    vec3 specular = light.specular * spec;

//    return (ambient + diffuse + specular);
//}
void main() {
    isColor=1;
    if(OS_position.y<.9&&OS_position.y>.25){
        isColor = 0;
    }
//    if(OS_position.y==.5){
//        isColor = 1;
//    }
    WS_position = (model * vec4(OS_position, 1.0)).xyz;
    WS_normal = (model * vec4(OS_normal, 0.0)).xyz;
    WS_color = OS_color;
    WS_texture = OS_texture;
   // vec3 lighting = calculateLighting(WS_normal, WS_position);
   // Color = aColor * lighting;
    gl_Position = projection * view * vec4(WS_position, 1.0);

}



