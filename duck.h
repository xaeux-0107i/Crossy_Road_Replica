#pragma once
#include <gl/glew.h>
#include <iostream>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

void duck(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos);
void leg(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos);