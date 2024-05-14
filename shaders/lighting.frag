#version 330
// A fragment shader for rendering fragments in the Phong reflection model.
layout (location=0) out vec4 FragColor;

// Inputs: the texture coordinates, world-space normal, and world-space position
// of this fragment, interpolated between its vertices.
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragWorldPos;
in vec4 FragPosLightSpace;

// Uniforms: MUST BE PROVIDED BY THE APPLICATION.

// The mesh's base (diffuse) texture.
uniform sampler2D baseTexture;
uniform sampler2D specMap;
uniform sampler2D normalMap;
uniform sampler2D shadowMap;
uniform bool includeLighting;

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
float lamber_factor;

vec3 ambientIntensity;
float diffuseIntensity;
vec3 specularIntensity;

vec3 spec_factor;
vec3 sampledNormal;
vec3 normal;

vec4 shininess;

uniform mat4 lights[10];
vec3 _pl_intensity;
float sceneBrightness = 0.5;

vec3 lPosition;
vec3 lColor;
float lType;
float lRange;
float lCutoffAngle;
vec3 lLookVec;

vec3 light_vector;
vec3 light_direction;
float distance;
float distanceMult;
float dotProduct;
float angleDist;

float spec;

vec3 view_vector;
vec3 reflect_vector;
vec3 light_intensity;

vec3 projCoords;
vec2 shadowMapUV;

float nshadowDepth;

float lineearDepth;
float ndc;

int width = 1200;
int height = 800;

vec3 shadowDepth;

float calculateShadow() {
    vec3 lightCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    if (lightCoords.z <= 1.0f) {
        lightCoords = lightCoords * 0.5 + 0.5;
        float closestDepth = texture(shadowMap, lightCoords.xy).r;
        float currentDepth = lightCoords.z;

        float bias = 0.005f;
        if (currentDepth > closestDepth + bias) {
            return 1.0f;
        }
    }
    return 0.0f;//abs(currentDepth - closestDepth); //0.0f;
}

void calculatePointLight(mat4 light) {
    lPosition = light[0].xyz;
    lColor = light[1].xyz;
    lType = light[2].x - 1;
    lRange = light[2].y;
    lCutoffAngle = light[2].z;
    lLookVec = light[3].xyz;

    if (lType < 0.01) { // Direcitonal light becomes a direction rather than a position
        light_vector = -normalize(lLookVec);
        light_direction = -normalize(lLookVec);
        distance = 0.0f;
    } else {
        light_vector = lPosition - FragWorldPos;
        light_direction = normalize(light_vector);
        distance = length(light_vector);
    }
    distanceMult = 1.0f - pow( min(distance/lRange, 1.0f), 2.0f);
    //Diffuse intensity
    cosine = dot(normal, light_direction);
    angleDist = acos(cosine);
    lamber_factor = 1-pow(clamp(angleDist/radians(90.0f),0,1),4);//min(max(cosine, 0),1);
    //float lamber_factor = min(max(cosine, 0),1);
    diffuseIntensity = lamber_factor;//vec3(lamber_factor);

    //Specular intensity
    view_vector = viewPos - FragWorldPos;
    reflect_vector = reflect(-light_direction, normal);
    cosine = dot(reflect_vector, normalize(view_vector));
    //angleDist = acos(cosine);
    spec = pow(clamp(cosine,0,1), 10) * 2 * distanceMult;// * shininess.x;
    //if (cosine < 0) {
    //    spec = 0.0;
    //}


    //light_intensity;
    if (lType < 0.01) {
        //light_intensity = vec3(1);
        light_intensity = vec3(1-calculateShadow());
    } else if (lType < 1.01) {
        light_intensity = vec3(distanceMult);
    } else if (lType < 2.01) {
        dotProduct = dot(-light_direction, lLookVec);
        angleDist = acos(dotProduct);
    if (angleDist > radians(lCutoffAngle)) {
        light_intensity = vec3(0);
        spec = 0.0f;
    } else {
        //diffuseIntensity = diffuseIntensity * (1-distanceMult);
        diffuseIntensity = 1.0f;
        cosine = pow(1-clamp(angleDist/radians(lCutoffAngle),0,1),2);
        spec = spec * cosine;
        light_intensity = vec3(cosine * distanceMult);//vec3(clamp(pow(cosAngle - cosCutoff, 2),0,1)) *
        // If the fragment is outside the light's cone, return 0
    }

}

//light_intensity = vec3(1.0f);

_pl_intensity = _pl_intensity + light_intensity * (diffuseIntensity + spec) * lColor; //* lColor;// + spec_factor);// + specularIntensity); //* color;
}

void main() {
    _pl_intensity = vec3(0.0);//Total brightness / color of the fragment

    // Use the converted color as your normal.
    //newNormal = Normal;//normalize(sampledNormal * 0.5 + Normal * 0.5);

    //Light Test//
    if (includeLighting) {
        normal = normalize(Normal);
        shininess = texture(specMap, TexCoord);
        for (int i = 0; i < 10; i++) {
            if (lights[i][2][0] == 0) {
                break;
            }
            calculatePointLight(lights[i]);
        }
        FragColor = vec4(_pl_intensity * sceneBrightness, 1)  * (texture(baseTexture, TexCoord));

        //shadowMapUV = (gl_FragCoord.xy) / vec2(width, height);
        //FragColor = texture(shadowMap,shadowMapUV)/4.0;
    }
    else {
        //float ndc = gl_FragCoord.z * 2.0 - 1.0;
        //float linearDepth = (2.0 * 0.1 * 100.0) / (100.0 + 0.1 - ndc * (100.0 - 0.1))/100.0;
        FragColor = vec4(vec3(1), 1.0);
    }
    //FragColor = vec4(1,1,1,1);
}