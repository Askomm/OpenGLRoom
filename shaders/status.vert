#version 330 core
uniform float width, height;
out vec2 uv;
void main(){
    uv = vec2(gl_VertexID % 2, gl_VertexID / 2);
    float scale = min(1.0,(width-12.0)/580.0);
    vec2 pixel = vec2(6.0) + uv * vec2(580.0,68.0)*scale;
    gl_Position = vec4(-1.0+2.0*pixel.x/width,1.0-2.0*pixel.y/height,0,1);
}
