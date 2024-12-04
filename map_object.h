#pragma once
#include <gl/glew.h>
#include <iostream>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <random>
#include <time.h>

struct Line {
	int floorType; // 0 : Ǯ, 1 : ����
	glm::vec3 floorPosition[15]; // �� ���� �� 15ĭ
	int isTree[15];
	float speed; // �ڵ��� �ӵ�
	int spawnTime; // �ڵ��� ���� �ð�
	std::time_t start_time;
	int lineNum;
	int direction;

	void update_line();
};

void draw_grass(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos);
void draw_road(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos);
void draw_tree(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos);
void draw_stone(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos);
void create_new_line(int i);
void init_lines();