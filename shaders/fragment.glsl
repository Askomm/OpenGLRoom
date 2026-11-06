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
uniform PointLight lights[3];

uniform sampler2D shadowMap;
uniform mat4 lightVP;
uniform bool shadowsOn;
uniform float exposure;

// Fraction of nine nearest depth samples visible to the pendant.
float visibility(vec3 normal, vec3 lightDir)
{
    vec4 clip = lightVP * vec4(FragPos, 1.0);
    vec3 p = clip.xyz / clip.w * 0.5 + 0.5;
    if (clip.w <= 0.0 || any(lessThan(p, vec3(0))) || any(greaterThan(p, vec3(1)))) return 1.0;
    float bias = max(0.00018, 0.0012 * (1.0 - dot(normal, lightDir)));
    vec2 texel = 1.0 / vec2(textureSize(shadowMap, 0));
    float sum = 0.0;
    for (int y = -1; y <= 1; ++y)
        for (int x = -1; x <= 1; ++x)
            sum += step(p.z - bias, texture(shadowMap, p.xy + vec2(x,y)*texel).r);
    return sum / 9.0;
}

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

    for (int i = 0; i < 3; ++i)
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

        vec3 reflected = reflect(-lightDir, normal);
        float highlight = diffuseStrength > 0.0 ? pow(max(dot(reflected, viewDir), 0.0), shininess) : 0.0;
        vec3 specular = lights[i].color * highlight * (shininess >= 60.0 ? 0.60 : 0.045);
        float weight = 1.0;
        if (i == 2) {
            weight = smoothstep(0.51, 0.72, lightDir.y);
            if (shadowsOn) weight *= visibility(normal, lightDir);
        }
        result += weight * (diffuse + specular) * attenuation * lights[i].intensity;
    }

    result += emissionColor * emissionStrength;

    // Small tone mapping step keeps bright lamp bulbs from clipping too hard.
    result *= exposure;
    result = result / (result + vec3(1.0));

    FragColor = vec4(pow(result, vec3(1.0/2.2)), 1.0);
}
