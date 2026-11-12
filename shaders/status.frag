#version 330 core
in vec2 uv;
out vec4 color;
uniform sampler2D panel;
uniform vec3 lamps;
uniform bool shadow, help, sun;
void main(){
    vec3 rgb = texture(panel,uv).rgb;
    vec2 p = uv*vec2(780,88);
    float xs[5]=float[](16,134,246,394,530);
    for (int i=0;i<5;++i) {
        if(!help && distance(p,vec2(xs[i],49))<4.0){
            bool activeState = i==4 ? sun : (i==3 ? shadow : lamps[min(i,2)]>0.5);
            rgb = activeState ? vec3(.98,.71,.32) : vec3(.33,.38,.37);
        }
    }
    color=vec4(rgb,1);
}
