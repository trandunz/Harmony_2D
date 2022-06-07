// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : BlinnFong3D.frag
// Description : Generic fragment shader for handling BlinnFong Lighting On Meshes
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Output to C++
layout (location = 0) out vec4 FragColor;

// Input from Previous Shader
in vec2 TexCoords;
in vec3 Position;
in vec3 Normals;

#define MAX_POINT_LIGHTS 4
#define MAX_DIRECTIONAL_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

struct PointLight
{
    vec3 Position;
    vec3 Color;
    float SpecularStrength;

    float AttenuationLinear;
    float AttenuationExponent;
};

struct DirectionalLight
{
    vec3 Direction;
    vec3 Color;
    float SpecularStrength;
};

struct SpotLight
{
    vec3 Position;
    vec3 Color;
    float SpecularStrength;
    vec3 Direction;

    float AttenuationLinear;
    float AttenuationExponent;

    float Cutoff;
    float OuterCutoff;
};

// Outside Variables Passed In As 'Uniforms'
uniform int TextureCount;
uniform sampler2D Texture0;

uniform vec3 CameraPos;
uniform vec3 AmbientColor;
uniform float Shininess;
uniform float AmbientStrength;

uniform int NumberOfPointLights;
uniform int NumberOfDirectionalLights;
uniform int NumberOfSpotLights;

uniform PointLight PointLights[MAX_POINT_LIGHTS];
uniform DirectionalLight DirectionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform SpotLight SpotLights[MAX_SPOT_LIGHTS];

uniform bool bRimLighting;
uniform float RimExponent;
uniform vec3 RimColor;

// Available Helper function Forward Declerations
vec4 ColourFromTextureORWhite(vec2 _texCoords);
vec3 CalculateAmbientLight();

vec3 CalculatePointLight(PointLight _pointLight);
vec3 CalculateDirectionalLight(DirectionalLight _directionalLight);
vec3 CalculateSpotLight(SpotLight _spotLight);
vec3 CalculateRimLight();

vec3 ReverseViewDir;

// Main function that gets called per vertex fragment.
void main()
{
    ReverseViewDir = normalize(CameraPos - Position);

    vec3 combinedLighting = CalculateAmbientLight();
    for (int i = 0; i < MAX_POINT_LIGHTS && i < NumberOfPointLights; i++)
    {
        combinedLighting += CalculatePointLight(PointLights[i]);
    }
    for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS && i < NumberOfDirectionalLights; i++)
    {
        combinedLighting += CalculateDirectionalLight(DirectionalLights[i]);
    }
    for (int i = 0; i < MAX_SPOT_LIGHTS && i < NumberOfSpotLights; i++)
    {
        combinedLighting += CalculateSpotLight(SpotLights[i]);
    }
    if (bRimLighting)
    {
        combinedLighting += CalculateRimLight();
    }

    FragColor = vec4(combinedLighting,1.0f) * ColourFromTextureORWhite(TexCoords);
}

// Checks for number of active textures and returns the colour output accordingly.
// If two textures are passed into the shader, they are mixed with a clamped sin function.
vec4 ColourFromTextureORWhite(vec2 _texCoords)
{
    vec4 outputColour;
    switch(TextureCount)
    {
        case 0:
        {
            outputColour = vec4(1.0f,1.0f,1.0f,1.0f);
            break;
        }
        default:
        {
            outputColour = texture(Texture0,_texCoords);
            break;
        }
    }
    return outputColour;
}

vec3 CalculateAmbientLight()
{
    return AmbientStrength * AmbientColor;
}

vec3 CalculateRimLight()
{
    float rimFactor = 1.0f - dot(Normals, ReverseViewDir);
    rimFactor = smoothstep(0.0f, 1.0f, rimFactor);
    rimFactor = pow(rimFactor, RimExponent);
    return rimFactor * RimColor;
}

vec3 CalculatePointLight(PointLight _pointLight)
{
    vec3 lightDir = normalize(Position - _pointLight.Position);

    float strength = max(dot(Normals, -lightDir), 0.0f);
    vec3 diffuseLight = strength * _pointLight.Color;

    vec3 halfwayVector = normalize(-lightDir + ReverseViewDir);
    float specularReflectivity = pow(max(dot(Normals, halfwayVector), 0.0f), Shininess);
    vec3 specularLight = _pointLight.SpecularStrength * specularReflectivity * _pointLight.Color;

    float distance = length(_pointLight.Position - Position);
    float attenuation = 1 + (_pointLight.AttenuationLinear * distance) + (_pointLight.AttenuationExponent * pow(distance, 2.0f));
    return (diffuseLight + specularLight) / attenuation;
}

vec3 CalculateDirectionalLight(DirectionalLight _directionalLight)
{
    float strength = max(dot(Normals, normalize(_directionalLight.Direction)), 0.0f);
    vec3 diffuseLight = strength * _directionalLight.Color;

    vec3 halfwayVector = normalize(normalize(_directionalLight.Direction) + ReverseViewDir);
    float specularReflectivity = pow(max(dot(Normals, halfwayVector), 0.0f), Shininess);
    vec3 specularLight = _directionalLight.SpecularStrength * specularReflectivity * _directionalLight.Color;

    return diffuseLight + specularLight;
}

vec3 CalculateSpotLight(SpotLight _spotLight)
{
    vec3 lightDir = normalize(Position - _spotLight.Position);

    float strength = max(dot(Normals, -lightDir), 0.0f);
    vec3 diffuseLight = strength * _spotLight.Color;

    vec3 halfwayVector = normalize(-lightDir + ReverseViewDir);
    float specularReflectivity = pow(max(dot(Normals, halfwayVector), 0.0f), Shininess);
    vec3 specularLight = _spotLight.SpecularStrength * specularReflectivity * _spotLight.Color;

    float distance = length(_spotLight.Position - Position);
    float attenuation = 1 + (_spotLight.AttenuationLinear * distance) + (_spotLight.AttenuationExponent * pow(distance, 2.0f)); 

    float theta = dot(lightDir, normalize(_spotLight.Direction));
    float intensity = clamp((theta - _spotLight.OuterCutoff) / (_spotLight.Cutoff - _spotLight.OuterCutoff), 0.0f, 1.0f);
    return ((diffuseLight + specularLight) * intensity) / attenuation;
}