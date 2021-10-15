#version 330 core

layout(location = 0) in vec3 ObjectSpace_position; // object-space vertex position
layout(location = 1) in vec3 ObjectSpace_normal;   // object-space vertex normal

uniform mat4 model, view, projection;

out vec3 CameraSpace_position; // eye-space position
out vec3 CameraSpace_normal;   // eye-space normal

void main() {
    CameraSpace_position = (view * model * vec4(ObjectSpace_position, 1.0)).xyz;
    CameraSpace_normal = (view * model * vec4(ObjectSpace_normal, 0.0)).xyz;

    gl_Position = projection * view * model * vec4(ObjectSpace_position, 1.0);
}
