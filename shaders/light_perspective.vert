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
    mat4 boneTransform = bones[max(vBoneIDs[0],0)] * vWeights[0] +
    bones[max(vBoneIDs[1],0)] * vWeights[1] +
    bones[max(vBoneIDs[2],0)] * vWeights[2] +
    bones[max(vBoneIDs[3],0)] * vWeights[3];

    vec4 adjustedPosition = vec4(vPosition, 1.0);
    vec3 normalL = mat3(inverse(boneTransform)) * vNormal;
    // Transform the position to clip space.
    gl_Position = projection * view * model * inverse(boneTransform) * adjustedPosition;
    TexCoord = vTexCoord;
    Normal = mat3(transpose(inverse(model))) * normalL; //vNormal

    FragWorldPos = vec3(model * adjustedPosition);//vec4(vPosition, 1.0));
    RelativeCamera = vec3(view * vec4(FragWorldPos,1));
}