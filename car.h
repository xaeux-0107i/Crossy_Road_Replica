#pragma once
#include <gl/glew.h>
#include <iostream>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <random>

struct Car {
	bool active = false;
	glm::vec3 color;
	glm::vec3 pos;
	float speed;
	int direction;
	int lineNum;

	void updateCar();
	void drawCar(GLint modelLoc, glm::vec3 objectColor);
	void createCar(float sp, glm::vec3 position, int n, int d);
};
