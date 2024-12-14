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
void Motion(int, int);
void TimerFunction(int v);
void TimerFunction2(int v);
GLvoid INITBuffer();
GLvoid DuckBuffer();
GLvoid LogoBuffer();
int isCollide(char key);
int isCollideWithCar(glm::vec3 pos);
int isCollideWithCloud(glm::vec3 pos);
void update_wing();
void fly_wing();
void change_camera_direction(float duckDegree);

std::vector<glm::vec3> readOBJ(std::string filename);

GLint width = 800, height = 600;
GLuint shaderProgramID; 
GLuint vertexShader; // 버텍스 셰이더
GLuint fragmentShader; // 프래그먼트 세이더
GLchar* vertexSource, * fragmentSource; 

int msecs = 30;
int camera_mode = 0;
float sun_angle = 90.0f;
float sun_time = 30.0f;
float light_strength = 1.0f;
float prevMouseX = 0.0;
int characterMode = 0;
int isBlending = 0;

//charcter
GLuint VAO[3];
GLuint VBO[3];
GLuint vertexCount[3];

GLuint Duck_VAO[6];
GLuint Duck_VBO[6];
GLuint Duck_vertexCount[6];

float cameraX = 0.0f;
float cameraY = 10.0f;
float cameraZ = 5.0f;

float camera_lookat_X = 0.0f;
float camera_lookat_y = 0.0f;
float camera_lookat_z = 0.0f;

float lightX = 0.0f;
float lightZ = 0.0f;
float lightY = 10.0f ;

float characterX = 0.0f;
float duckDegree = 180.0;
float cameraDegree = 0;
float duckHeight = 0.26;
float dieduckHeight = 0.05;
float characterScale = 30.0f;
float characterYScale = 30.0f;
float diecharacterYScale = 1.0f;
bool isAlive = true;
bool isDragging = false;

float rotationSpeed = 2.0f;
float wingAngle = 0.0f;

//점프
bool isJumping = false; // 점프 상태
float jumpSpeed = 0.2f; // 점프 속도
float gravity = -0.01f; // 중력
bool jump2 = false; // 구름 위에서 한 번 더 점프

bool startmode = true;
bool rogoRotating = true;
float logoAngle = -30.0f;
float logoSpeed = 1.0f;     


glm::mat4 view = glm::lookAt(
	glm::vec3(cameraX, cameraY, cameraZ), // 카메라 위치
	glm::vec3(camera_lookat_X, camera_lookat_y, camera_lookat_z), // 카메라가 바라보는 대상
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

GLuint logo_VAO[6];
GLuint logo_VBO[6];
GLuint logo_vertexCount[6];

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
	glGenVertexArrays(3, VAO); // VAO 생성
	glGenBuffers(3, VBO);      // VBO 생성

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

	glBindVertexArray(VAO[2]);     // VAO 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]); // VBO 바인딩

	vertexData = readOBJ("sphere.obj");
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size() * 3, vertexData.data(), GL_STATIC_DRAW);

	vertexCount[2] = vertexData.size() / 3;

	// 버텍스 위치 (location = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, reinterpret_cast<void*>(sizeof(float) * 6));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[0]);     // VAO 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // VBO 바인딩

	vertexData = readOBJ("cloud.obj");
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexData.size() * 3, vertexData.data(), GL_STATIC_DRAW);

	vertexCount[0] = vertexData.size() / 3;

	// 버텍스 위치 (location = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, reinterpret_cast<void*>(sizeof(float) * 6));
	glEnableVertexAttribArray(1);


	glBindVertexArray(0);
}

