// 필요한 헤더파일 선언
#define _CRT_SECURE_NO_WARNINGS

#include <gl/glew.h>
#include <iostream>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>

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

GLint width = 800, height = 600;
GLuint shaderProgramID; 
GLuint vertexShader; // 버텍스 셰이더
GLuint fragmentShader; // 프래그먼트 세이더
GLchar* vertexSource, * fragmentSource; 

int msecs = 30;

glm::mat4 view = glm::lookAt(
	glm::vec3(0.0f, 0.0f, 3.0f), // 카메라 위치
	glm::vec3(0.0f, 0.0f, 0.0f), // 카메라가 바라보는 대상
	glm::vec3(0.0f, 1.0f, 0.0f)  // 카메라의 업 벡터
);

glm::mat4 projection = glm::perspective(
	glm::radians(45.0f), // 시야각
	(float)width / (float)height, // 화면 비율
	0.1f, 100.0f // 근거리 및 원거리 클리핑 
);

//
// 메인 함수
void main(int argc, char** argv)
{
	float width = 800.0f;
	float height = 600.0f;


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	winID = glutCreateWindow("Example1");

	glewExperimental = GL_TRUE;
	glewInit();

	make_shaderProgram(); 
	InitBuffer();

	glutDisplayFunc(drawScene); 
	glutKeyboardFunc(Keyboard); 
	glutSpecialFunc(SpecialKeys); 
	glutMouseFunc(Mouse); 
	glutReshapeFunc(Reshape);
	glutTimerFunc(msecs, TimerFunction, 1); // 타이머 함수 설정
	glutMainLoop();
}

// 키보드
GLvoid Keyboard(unsigned char key, int x, int y) {
	if (key != 'q') {
		glutPostRedisplay(); // 화면 재출력
	}
	std::cout << key << '\n';
}

GLvoid SpecialKeys(int key, int x, int y) {
	glutPostRedisplay(); 
}

void Mouse(int button, int state, int x, int y) {
	GLfloat newX = (2.0f * x) / width - 1.0f;
	GLfloat newY = 1.0f - (2.0f * y) / height;


	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { // 마우스 좌클릭

	}

	glutPostRedisplay(); // 화면 재출력
}

void make_vertexShaders()
{

	vertexSource = filetobuf("vertexShader.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER); 
	glShaderSource(vertexShader, 1, (const char**)&vertexSource, 0); 
	glCompileShader(vertexShader); 
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR : vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}


void make_fragmentShaders()
{
	fragmentSource = filetobuf("fragmentShader.glsl"); // 프래그 세이더 읽어오기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR : frag_shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

// 세이더 프로그램 만들고 세이더 객체 링크하기
void make_shaderProgram() {

	make_vertexShaders(); // 버텍스 세이더 만들기
	make_fragmentShaders(); // 프래그먼트 세이더 만들기

	shaderProgramID = glCreateProgram(); // 세이더 프로그램 만들기

	glAttachShader(shaderProgramID, vertexShader); // 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(shaderProgramID, fragmentShader); // 세이더 프로그램에 프래그먼트 세이더 붙이기
	glLinkProgram(shaderProgramID); // 세이더 프로그램 링크하기

	glDeleteShader(vertexShader);  // 세이더 삭제
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgramID); // shader program 사용
}

GLvoid drawScene() {
	glEnable(GL_DEPTH_TEST); // 은면제거

	GLfloat rColor, gColor, bColor;
	rColor = gColor = bColor = 0.0f;
	glClearColor(rColor, gColor, bColor, 1.0f); // 배경색 검정
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 렌더링 파이프라인에서 세이더 불러오기
	glUseProgram(shaderProgramID);

	// 셰이더 내 유니폼 변수 위치 찾기
	GLint viewLoc = glGetUniformLocation(shaderProgramID, "view");
	GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
	GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
	GLint isSolidColorLoc = glGetUniformLocation(shaderProgramID, "isSolidColor");
	GLint solidColorLoc = glGetUniformLocation(shaderProgramID, "solidColor");

	// 행렬을 셰이더로 전달
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 model = glm::mat4(1.0f);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

	glutSwapBuffers(); // 화면에 출력
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

	glutPostRedisplay(); // 화면 재출력
	glutTimerFunc(msecs, TimerFunction, 1); // 타이머 함수 설정
}

