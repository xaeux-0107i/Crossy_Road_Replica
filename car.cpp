#include "car.h"

extern GLuint vertexCount[2];
extern GLuint VAO[2];
extern GLuint shaderProgramID;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> urdC(0.0, 1.0f);
std::uniform_real_distribution<> urdS(0.0, 0.05f);

void Car::drawCar(GLint modelLoc, glm::vec3 objectColor) {
 
    // 몸통
    objectColor = color; // 색상 설정
    glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);

};

void Car::updateCar() {

};

void Car::createCar() {
	color = glm::vec3(urdC(gen), urdC(gen), urdC(gen)); // 색상 설정
	pos = glm::vec3(0.0, 0.0, 0.0); // 위치 설정
	speed = urdS(gen);
};