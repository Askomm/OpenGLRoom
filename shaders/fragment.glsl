#version 330 core

struct PointLight
{
    vec3 position;
    vec3 color;
    float intensity;
};

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 emissionColor;
uniform float emissionStrength;

uniform bool useChecker;
uniform float checkerScale;
uniform float shininess;

uniform vec3 ambientLight;
uniform PointLight lights[2];

vec3 GetBaseColor()
{
    if (!useChecker)
        return objectColor;

    float checker = mod(
        floor(FragPos.x * checkerScale) + floor(FragPos.z * checkerScale),
        2.0
    );

    return mix(objectColor * 0.65, objectColor * 1.15, checker);
}

void main()
{
    vec3 baseColor = GetBaseColor();
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = ambientLight * baseColor;

    for (int i = 0; i < 2; ++i)
    {
        if (lights[i].intensity <= 0.0)
            continue;

        vec3 lightDir = normalize(lights[i].position - FragPos);
        float distanceToLight = length(lights[i].position - FragPos);

        float attenuation =
            1.0 /
            (1.0 + 0.10 * distanceToLight + 0.032 * distanceToLight * distanceToLight);

        float diffuseStrength = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diffuseStrength * baseColor * lights[i].color;

        vec3 halfDir = normalize(lightDir + viewDir);
        float specularStrength = pow(max(dot(normal, halfDir), 0.0), shininess);
        vec3 specular = specularStrength * vec3(0.65) * lights[i].color;

        result += (diffuse + specular) * attenuation * lights[i].intensity;
    }

    result += emissionColor * emissionStrength;

    // Small tone mapping step keeps bright lamp bulbs from clipping too hard.
    result = result / (result + vec3(1.0));

    FragColor = vec4(result, 1.0);
}
