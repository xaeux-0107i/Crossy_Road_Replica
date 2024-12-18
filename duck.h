#pragma once
#include <gl/glew.h>
#include <iostream>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

void duck(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos, float sc, float degree, float height, float sc2);
void wing(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos, float sc, float degree, float height, float wingAngle, float sc2);
void logo(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos, float logoAngle);