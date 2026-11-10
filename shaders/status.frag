#version 330 core
in vec2 uv;
out vec4 color;
uniform sampler2D panel;
uniform vec3 lamps;
uniform bool shadow, help;
void main(){
    vec3 rgb = texture(panel,uv).rgb;
    vec2 p = uv*vec2(580,68);
    float xs[4]=float[](16,134,246,394);
    for (int i=0;i<4;++i) {
        if(!help && distance(p,vec2(xs[i],49))<4.0){
            bool activeState = i==3 ? shadow : lamps[min(i,2)]>0.5;
            rgb = activeState ? vec3(.98,.71,.32) : vec3(.33,.38,.37);
        }
    }
    color=vec4(rgb,1);
}
