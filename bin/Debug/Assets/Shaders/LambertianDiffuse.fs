#version 330

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoords;

out vec4 finalColor;

uniform vec4 colDiffuse;
uniform sampler2D texture0;

void main()
{
    vec3 N = normalize(fragNormal);
    vec3 lightDir = normalize(vec3(-1.0f, -1.0f, -1.0f));

    vec4 col = texture(texture0, fragTexCoords);

    float lum = max(dot(-lightDir, N), 0.0);
    float ambient = 0.3f;

    vec3 color = col.rgb * colDiffuse.rgb * (lum + ambient);

    finalColor = vec4(color, 1.0f);
}