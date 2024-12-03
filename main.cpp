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
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include "map_object.h"
#include "duck.h"

void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char*);
void make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char, int, int);
GLvoid SpecialKeys(int, int, int);
void Mouse(int, int, int, int);
void TimerFunction(int v);
GLvoid INITBuffer();
GLvoid DuckBuffer();

std::vector<glm::vec3> readOBJ(std::string filename);
GLuint vertexCount[3];

GLint width = 800, height = 600;
GLuint shaderProgramID; 
GLuint vertexShader; // 버텍스 셰이더
GLuint fragmentShader; // 프래그먼트 세이더
GLchar* vertexSource, * fragmentSource; 

int msecs = 30;

//charcter
GLuint VAO[3];
GLuint VBO[3];

GLuint Duck_VAO[4];
GLuint Duck_VBO[4];
GLuint Duck_vertexCount[4];

float cameraX = 0.0f;
float cameraY = 2.0f;
float cameraZ = 5.0f;

float lightX = 0.0f;
float lightZ = 0.0f;
float lightY = 10.0f;


glm::mat4 view = glm::lookAt(
	glm::vec3(cameraX, cameraY, cameraZ), // 카메라 위치
	glm::vec3(0.0f, 0.0f, 0.0f), // 카메라가 바라보는 대상
	glm::vec3(0.0f, 1.0f, 0.0f)  // 카메라의 업 벡터
);

glm::mat4 projection = glm::perspective(
	glm::radians(45.0f), // 시야각
	(float)width / (float)height, // 화면 비율
	0.1f, 100.0f // 근거리 및 원거리 클리핑 
);


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

std::vector<glm::vec3> readOBJ(std::string filename)
{
	std::ifstream in{ filename };
	if (!in) {
		std::cout << filename << " file read failed\n";
		exit(1);
	}

	// c++ stream --> input output을 해주는 흐름?

	srand(static_cast<unsigned int>(time(nullptr)));

	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> color;
	std::vector<glm::vec3> normal;
	std::vector<glm::ivec3> vindex;
	std::vector<glm::ivec3> nindex;
	while (in) {
		std::string line;
		std::getline(in, line);
		std::stringstream ss{ line };
		std::string str;
		ss >> str;
		if (str == "v") {
			glm::vec3 v;
			for (int i = 0; i < 3; ++i) {
				std::string subStr;
				ss >> subStr;
				v[i] = std::stof(subStr);
			}
			vertex.push_back(v);
			color.push_back(glm::vec3(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX)));
		}
		else if (str == "vn") {
			glm::vec3 n;
			for (int i = 0; i < 3; ++i) {
				std::string subStr;
				ss >> subStr;
				n[i] = std::stof(subStr);
			}
			normal.push_back(n);
		}
		else if (str == "f") {
			glm::ivec3 fv;
			glm::ivec3 fn;
			for (int i = 0; i < 3; ++i) {
				std::string substr;
				ss >> substr;
				std::stringstream subss{ substr };
				std::string vIdx;
				std::getline(subss, vIdx, '/');
				fv[i] = std::stoi(vIdx) - 1;
				std::getline(subss, vIdx, '/');
				// 텍스처 건너뛰기
				std::getline(subss, vIdx, '/');
				fn[i] = std::stoi(vIdx) - 1;
			}
			vindex.push_back(fv);
			nindex.push_back(fn);
		}
	}

	std::vector<glm::vec3> data;
	for (int i = 0; i < vindex.size(); ++i) {
		data.push_back(vertex[vindex[i][0]]); // 정점 location 0
		data.push_back(color[vindex[i][0]]);  // 색상 location 1
		data.push_back(normal[nindex[i][0]]); // 노멀 location 2
		data.push_back(vertex[vindex[i][1]]);
		data.push_back(color[vindex[i][1]]);
		data.push_back(normal[nindex[i][1]]);
		data.push_back(vertex[vindex[i][2]]);
		data.push_back(color[vindex[i][2]]);
		data.push_back(normal[nindex[i][2]]);
	}

	std::cout << filename << " File Read, " << data.size() / 3 << " Vertices Exists." << std::endl;
	return data;
}
//
// 메인 함수

