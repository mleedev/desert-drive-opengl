#version 330
// A vertex shader for rendering vertices with normal vectors and texture coordinates,
// which creates outputs needed for a Phong reflection fragment shader.
layout (location=0) in vec3 vPosition;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec2 vTexCoord;
layout (location=3) in ivec4 vBoneIDs; // New attribute for bone IDs
layout (location=4) in vec4 vWeights; // New attribute for bone weights

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 bones[100];

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragWorldPos;
out vec3 RelativeCamera;



void main() {
    mat4 boneTransform = bones[max(vBoneIDs.x,0)] * vWeights.x +
    bones[max(vBoneIDs.y,0)] * vWeights.y +
    bones[max(vBoneIDs.z,0)] * vWeights.z +
    bones[max(vBoneIDs.w,0)] * vWeights.w;

    vec4 adjustedPosition = vec4(vPosition,1.0); //vec4(vPosition, 1.0) * boneTransform;
    vec3 normalL = vNormal;//mat3(boneTransform) * vNormal;
    // Transform the position to clip space.
    gl_Position = projection * view * model * adjustedPosition;
    TexCoord = vTexCoord;
    Normal = mat3(transpose(inverse(model))) * normalL; //vNormal

    FragWorldPos = vec3(model * adjustedPosition);//vec4(vPosition, 1.0));
    RelativeCamera = vec3(view * vec4(FragWorldPos,1));
}