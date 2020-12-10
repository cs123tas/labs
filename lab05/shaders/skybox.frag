#version 330 core

//in vec3 vs_tex_coords;
//in vec4 vs_clip_pos;
//in vec4 vs_prev_clip_pos;

//uniform samplerCube Sampler;

//out vec4 fs_color;
//out vec4 fs_motion_vector;

//void main()
//{
//   fs_color = texture(Sampler, vs_tex_coords);

//   /* Motion vector (0.5 means no motion) */
//   vec3 ndc_pos = (vs_clip_pos / vs_clip_pos.w).xyz;
//   vec3 prev_ndc_pos = (vs_prev_clip_pos / vs_prev_clip_pos.w).xyz;
//   fs_motion_vector = vec4((ndc_pos - prev_ndc_pos).xy + 0.5, 0, 1);
//}
//in vec2 WS_texture;
in vec3 color;

out vec3 fragColor;
//uniform vec3 color;
//uniform sampler2D sampler;
void main(){
//    fragColor = texture(sampler,WS_texture);
    fragColor = vec3(0.0f,1.0f,0.0f);
}
