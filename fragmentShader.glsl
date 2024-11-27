#version 330 core

in vec3 out_Color;  // 버텍스 셰이더에서 전달된 색상
out vec4 FragColor;     // 출력 색상

uniform bool isSolidColor;
uniform vec3 solidColor;

void main()
{
    if (isSolidColor) {
        FragColor = vec4(solidColor, 1.0); // 고정된 색상 사용
    } else {
        FragColor = vec4(out_Color, 1.0); // VBO에서 전달된 색상 사용
    }
}