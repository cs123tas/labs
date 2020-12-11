#version 400
in vec2 uv;
out vec4 color;

// TODO: Declare a sampler2D uniform (Task 15)
uniform sampler2D sampler;

void main(){
    // TODO: Set the output color to the corresponding texel (Task 15)
    color = texture(sampler,uv);

}
