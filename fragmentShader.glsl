#version 330 core

in vec3 out_Color;  // ���ؽ� ���̴����� ���޵� ����
out vec4 FragColor;     // ��� ����

uniform bool isSolidColor;
uniform vec3 solidColor;

void main()
{
    if (isSolidColor) {
        FragColor = vec4(solidColor, 1.0); // ������ ���� ���
    } else {
        FragColor = vec4(out_Color, 1.0); // VBO���� ���޵� ���� ���
    }
}