GLvoid LogoBuffer() {
	glGenVertexArrays(3, logo_VAO); // VAO 생성
	glGenBuffers(3, logo_VBO);      // VBO 생성

	glBindVertexArray(logo_VAO[0]);     // VAO 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, logo_VBO[0]); // VBO 바인딩

	std::vector<glm::vec3>logo_vertexData = readOBJ("logo.obj");
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * logo_vertexData.size() * 3, logo_vertexData.data(), GL_STATIC_DRAW);

	logo_vertexCount[0] = logo_vertexData.size() / 3;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, reinterpret_cast<void*>(sizeof(float) * 6));
	glEnableVertexAttribArray(1);

	glBindVertexArray(logo_VAO[1]);     // VAO 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, logo_VBO[1]); // VBO 바인딩

	logo_vertexData = readOBJ("logoboard.obj");
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * logo_vertexData.size() * 3, logo_vertexData.data(), GL_STATIC_DRAW);

	logo_vertexCount[1] = logo_vertexData.size() / 3;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, reinterpret_cast<void*>(sizeof(float) * 6));
	glEnableVertexAttribArray(1);


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
	LogoBuffer();

	glutDisplayFunc(drawScene); 
	glutKeyboardFunc(Keyboard); 
	glutSpecialFunc(SpecialKeys); 
	glutMouseFunc(Mouse); 
	glutMotionFunc(Motion);
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
		duckDegree = cameraDegree = 180.0;
		change_camera_direction(cameraDegree);
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
		duckDegree = cameraDegree = 0.0;
		change_camera_direction(cameraDegree);
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
		duckDegree = cameraDegree = 90.0;
		change_camera_direction(cameraDegree);
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
		duckDegree = cameraDegree = 270.0;
		change_camera_direction(cameraDegree);
		break;

	case ' ':  //이건 스페이스바
	{
		//std::cout << "Spacebar" << std::endl;
		if (!isJumping) {
			isJumping = true;
			jumpSpeed = 0.2f;
		}

		int n = 0;
		for (int i = 0; i < 16; i++) {
			if (line[i].cloudNum != 0) {
				if (isCollideWithCloud(line[i].floorPosition[line[i].cloudNum])) n = 1;
			}
		}

		if (n == 1) {
			isJumping = true;
			jumpSpeed = 0.2f;
			jump2 = true;
		}
	}
		break;

	case 27: // ESC 키
		std::cout << "Escape" << std::endl;
		exit(0); // 프로그램 종료
		break;

	case 'c':
		camera_mode++;
		if (camera_mode % 2 == 1) {
			change_camera_direction(duckDegree);

			cameraY = duckHeight+0.5f;

			camera_lookat_y = duckHeight + 0.5f;

		}
		else {
			cameraX = 0.0f;
			cameraY = 10.0f;
			cameraZ = 5.0f;
		}
		break;

	case '-':
		sun_time = 30.0f;
		break;
	case '=':
		sun_time = 300.0f;
		break;
	
	case 'p':
		sun_time = 0.0f;
		sun_angle = 90.0f;
		break;
	case 'r':
		isAlive = true;
		break;

	case 13: 
		std::cout << "Enter key pressed!" << std::endl;
		startmode = false;
		break;

	}//key 스위치문

	if (key != 'q') {
		glutPostRedisplay(); // 화면 재출력
	}
}

GLvoid SpecialKeys(int key, int x, int y) {
	glutPostRedisplay(); 
}

void Mouse(int button, int state, int x, int y) {
	GLfloat newX = (2.0f * x) / width - 1.0f;
	GLfloat newY = 1.0f - (2.0f * y) / height;


	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { // 마우스 좌클릭
		isDragging = true;
		prevMouseX = x;
		cameraDegree = duckDegree;
	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) { // 마우스 떼기
		isDragging = false;
		camera_lookat_X = sin(glm::radians(duckDegree));
		camera_lookat_z = cos(glm::radians(duckDegree));
	}

	glutPostRedisplay(); // 화면 재출력
}

