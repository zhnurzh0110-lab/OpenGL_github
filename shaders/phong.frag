#version 330 core
#define MAX_LIGHTS 8

// Shading modes (must match ShadeMode in Material.h)
#define MODE_LIT      0
#define MODE_EMISSIVE 1
#define MODE_SCREEN   2
#define MODE_GLOW     3
#define MODE_LASER    4

struct Light {
    int   type;        // 1 = point, 2 = spot
    vec3  pos;
    vec3  dir;         // spot direction (pointing away from the lamp)
    vec3  color;
    float intensity;
    float innerCos;
    float outerCos;
};

in vec3 vWorldPos;
in vec3 vNormal;
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uTex;
uniform int   uMode;
uniform vec3  uTint;
uniform float uSpecular;
uniform float uShininess;
uniform vec3  uEmissive;
uniform float uAlpha;
uniform bool  uWorldUV;
uniform float uUVScale;

uniform Light uLights[MAX_LIGHTS];
uniform int   uNumLights;
uniform vec3  uViewPos;
uniform vec3  uAmbient;
uniform float uAlarm;      // 0..1, drives the red alarm look
uniform float uTime;

const vec3 ALARM_RED = vec3(1.0, 0.04, 0.04);

// Box projection of world position: avoids stretching on scaled cubes.
vec2 projectUV(vec3 p, vec3 n)
{
    vec3 a = abs(n);
    if (a.y >= a.x && a.y >= a.z) return p.xz;
    if (a.x >= a.z)               return p.zy;
    return p.xy;
}

void main()
{
    if (uMode == MODE_EMISSIVE) {
        FragColor = vec4(uEmissive, 1.0);
        return;
    }
    if (uMode == MODE_LASER) {
        float pulse = 0.9 + 0.1 * sin(uTime * 45.0 + vWorldPos.x * 4.0 + vWorldPos.z * 4.0);
        FragColor = vec4(uEmissive * pulse * (1.0 + 0.5 * uAlarm), 1.0);
        return;
    }
    if (uMode == MODE_GLOW) {
        FragColor = vec4(uEmissive * (1.0 + 0.5 * uAlarm), uAlpha);
        return;
    }
    if (uMode == MODE_SCREEN) {
        // CCTV monitor: render-to-texture + procedural scanlines, vignette and grain.
        vec3 c = texture(uTex, vUV).rgb;
        float scan  = 0.86 + 0.14 * sin(vUV.y * 380.0 + uTime * 6.0);
        vec2  q     = vUV - 0.5;
        float vig   = clamp(1.0 - dot(q, q) * 1.8, 0.0, 1.0);
        float grain = fract(sin(dot(vUV * (1.0 + fract(uTime)), vec2(12.9898, 78.233))) * 43758.5453);
        c = c * vec3(0.65, 1.0, 0.72) * scan * vig + (grain - 0.5) * 0.07;
        FragColor = vec4(c, 1.0);
        return;
    }

    // ---- MODE_LIT : per-fragment Blinn-Phong with ambient + diffuse + specular ----
    vec3 N = normalize(vNormal);
    vec3 V = normalize(uViewPos - vWorldPos);

    vec2 uv = uWorldUV ? projectUV(vWorldPos, N) * uUVScale : vUV * uUVScale;
    vec3 albedo = texture(uTex, uv).rgb * uTint;

    vec3 ambientCol = mix(uAmbient, vec3(0.22, 0.0, 0.0), uAlarm);
    vec3 result = ambientCol * albedo;

    for (int i = 0; i < uNumLights; ++i) {
        Light L = uLights[i];
        vec3  toLight = L.pos - vWorldPos;
        float d  = length(toLight);
        vec3  Ld = toLight / d;

        float att = 1.0 / (1.0 + 0.09 * d + 0.032 * d * d);
        if (L.type == 2) {
            float theta = dot(Ld, -L.dir);
            att *= clamp((theta - L.outerCos) / (L.innerCos - L.outerCos), 0.0, 1.0);
        }

        vec3  col  = mix(L.color, ALARM_RED, uAlarm) * L.intensity;
        float diff = max(dot(N, Ld), 0.0);
        vec3  H    = normalize(Ld + V);
        float spec = (diff > 0.0) ? pow(max(dot(N, H), 0.0), uShininess) * uSpecular : 0.0;

        result += (diff * albedo + spec * vec3(1.0)) * col * att;
    }

    result += uEmissive;
    FragColor = vec4(result, 1.0);
}
