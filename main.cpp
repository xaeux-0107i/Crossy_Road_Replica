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
#include "car.h"

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
int isCollide(char key);
int isCollideWithCar(glm::vec3 pos);
void update_wing();

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

GLuint Duck_VAO[6];
GLuint Duck_VBO[6];
GLuint Duck_vertexCount[6];

float cameraX = 0.0f;
float cameraY = 10.0f;
float cameraZ = 5.0f;

float lightX = 0.0f;
float lightZ = 0.0f;
float lightY = 10.0f;

float characterX = 0.0f;
float duckDegree = 180.0;
float duckHeight = 0.26;
bool isAlive = true;

float rotationSpeed = 2.0f;
float wingAngle = 0.0f;

//점프
bool isJumping = false; // 점프 상태
float jumpSpeed = 0.2f; // 점프 속도
float gravity = -0.01f; // 중력

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

// 맵
Line line[16];
int numOfLines = 16;

// 자동차
Car car[100];

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
	glGenVertexArrays(6, Duck_VAO); // VAO 생성
	glGenBuffers(6, Duck_VBO);      // VBO 생성

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

	glBindVertexArray(Duck_VAO[4]);     // VAO 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, Duck_VBO[4]); // VBO 바인딩

	Duck_vertexData = readOBJ("right_wing.obj");
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Duck_vertexData.size() * 3, Duck_vertexData.data(), GL_STATIC_DRAW);

	Duck_vertexCount[4] = Duck_vertexData.size() / 3;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, reinterpret_cast<void*>(sizeof(float) * 6));
	glEnableVertexAttribArray(1);

	glBindVertexArray(Duck_VAO[5]);     // VAO 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, Duck_VBO[5]); // VBO 바인딩

	Duck_vertexData = readOBJ("left_wing.obj");
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Duck_vertexData.size() * 3, Duck_vertexData.data(), GL_STATIC_DRAW);

	Duck_vertexCount[5] = Duck_vertexData.size() / 3;

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

	// 맵 초기화
	init_lines();

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
	switch (key) {
	case 'w':
		if (isCollide(key) && isAlive) {
			for (int i = 0; i < numOfLines; i++) { // 맵 이동
				for (int j = 0; j < 15; j++) {
					line[i].floorPosition[j].z += 1;
				}
				if (line[i].floorPosition[0].z > 5) {
					create_new_line(i);
				}
			}

			for (int i = 0; i < 100; i++) { // 자동차 이동
				if (car[i].active) {
					car[i].pos.z += 1;
				}
			}
		}
		duckDegree = 180.0;
		break;
	case 's':
		if (isCollide(key) && isAlive) {
			for (int i = 0; i < numOfLines; i++) { // 맵 이동
				for (int j = 0; j < 15; j++) {
					line[i].floorPosition[j].z -= 1;
				}
			}

			for (int i = 0; i < 100; i++) { // 자동차 이동
				if (car[i].active) {
					car[i].pos.z -= 1;
				}
			}
		}
		duckDegree = 0.0;
		break;
	case 'd':
		if (isCollide(key) && characterX != -7.0 && isAlive) {
			for (int i = 0; i < numOfLines; i++) { // 맵 이동
				for (int j = 0; j < 15; j++) {
					line[i].floorPosition[j].x -= 1;
				}
			}

			for (int i = 0; i < 100; i++) { // 자동차 이동
				if (car[i].active) {
					car[i].pos.x -= 1;
				}
			}

			characterX -= 1.0;
		}
		duckDegree = 90.0;
		break;
	case 'a':
		if (isCollide(key) && characterX != 7.0 && isAlive) {
			for (int i = 0; i < numOfLines; i++) { // 맵 이동
				for (int j = 0; j < 15; j++) {
					line[i].floorPosition[j].x += 1;
				}
			}

			for (int i = 0; i < 100; i++) { // 자동차 이동
				if (car[i].active) {
					car[i].pos.x += 1;
				}
			}
			characterX += 1.0;
		}
		duckDegree = 270.0;
		break;

	case ' ':  //이건 스페이스바
		//std::cout << "Spacebar" << std::endl;
		if (!isJumping) {
			isJumping = true;
			jumpSpeed = 0.2f; 
		}
		break;

	case 27: // ESC 키
		std::cout << "Escape" << std::endl;
		exit(0); // 프로그램 종료
		break;
	}
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
	glm::vec3 objectColor(1.0f, 1.0f, 1.0f); // 물체 색상

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
	if (isAlive) {
		duck(modelLoc, objectColor, glm::vec3(0, 0, 0), 30.0f, duckDegree, duckHeight);
		wing(modelLoc, objectColor, glm::vec3(0, 0, 0), 30.0f, duckDegree, duckHeight, wingAngle);
	}


	//타일 그리기
	for (int i = 0; i < numOfLines; i++) {
		for (int j = 0; j < 15; j++) {
			switch (line[i].floorType) {
			case 0: // 풀밭
				draw_grass(modelLoc, objectColor, line[i].floorPosition[j]);
				if (line[i].isTree[j] == 1) {
					draw_tree(modelLoc, objectColor, line[i].floorPosition[j]);
				}
				if (line[i].isTree[j] == 2) {
					draw_stone(modelLoc, objectColor, line[i].floorPosition[j]);
				}
				break;
			case 1: // 도로
				draw_road(modelLoc, objectColor, line[i].floorPosition[j]);
				break;
			}
		}
	}

	// 자동차 그리기
	for (int i = 0; i < 100; i++) {
		if (car[i].active) {
			car[i].drawCar(modelLoc, objectColor);
		}
	}

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void TimerFunction(int v) {
	for (int i = 0; i < 16; i++) {
		if(line[i].floorType == 1) line[i].update_line();
	}

	for (int i = 0; i < 100; i++) {
		if (car[i].active) {
			car[i].updateCar();
			if (isCollideWithCar(car[i].pos)) isAlive = false;
		}
	}

	if (isJumping) {
		duckHeight += jumpSpeed;   
		jumpSpeed += gravity;       

		// 바닥에 도달
		if (duckHeight <= 0.26f) {
			duckHeight = 0.26f;     
			isJumping = false;      
			jumpSpeed = 0.0f;      
		}
	}

	update_wing();
	glutPostRedisplay(); // 화면 재출력
	glutTimerFunc(msecs, TimerFunction, 1); // 타이머 함수 설정
}