void Motion(int x, int y) {
	if (isDragging) {
		// 마우스 이동량에 따라 cameraDegree 변경
		int deltaX = x - prevMouseX;
		cameraDegree += deltaX * 0.1f; // 이동량에 비례하여 각도 업데이트 (0.1은 민감도)
		prevMouseX = x; // 현재 마우스 위치를 이전 위치로 갱신
		camera_lookat_X = sin(glm::radians(cameraDegree));
		camera_lookat_z = cos(glm::radians(cameraDegree));
		glutPostRedisplay(); // 화면 재출력
	}
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);															 // 깊이 테스트 활성화

	for (int i = 0; i < 2; i++) {	//뷰포트 포문
		if (camera_mode % 2 == 1) {
			if (i == 0) {
				glViewport(0, 0, 800, 600);
				view = glm::lookAt(
					glm::vec3(cameraX, cameraY, cameraZ), // 카메라 위치
					glm::vec3(camera_lookat_X, camera_lookat_y, camera_lookat_z), // 카메라가 바라보는 대상
					glm::vec3(0.0f, 1.0f, 0.0f)  // 카메라의 업 벡터
				);
			}
			else if (i == 1) {
				glViewport(0, 400, 200, 200);
				view = glm::lookAt(
					glm::vec3(0.0f, 10.0f, 5.0f), // 카메라 위치
					glm::vec3(0.0f, 0.0f, 0.0f), // 카메라가 바라보는 대상
					glm::vec3(0.0f, 1.0f, 0.0f)  // 카메라의 업 벡터
				);

			}
		}
		else {
			glViewport(0, 0, 800, 600);
			view = glm::lookAt(
				glm::vec3(0.0f, 10.0f, 5.0f), // 카메라 위치
				glm::vec3(0.0f, 0.0f, 0.0f), // 카메라가 바라보는 대상
				glm::vec3(0.0f, 1.0f, 0.0f)  // 카메라의 업 벡터
			);
		}

		// 셰이더 내 유니폼 변수 위치 찾기
		GLint viewLoc = glGetUniformLocation(shaderProgramID, "view");
		GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
		GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");

		
		//조명 관련
		lightX = 10.0f * glm::cos(sun_angle);  // 동쪽-서쪽으로 이동
		lightY = 10.0f * glm::sin(sun_angle);  
		lightZ = 0.0f;   

		 //light_strength 설정
		if (sun_angle >= 0.0f && sun_angle <= glm::pi<float>()) {
			light_strength = 1.0f; // 낮: 조명 활성화
		}
		else {
			light_strength = 0.5f; // 밤: 조명 비활성화
		}


		// 조명 관련 uniform 설정
		glm::vec3 lightPos(lightX, lightY, lightZ);
		glm::vec3 viewPos(cameraX, cameraY, cameraZ);
		glm::vec3 lightColor(1.0f, 1.0f, 0.8f); // 흰색 광원
		glm::vec3 objectColor(1.0f, 1.0f, 1.0f); // 물체 색상

		glm::mat4 model = glm::mat4(1.0f);
		glUniform3fv(glGetUniformLocation(shaderProgramID, "lightPos"), 1, &lightPos[0]);
		glUniform3fv(glGetUniformLocation(shaderProgramID, "viewPos"), 1, &viewPos[0]);
		glUniform3fv(glGetUniformLocation(shaderProgramID, "lightColor"), 1, &lightColor[0]);
		glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
		glUniform1f(glGetUniformLocation(shaderProgramID, "light_strength"), light_strength);


		// 행렬을 셰이더로 전달
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

		//model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // x축으로 10도 회전
		//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // y축으로 10도 회전

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

		glUseProgram(shaderProgramID);

		if(startmode) logo(modelLoc, objectColor, glm::vec3(0, 0, 0), logoAngle);

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

		// 아이템 그리기
		for (int i = 0; i < 16; i++) {
			if (line[i].itemType != 0) {
				draw_item(modelLoc, objectColor, line[i].floorPosition[7], line[i].itemType);
			}
		}

		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		if (characterMode == 1) isBlending = 1;
		glUniform1i(glGetUniformLocation(shaderProgramID, "isBlending"), isBlending);
		//오리

		if (isAlive) {
			duck(modelLoc, objectColor, glm::vec3(0, 0, 0), characterScale, duckDegree, duckHeight, characterYScale);
			wing(modelLoc, objectColor, glm::vec3(0, 0, 0), characterScale, duckDegree, duckHeight, wingAngle, characterYScale);
		}
		else {
			duck(modelLoc, objectColor, glm::vec3(0, 0, 0), characterScale, duckDegree, dieduckHeight, diecharacterYScale);
			wing(modelLoc, objectColor, glm::vec3(0, 0, 0), characterScale, duckDegree, dieduckHeight, wingAngle, diecharacterYScale);
		}

		isBlending = 2;
		glUniform1i(glGetUniformLocation(shaderProgramID, "isBlending"), isBlending);
		// 구름 그리기
		for (int i = 0; i < 16; i++) {
			if (line[i].cloudNum != 0) {
				if (isCollideWithCloud(line[i].floorPosition[line[i].cloudNum])) {
					draw_cloud2(modelLoc, objectColor, line[i].floorPosition[line[i].cloudNum]);
				}
				else {
					draw_cloud(modelLoc, objectColor, line[i].floorPosition[line[i].cloudNum]);
				}
			}
		}
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		isBlending = 0;
		glUniform1i(glGetUniformLocation(shaderProgramID, "isBlending"), isBlending);
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
			if (isCollideWithCar(car[i].pos) &&  characterMode == 0) isAlive = false;
			if (isCollideWithCar(car[i].pos) && characterMode == 2) car[i].active = false;
		}
	}

	if (isJumping) {
		int n = 0;
		for (int i = 0; i < 16; i++) {
			if (line[i].cloudNum != 0) {
				if (isCollideWithCloud(line[i].floorPosition[line[i].cloudNum])) n = 1;
			}
		}

		if (jump2) n = 0;
		if (n == 0) {
			duckHeight += jumpSpeed;
			jumpSpeed += gravity;
			fly_wing();

			// 바닥에 도달
			if (duckHeight <= 0.26f) {
				duckHeight = 0.26f;
				isJumping = false;
				jumpSpeed = 0.0f;
				if (jump2) jump2 = false;
			}
		}
	}

	// 태양 각도
	sun_angle += glm::radians(sun_time / 360.0f);
	if (sun_angle >= glm::two_pi<float>()) {
		sun_angle -= glm::two_pi<float>(); 
	}

	//logoAngle = -10.0f * glm::sin(glm::radians(logoSpeed)) - 10.0f; 
	if (logoAngle <= -70.0f || logoAngle >= -30.0f) {
		logoSpeed = -logoSpeed;  // 방향 반전
	}
	logoAngle += logoSpeed;  // 각도 갱신

	update_wing();
	glutPostRedisplay(); // 화면 재출력
	glutTimerFunc(msecs, TimerFunction, 1); // 타이머 함수 설정
}

