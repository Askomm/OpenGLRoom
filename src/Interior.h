#pragma once
// Called in both passes so the same geometry casts and receives pendant shadows.
static void DrawInterior(Mesh& box, Mesh& round, Mesh& ball, Shader& shader)
{
    using V = glm::vec3;
    const V oak(.39f,.21f,.095f), ink(.033f,.065f,.065f), linen(.12f,.38f,.34f);
    auto block = [&](V pos, V size, V color, int pattern=0, float gloss=22.f) {
        shader.SetInt("pattern", pattern);
        DrawObject(box, shader, Transform(pos,size),color,false,V(0),0,gloss);
    };
    block({0,-.08,0},{8,.16,8},oak,1);
    block({0,2,-4},{8,4,.14},{.48,.46,.36});
    block({-4,2,0},{.14,4,8},{.26,.37,.34});
    block({4,2,0},{.14,4,8},{.26,.37,.34});
    block({0,2,4},{8,4,.14},{.41,.43,.36});
    block({0,4,0},{8,.14,8},{.62,.57,.46});
    // Window recess, painted landscape and timber mullions.
    block({.4,2.48,-3.86},{4.62,1.91,.12},ink);
    block({.4,2.48,-3.78},{4.45,1.76,.035},{1,1,1},3);
    for (float x : {-1.87f,-.36f,1.16f,2.68f})
        block({x,2.48,-3.72},{.065,1.88,.09},oak);
    block({.4,1.52,-3.71},{4.72,.11,.26},oak);
    // Slatted wall beside the window and a low credenza.
    for(int i=0;i<9;++i) block({-3.78+i*.19,2,-3.82},{.075,3.83,.12},oak);
    block({-2.84,.77,-3.30},{1.76,1.4,.7},ink);
    block({-2.84,1.50,-3.30},{1.85,.09,.79},oak,1);
    for(int i=0;i<3;++i) block({-3.38+i*.54,.84,-2.93},{.48,1.14,.04},{.26,.32,.25});
    block({0,.018,.60},{4.9,.035,3.25},{.22,.36,.32},2);
    block({0,1.06,-1.45},{4.1,.16,1.75},oak,1);
    for(float x:{-1.82f,1.82f}) for(float z:{-2.12f,-.78f})
        block({x,.50,z},{.12,1,.12},ink,0,72);
    // Upholstered reading chair and timber frame.
    block({-.48,.61,.35},{.93,.16,.90},oak);
    block({-.48,.76,.35},{.88,.18,.83},linen,2);
    block({-.48,1.22,.76},{.93,.74,.11},oak);
    block({-.48,1.23,.68},{.83,.59,.12},linen,2);
    for(float x:{-.84f,-.12f}) for(float z:{.02f,.67f})
        block({x,.29,z},{.09,.58,.09},oak);
    // Books provide small, recognizable occluders on the writing surface.
    for(int i=0;i<3;++i) {
        float y=1.17f+i*.095f;
        block({.35+i*.025,y,-1.1},{.58,.075,.39},{.78,.71,.52});
        block({.35+i*.025,y+.043,-1.1},{.60,.018,.41},i==1?linen:ink);
    }
    // Glazed ceramic vase on the cabinet.
    shader.SetInt("pattern",0);
    DrawObject(ball,shader,Transform({-2.9,1.81,-3.26},{.47,.57,.47}),{.69,.30,.13},false,V(0),0,84);
    DrawObject(round,shader,Transform({-2.9,2.06,-3.26},{.22,.18,.22}),{.69,.30,.13},false,V(0),0,84);
}