GLvoid DuckBuffer()
{
	glGenVertexArrays(4, Duck_VAO); // VAO 생성
	glGenBuffers(4, Duck_VBO);      // VBO 생성

	glBindVertexArray(Duck_VAO[0]);     // VAO 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, Duck_VBO[0]); // VBO 바인딩

	std::vector<glm::vec3>Duck_vertexData = readOBJ("body.obj");
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Duck_vertexData.size() * 3, Duck_vertexData.data(), GL_STATIC_DRAW);

	Duck_vertexCount[0] = Duck_vertexData.size() / 3;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, reinterpret_cast<void*>(sizeof(float) * 6));
	glEnableVertexAttribArray(1);

	glBindVertexArray(Duck_VAO[1]);     // VAO 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, Duck_VBO[1]); // VBO 바인딩

	Duck_vertexData = readOBJ("leg.obj");
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Duck_vertexData.size() * 3, Duck_vertexData.data(), GL_STATIC_DRAW);

	Duck_vertexCount[1] = Duck_vertexData.size() / 3;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, reinterpret_cast<void*>(sizeof(float) * 6));
	glEnableVertexAttribArray(1);

	glBindVertexArray(Duck_VAO[2]);     // VAO 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, Duck_VBO[2]); // VBO 바인딩

	Duck_vertexData = readOBJ("mouth.obj");
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Duck_vertexData.size() * 3, Duck_vertexData.data(), GL_STATIC_DRAW);

	Duck_vertexCount[2] = Duck_vertexData.size() / 3;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, reinterpret_cast<void*>(sizeof(float) * 6));
	glEnableVertexAttribArray(1);

	glBindVertexArray(Duck_VAO[3]);     // VAO 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, Duck_VBO[3]); // VBO 바인딩

	Duck_vertexData = readOBJ("head.obj");
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Duck_vertexData.size() * 3, Duck_vertexData.data(), GL_STATIC_DRAW);

	Duck_vertexCount[3] = Duck_vertexData.size() / 3;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, reinterpret_cast<void*>(sizeof(float) * 6));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

GLvoid INITBuffer()
{
	glGenVertexArrays(2, VAO); // VAO 생성
	glGenBuffers(2, VBO);      // VBO 생성

	glBindVertexArray(VAO[1]);     // VAO 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]); // VBO 바인딩

	std::vector < glm::vec3> vertexData = readOBJ("cube.obj");
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size() * 3, vertexData.data(), GL_STATIC_DRAW);

	vertexCount[1] = vertexData.size() / 3;

	// 버텍스 위치 (location = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, reinterpret_cast<void*>(sizeof(float) * 6));
	glEnableVertexAttribArray(1);


	glBindVertexArray(0);
}

void main(int argc, char** argv)
{
	float width = 800.0f;
	float height = 600.0f;


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("crossy_road");

	glewExperimental = GL_TRUE;
	glewInit();

	make_shaderProgram(); 
	INITBuffer();
	DuckBuffer();

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


GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{

	glEnable(GL_DEPTH_TEST);																				  // 깊이 테스트 활성화

	GLfloat rColor, gColor, bColor;
	//rbb --> float 값으로 변경한 색상값
	//rColor = 0.678;
	//gColor = 0.882;
	//bColor = 0.396;
	rColor = gColor = bColor = 1.0f;
	glClearColor(rColor, gColor, bColor, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);															 // 깊이 테스트 활성화


	view = glm::lookAt(
		glm::vec3(cameraX, cameraY, cameraZ), // 카메라 위치
		glm::vec3(0.0f, 0.0f, 0.0f), // 카메라가 바라보는 대상
		glm::vec3(0.0f, 1.0f, 0.0f)  // 카메라의 업 벡터
	);
	// 셰이더 내 유니폼 변수 위치 찾기
	GLint viewLoc = glGetUniformLocation(shaderProgramID, "view");
	GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
	GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");

	// 조명 관련 uniform 설정
	glm::vec3 lightPos(lightX, lightY, lightZ);
	glm::vec3 viewPos(cameraX, cameraY, cameraZ);
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f); // 흰색 광원
	glm::vec3 objectColor(1.0f, 1.0f, 0.0f); // 물체 색상

	glm::mat4 model = glm::mat4(1.0f);
	glUniform3fv(glGetUniformLocation(shaderProgramID, "lightPos"), 1, &lightPos[0]);
	glUniform3fv(glGetUniformLocation(shaderProgramID, "viewPos"), 1, &viewPos[0]);
	glUniform3fv(glGetUniformLocation(shaderProgramID, "lightColor"), 1, &lightColor[0]);
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);

	// 행렬을 셰이더로 전달
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

	//model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // x축으로 10도 회전
	//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // y축으로 10도 회전

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

	glUseProgram(shaderProgramID);

	//
	//오리
	duck(modelLoc, objectColor, glm::vec3(0, 0, 0));

	//타일 그리기
	draw_grass(modelLoc, objectColor,glm::vec3(0,0,0));

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void TimerFunction(int v) {

	glutPostRedisplay(); // 화면 재출력
	glutTimerFunc(msecs, TimerFunction, 1); // 타이머 함수 설정
}

