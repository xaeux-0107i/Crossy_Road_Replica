// �ʿ��� ������� ����
#define _CRT_SECURE_NO_WARNINGS
#define PI 3.141592
#define LEN 0.8

#include <gl/glew.h>
#include <iostream>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <math.h>

void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char*);
void make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid InitBuffer();
GLvoid Keyboard(unsigned char, int, int);
GLvoid SpecialKeys(int, int, int);
void Mouse(int, int, int, int);
void TimerFunction(int v);
void drawHexahedron(GLint, GLint);
int winID;

// �ʿ��� ���� ����
GLint width = 800, height = 600;
GLuint shaderProgramID; // ���̴� ���α׷� �̸�
GLuint vertexShader; // ���ؽ� ���̴� ��ü
GLuint fragmentShader; // �����׸�Ʈ ���̴�
GLchar* vertexSource, * fragmentSource; // �ҽ��ڵ� ���� ����



int msecs = 30;

glm::mat4 view = glm::lookAt(
	glm::vec3(0.0f, 0.0f, 3.0f), // ī�޶� ��ġ
	glm::vec3(0.0f, 0.0f, 0.0f), // ī�޶� �ٶ󺸴� ���
	glm::vec3(0.0f, 1.0f, 0.0f)  // ī�޶��� �� ����
);

glm::mat4 projection = glm::perspective(
	glm::radians(45.0f), // �þ߰�
	(float)width / (float)height, // ȭ�� ����
	0.1f, 100.0f // �ٰŸ� �� ���Ÿ� Ŭ���� ���
);


// ���� �Լ�
void main(int argc, char** argv)
{
	// OpenGL ��ǥ��� ��ȯ�� ���� ���
	float width = 800.0f;
	float height = 600.0f;

	// ������ �����ϱ�
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	winID = glutCreateWindow("Example1");

	// GLEW �ʱ�ȭ
	glewExperimental = GL_TRUE;
	glewInit();

	// ���̴� �о�ͼ� ���̴� ���α׷� �����
	make_shaderProgram(); // ���̴� ���α׷� �����
	InitBuffer();

	glutDisplayFunc(drawScene); // ��� �ݹ� �Լ�
	glutKeyboardFunc(Keyboard); // Ű���� �Է�
	glutSpecialFunc(SpecialKeys); // Ư�� Ű �ݹ� �Լ� ���
	glutMouseFunc(Mouse); // ���콺 �Է�
	glutReshapeFunc(Reshape);
	glutTimerFunc(msecs, TimerFunction, 1); // Ÿ�̸� �Լ� ����
	glutMainLoop();
}

// Ű����
GLvoid Keyboard(unsigned char key, int x, int y) {
	if (key != 'q') {
		glutPostRedisplay(); // ȭ�� �����
	}
	std::cout << key << '\n';
}

GLvoid SpecialKeys(int key, int x, int y) {
	glutPostRedisplay(); // ȭ�� �����
}

void Mouse(int button, int state, int x, int y) {
	GLfloat newX = (2.0f * x) / width - 1.0f;
	GLfloat newY = 1.0f - (2.0f * y) / height;


	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { // ���콺 ��Ŭ��

	}

	glutPostRedisplay(); // ȭ�� �����
}

// ���ؽ� ���̴� ��ü �����
void make_vertexShaders()
{
	// ���ؽ� ���̴� �о� �����ϰ� �������ϱ�
	// filetobuf - ����� ���� �Լ��� �ؽ�Ʈ ������ �о ���ڿ��� �����ϴ� �Լ�

	vertexSource = filetobuf("vertexShader_13.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // ���ؽ� ���̴� ��ü ����
	glShaderSource(vertexShader, 1, (const char**)&vertexSource, 0); // ���̴� �ڵ� ��ü�� �ֱ�
	glCompileShader(vertexShader); // ���ؽ� ���̴� ������

	// ����üũ
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR : vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

// �����׸�Ʈ ���̴� ��ü �����
void make_fragmentShaders()
{
	// �����׸�Ʈ ���̴� �о� �����ϰ� �������ϱ�
	fragmentSource = filetobuf("fragmentShader18.glsl"); // ������ ���̴� �о����
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR : frag_shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

// ���̴� ���α׷� ����� ���̴� ��ü ��ũ�ϱ�
void make_shaderProgram() {

	make_vertexShaders(); // ���ؽ� ���̴� �����
	make_fragmentShaders(); // �����׸�Ʈ ���̴� �����

	shaderProgramID = glCreateProgram(); // ���̴� ���α׷� �����

	glAttachShader(shaderProgramID, vertexShader); // ���̴� ���α׷��� ���ؽ� ���̴� ���̱�
	glAttachShader(shaderProgramID, fragmentShader); // ���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�
	glLinkProgram(shaderProgramID); // ���̴� ���α׷� ��ũ�ϱ�

	glDeleteShader(vertexShader);  // ���̴� ����
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgramID); // shader program ���
}

GLvoid drawScene() {
	glEnable(GL_DEPTH_TEST); // ��������

	GLfloat rColor, gColor, bColor;
	rColor = gColor = bColor = 0.0f;
	glClearColor(rColor, gColor, bColor, 1.0f); // ���� ����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ������ ���������ο��� ���̴� �ҷ�����
	glUseProgram(shaderProgramID);

	// ���̴� �� ������ ���� ��ġ ã��
	GLint viewLoc = glGetUniformLocation(shaderProgramID, "view");
	GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
	GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
	GLint isSolidColorLoc = glGetUniformLocation(shaderProgramID, "isSolidColor");
	GLint solidColorLoc = glGetUniformLocation(shaderProgramID, "solidColor");

	// ����� ���̴��� ����
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 model = glm::mat4(1.0f);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

	//================================================================
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	// ������ü �׸���
	drawHexahedron(modelLoc, isSolidColorLoc);
	glDisable(GL_CULL_FACE);
	glutSwapBuffers(); // ȭ�鿡 ���
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

char* filetobuf(const char* file) {
	FILE* fptr;
	long length;
	char* buf;

	fptr = fopen(file, "rb");
	if (!fptr)
		return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;

	return buf;
}

GLvoid InitBuffer() {

}

void TimerFunction(int v) {

	glutPostRedisplay(); // ȭ�� �����
	glutTimerFunc(msecs, TimerFunction, 1); // Ÿ�̸� �Լ� ����
}