// 아이템 타이머
void TimerFunction2(int v) {
	characterMode = 0;
	duckHeight = 0.26;
	characterScale = 30.0f;
	std::cout << "item off\n";
};

// 장애물과 충돌 처리
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
			if ((line[i].floorPosition[j] == characterCoord) && duckHeight < 1.0 &&
				line[i].isTree[j] != 0 && line[i].floorType == 0) {
				return false;
			}
		}
		
		if (line[i].floorPosition[7] == characterCoord && line[i].itemType != 0) {
			switch (line[i].itemType) {
			case 1: // 투명화
				characterMode = 1;
				break;
			case 2: // 거대화
				characterMode = 2;
				duckHeight = 0.52;
				characterScale = 60.0f;
				break;
			}
			glutTimerFunc(5000, TimerFunction2, 2); // 타이머 함수 설정
			line[i].itemType = 0;
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
	if (duckHeight > 0.7) return false; // y축 고려
	return true; // 충돌
}

int isCollideWithCloud(glm::vec3 pos) {
	// 자동차 x 너비 - 0.75, z 너비 - 0.4
	// 캐릭터 x, z 너비 - 0.6
	if (pos.x - 0.7 > 0 + 0.3) return false; // A의 왼쪽이 B의 오른쪽을 넘음
	if (pos.x + 0.7 < 0 - 0.3) return false; // A의 오른쪽이 B의 왼쪽을 넘음
	if (pos.z - 0.7 > 0 + 0.3) return false; // A의 아래쪽이 B의 위쪽을 넘음
	if (pos.z + 0.7 / 2 < 0 - 0.3) return false; // A의 위쪽이 B의 아래쪽을 넘음
	if (duckHeight > 1.5 - 0.13 && duckHeight < 1.5 + 0.13) return false; // y축 고려
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

void fly_wing() {
	if (isIncreasing) {
		wingAngle += rotationSpeed*10;
		if (wingAngle >= 45.0f) {
			wingAngle = 45.0f; 
			isIncreasing = false; 
		}
	}
	else {
		wingAngle -= rotationSpeed*10;
		if (wingAngle <= 0.0f) {
			wingAngle = 0.0f; 
			isIncreasing = true; 
		}
	}
}

void change_camera_direction(float duckDegree) {
	camera_lookat_X = sin(glm::radians(duckDegree));
	camera_lookat_z = cos(glm::radians(duckDegree));

	cameraX = 0.5 * sin(glm::radians(duckDegree));
	cameraZ = 0.5 * cos(glm::radians(duckDegree));
}