int isCollide(char key) {
	glm::vec3 characterCoord;
	switch (key) {
	case 'w':
		characterCoord = glm::vec3(0.0, 0.0, -1.0);
		break;
	case 'a':
		characterCoord = glm::vec3(-1.0, 0.0, 0.0);
		break;
	case 'd':
		characterCoord = glm::vec3(1.0, 0.0, 0.0);
		break;
	case 's':
		characterCoord = glm::vec3(0.0, 0.0, 1.0);
		break;


	}


	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 15; j++) {
			if ((line[i].floorPosition[j] == characterCoord) && line[i].isTree[j] != 0 && line[i].floorType == 0) {
				return false;
			}
		}
	}

	return true;
}

int isCollideWithCar(glm::vec3 pos) {
	// 자동차 x 너비 - 0.75, z 너비 - 0.4
	// 캐릭터 x, z 너비 - 0.6
	if (pos.x - 0.375 > 0 + 0.3) return false; // A의 왼쪽이 B의 오른쪽을 넘음
	if (pos.x + 0.375 < 0 - 0.3) return false; // A의 오른쪽이 B의 왼쪽을 넘음
	if (pos.z - 0.2 > 0 + 0.3) return false; // A의 아래쪽이 B의 위쪽을 넘음
	if (pos.z + 0.2 / 2 < 0 - 0.3) return false; // A의 위쪽이 B의 아래쪽을 넘음
	return true; // 충돌
}

bool isIncreasing = true; // 증가 중인지 여부를 추적하는 플래그

void update_wing() {
	if (isIncreasing) {
		wingAngle += rotationSpeed;
		if (wingAngle >= 45.0f) {
			wingAngle = 45.0f; // 최대 각도를 제한
			isIncreasing = false; // 감소로 전환
		}
	}
	else {
		wingAngle -= rotationSpeed;
		if (wingAngle <= 0.0f) {
			wingAngle = 0.0f; // 최소 각도를 제한
			isIncreasing = true; // 증가로 전환
		}
	}
}
