#version 330 core

in vec3 WS_position; // world-space position
in vec3 WS_normal;   // world-space normal
in vec2 WS_texture;
in vec3 WS_color;
in float isColor;
out vec3 fragColor;
//uniform vec3 color;
uniform sampler2D sampler;
uniform sampler2D othersampler;
void main(){
    vec3 WS_toLight = normalize(vec3(10.0) - WS_position);
    //fragColor = texture(sampler,WS_texture)*vec4(vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight))),1)*vec4(vec3(181/255.0f,101/255.0f,29/255.0f),1.0);
   // fragColor = vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight)));
//    fragColor = vec3(0.0f,1.0f,0.0f);
    //fragColor = texture(sampler,WS_texture)*vec4(vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight))),1)*vec4(1.0f,1.0f,1.0f,0.0f);
    if(isColor==1){
        fragColor=WS_color*vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight)));
    }
    else{
        fragColor = texture(sampler,WS_texture).xyz;
    }

    //fragColor = col*vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight)));
   // fragColor = vec3(colors*max(0.0, dot(normalize(WS_normal), WS_toLight))));
  //  fragColor = WS_color*vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight)));
    // fragColor = texture(sampler,WS_texture)*vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight)));
   //fragColor = vec3(0.0f,1.0f,0.0f)*vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight)));
}

