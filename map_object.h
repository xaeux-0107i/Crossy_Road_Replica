#pragma once
#include <gl/glew.h>
#include <iostream>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <random>

struct Line {
	int floorType; // 0 : Ǯ, 1 : ����
	glm::vec3 floorPosition[9]; // �� ���� �� 9ĭ
	bool isTree[9];
};

void draw_grass(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos);
void draw_road(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos);
void draw_tree(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos);
void create_new_line(int i);
void init_lines();