// 필요한 헤더파일 선언
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

// 필요한 변수 선언
GLint width = 800, height = 600;
GLuint shaderProgramID; // 셰이더 프로그램 이름
GLuint vertexShader; // 버텍스 셰이더 객체
GLuint fragmentShader; // 프래그먼트 세이더
GLchar* vertexSource, * fragmentSource; // 소스코드 저장 변수



int msecs = 30;

glm::mat4 view = glm::lookAt(
	glm::vec3(0.0f, 0.0f, 3.0f), // 카메라 위치
	glm::vec3(0.0f, 0.0f, 0.0f), // 카메라가 바라보는 대상
	glm::vec3(0.0f, 1.0f, 0.0f)  // 카메라의 업 벡터
);

glm::mat4 projection = glm::perspective(
	glm::radians(45.0f), // 시야각
	(float)width / (float)height, // 화면 비율
	0.1f, 100.0f // 근거리 및 원거리 클리핑 평면
);


// 메인 함수
void main(int argc, char** argv)
{
	// OpenGL 좌표계로 변환을 위한 상수
	float width = 800.0f;
	float height = 600.0f;

	// 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	winID = glutCreateWindow("Example1");

	// GLEW 초기화
	glewExperimental = GL_TRUE;
	glewInit();

	// 세이더 읽어와서 세이더 프로그램 만들기
	make_shaderProgram(); // 세이더 프로그램 만들기
	InitBuffer();

	glutDisplayFunc(drawScene); // 출력 콜백 함수
	glutKeyboardFunc(Keyboard); // 키보드 입력
	glutSpecialFunc(SpecialKeys); // 특수 키 콜백 함수 등록
	glutMouseFunc(Mouse); // 마우스 입력
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
	glutPostRedisplay(); // 화면 재출력
}

void Mouse(int button, int state, int x, int y) {
	GLfloat newX = (2.0f * x) / width - 1.0f;
	GLfloat newY = 1.0f - (2.0f * y) / height;


	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { // 마우스 좌클릭

	}

	glutPostRedisplay(); // 화면 재출력
}

// 버텍스 세이더 객체 만들기
void make_vertexShaders()
{
	// 버텍스 세이더 읽어 저장하고 컴파일하기
	// filetobuf - 사용자 정의 함수로 텍스트 파일을 읽어서 문자열에 저장하는 함수

	vertexSource = filetobuf("vertexShader_13.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // 버텍스 세이더 객체 생성
	glShaderSource(vertexShader, 1, (const char**)&vertexSource, 0); // 세이더 코드 객체에 넣기
	glCompileShader(vertexShader); // 버텍스 세이더 컴파일

	// 에러체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR : vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

// 프래그먼트 세이더 객체 만들기
void make_fragmentShaders()
{
	// 프래그먼트 세이더 읽어 저장하고 컴파일하기
	fragmentSource = filetobuf("fragmentShader18.glsl"); // 프래그 세이더 읽어오기
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

	//================================================================
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	// 정육면체 그리기
	drawHexahedron(modelLoc, isSolidColorLoc);
	glDisable(GL_CULL_FACE);
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

