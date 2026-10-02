#version 330

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec3 vertexColor;
in vec2 textureCoordinate;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragTexCoords;

uniform mat4 mvp;
uniform mat4 modelMatrix;
uniform mat4 matNormal;

void main()
{
    fragPosition = vec3(modelMatrix * vec4(vertexPosition, 1.0f));
    fragNormal = normalize(vec3(matNormal * vec4(vertexNormal, 0.0)));
    fragTexCoords = textureCoordinate;
    
    gl_Position = mvp * vec4(vertexPosition, 1.0f);
}