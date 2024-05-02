#version 330
// A fragment shader for rendering fragments in the Phong reflection model.
layout (location=0) out vec4 FragColor;

// Inputs: the texture coordinates, world-space normal, and world-space position
// of this fragment, interpolated between its vertices.
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragWorldPos;
in vec3 RelativeCamera;

// Uniforms: MUST BE PROVIDED BY THE APPLICATION.

// The mesh's base (diffuse) texture.
uniform sampler2D baseTexture;
uniform sampler2D specMap;
uniform sampler2D normalMap;

// Material parameters for the whole mesh: k_a, k_d, k_s, shininess.
uniform vec4 material;

// Ambient light color.
uniform vec3 ambientColor;

// Direction and color of a single directional light.
uniform vec3 directionalLight; // this is the "I" vector, not the "L" vector.
uniform vec3 directionalColor;

// Location of the camera.
uniform vec3 viewPos;

float cosine;
float lambert_factor;

vec3 ambientIntensity;
vec3 diffuseIntensity;
vec3 specularIntensity;

vec3 spec_factor;
vec3 sampledNormal;
vec3 newNormal;

void main() {
    // TODO: using the lecture notes, compute ambientIntensity, diffuseIntensity, 
    // and specularIntensity.
    vec3 sampledNormal = texture(normalMap, TexCoord).rgb;

    // Convert the sampled color from [0, 1] to [-1, 1].
    sampledNormal = sampledNormal * 2.0 - 1.0;

    // Use the converted color as your normal.
    vec3 newNormal = normalize(sampledNormal * 0.5 + Normal * 0.5);

    float cosine = dot(newNormal, -directionalLight);
    float lambert_factor = max(cosine, 0);

    vec3 ambientIntensity = ambientColor; //* material.x;
    vec3 diffuseIntensity = vec3(lambert_factor); //material.y * vec3(1) *

    vec3 reflect_vector = reflect(directionalLight, normalize(newNormal));
    cosine = dot(normalize(reflect_vector), normalize(-RelativeCamera));
    vec3 spec_factor = vec3(pow(max(cosine, 0), 1));

    vec3 specularIntensity = vec3(vec4(spec_factor,1) * texture(specMap, TexCoord));

    vec3 lightIntensity = ambientIntensity * 1 + diffuseIntensity * 1 + specularIntensity * 1;
    FragColor = vec4(lightIntensity, 1) * texture(baseTexture, TexCoord);
}