#version 330 core
//layout(binding=0) uniform sampler2D diffuseTex;

//layout (location = 1) uniform sampler2D sampler3;
in vec3 WS_position; // world-space position
in vec3 WS_normal;   // world-space normal
in vec2 WS_texture;
in vec3 WS_color;
in float isColor;
out vec3 fragColor;
//uniform vec3 color;
uniform sampler2D sampler2;
uniform sampler2D sampler3;
//uniform sampler2D othersampler;
void main(){
    vec3 WS_toLight = normalize(vec3(10.0) - WS_position);
    //fragColor = texture(sampler,WS_texture)*vec4(vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight))),1)*vec4(vec3(181/255.0f,101/255.0f,29/255.0f),1.0);
   // fragColor = vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight)));
//    fragColor = vec3(0.0f,1.0f,0.0f);
    //fragColor = texture(sampler,WS_texture)*vec4(vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight))),1)*vec4(1.0f,1.0f,1.0f,0.0f);
    if(isColor==1 ||isColor==2){
        fragColor=WS_color*vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight)));
    }
    else if(isColor==2){
        fragColor=texture(sampler2,WS_texture).xyz;
    }
    else{
        fragColor = texture(sampler2,WS_texture).xyz;
    }

    //fragColor = col*vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight)));
   // fragColor = vec3(colors*max(0.0, dot(normalize(WS_normal), WS_toLight))));
  //  fragColor = WS_color*vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight)));
    // fragColor = texture(sampler,WS_texture)*vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight)));
   //fragColor = vec3(0.0f,1.0f,0.0f)*vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight)));
}

