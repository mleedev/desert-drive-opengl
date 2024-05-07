#version 330
// A fragment shader for rendering fragments in the Phong reflection model.
layout (location=0) out vec4 FragColor;

// Inputs: the texture coordinates, world-space normal, and world-space position
// of this fragment, interpolated between its vertices.
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragWorldPos;

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

vec4 shininess;

vec3 _pl_intensity;
//float _pl_color;
//vec3 _pl_color;

mat4 dirLight = mat4(
0,-1,0,0, //Position (Direction for directional lights);
0.2,0.2,0.2,0, //Color
1,1,10,0, //LightType, Range, Cuttoff Angle (For spotlights)
0,0,0,0 //LookAt (For spotlights)
);

mat4 pointLight = mat4(
-2,1,0,0, //Position
0,0,1,0, //Color
2,5,0,0, //LightType, Range, Cuttoff Angle (For spotlights)
0,0,0,0 //LookAt (For spotlights)
);

mat4 spotLight = mat4(
1,0,5,0, //Position
1,1,1,0, //Color
3,20,5,0, //LightType, Range, Cuttoff Angle (For spotlights)
0,0,1,0 //LookAt (For spotlights)
);
//
mat4 lights[3] = mat4[](dirLight, pointLight, spotLight);


float calculateSpotlightBrightness(vec3 lightPos, vec3 lightLookVec, vec3 fragWorldPos, float cutoffAngle, float falloff) {
    // Calculate the direction from the light to the fragment
    vec3 lightToFrag = normalize(fragWorldPos - lightPos);

    // Calculate the cosine of the angle between the light direction and the direction to the fragment
    float cosAngle = dot(lightLookVec, lightToFrag);

    // Calculate the cosine of the cutoff angle
    float cosCutoff = cos(radians(cutoffAngle));

    // If the fragment is outside the light's cone, return 0
    if (cosAngle < cosCutoff) {
        return 0.0;
    }

    // Calculate the distance from the light to the fragment
    float distance = length(fragWorldPos - lightPos);

    // Calculate the attenuation (i.e., how much the light diminishes with distance)
    float attenuation = 1.0 / (1.0 + falloff * pow(distance, 2));

    // The brightness is the product of the attenuation and the amount of light the fragment receives based on its angle to the light
    float brightness = attenuation * pow(cosAngle - cosCutoff, 2);

    return brightness;
}

//void calculatePointlight(vec3 position, float radius, vec3 color, int lightType, vec3 lightLookVec, float cutoffAngle) {
void calculatePointLight(mat4 light) {
    vec3 position = light[0].xyz;
    vec3 color = light[1].xyz;
    float lightType = light[2].x - 1;
    float radius = light[2].y;
    float cutoffAngle = light[2].z;
    vec3 lightLookVec = light[3].xyz;
    if (lightType == 0) { // Direcitonal light becomes a direction rather than a position
        position = FragWorldPos - position;
    }

    vec3 light_vector = position - FragWorldPos;
    float distance = length(light_vector);

    //Diffuse intensity
    float cosine = dot(newNormal, normalize(light_vector));
    float lamber_factor = max(cosine, 0);
    vec3 diffuseIntensity = vec3(lamber_factor);

    //Specular intensity
    vec3 view_vector = viewPos - FragWorldPos;
    vec3 reflect_vector = reflect(-light_vector, newNormal);
    cosine = dot(normalize(reflect_vector), normalize(view_vector));
    float spec = pow(max(cosine, 0), 3) * shininess.x;

    vec3 light_intensity;
    if (lightType == 0) {
        light_intensity = vec3(1);
    } else if (lightType == 1) {
        light_intensity = vec3(1 - pow(min(distance/radius, 1), 2));
    } else if (lightType == 2) {
        vec3 lightToFrag = normalize(FragWorldPos - position);

        // Calculate the cosine of the angle between the light direction and the direction to the fragment
        float cosAngle = dot(lightLookVec, lightToFrag);

        // Calculate the cosine of the cutoff angle
        float cosCutoff = cos(radians(cutoffAngle));
        light_intensity = vec3(pow(cosAngle - cosCutoff, 2)) * vec3(1 - pow(min(distance/radius, 1), 2));;
        // If the fragment is outside the light's cone, return 0
        if (abs(cosAngle) < cosCutoff) {
            light_intensity = vec3(0);
        }

    }



    _pl_intensity = _pl_intensity + light_intensity * (diffuseIntensity + spec) * color;// + spec_factor);// + specularIntensity); //* color;
}

void main() {
    // TODO: using the lecture notes, compute ambientIntensity, diffuseIntensity, 
    // and specularIntensity.
    vec3 sampledNormal = texture(normalMap, TexCoord).rgb;

    // Convert the sampled color from [0, 1] to [-1, 1].
    sampledNormal = sampledNormal * 2.0 - 1.0;

    // Use the converted color as your normal.
    newNormal = normalize(sampledNormal * 0.5 + Normal * 0.5);

    //Light Test//

    shininess = texture(specMap, TexCoord);
    _pl_intensity = vec3(0);
    for (int i = 0; i < 3; i++) {
        if (lights[i][2][0] == 0) {
            break;
        }
        calculatePointLight(lights[i]);
    }
    //calculatePointLight(dirLight);
    //calculatePointLight(pointLight);
    //calculatePointLight(spotLight);
    //calculatePointlight(vec3(0,-1,0), 5, vec3(0.2,0.2,0.2), 0, vec3(0,0,0), 0);
    //calculatePointlight(vec3(-2,1,0), 4, vec3(0,0,1), 1, vec3(0,0,0), 0);
    //calculatePointlight(vec3(2,1,0), 4, vec3(1,0,0), 1, vec3(0,0,0), 0);
    //calculatePointlight(vec3(1,0,5), 20, vec3(1,1,1), 2, normalize(vec3(0,0,1)), 5);
    vec3 lightIntensity = _pl_intensity;//ambientIntensity * 0.001 + diffuseIntensity * 0.001 + specularIntensity * 0.001 + _pl_intensity; //+ _pl_intensity;
    FragColor = vec4(lightIntensity, 1)  * (texture(baseTexture, TexCoord));
}