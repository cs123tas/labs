#version 400
// TODO: Make a new in vec3 with the same name as the vertex shader's output (Task 13)

out vec3 fragColor;
// begin ta solution
in vec3 outColor;
// end ta solution

void main(){
    fragColor = vec3(1.0);
    // TODO: Set fragColor to the color input variable. (Task 13)
    // begin ta solution
    fragColor = outColor;
    // end ta solution
}
