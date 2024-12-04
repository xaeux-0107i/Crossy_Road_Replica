#include "car.h"
#include "map_object.h"

extern GLuint vertexCount[2];
extern GLuint VAO[2];
extern GLuint shaderProgramID;

extern Line line[16];

void Car::drawCar(GLint modelLoc, glm::vec3 objectColor) {
 
    // 아래 몸통
    objectColor = color; // 색상 설정
    glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(0, 0.125, 0));
	model = glm::scale(model, glm::vec3(0.75f, 0.25f, 0.4f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);

	// 위쪽 몸통
	objectColor = glm::vec3(1.0f, 1.0f, 1.0f); // 흰색 
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	model = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(0, 0.375, 0));
	model = glm::scale(model, glm::vec3(0.5f, 0.25f, 0.4f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);

	// 바퀴
	objectColor = glm::vec3(0.0f, 0.0f, 0.0f); // 검정색
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(0.20, 0.075, 0.2));
	model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);

	objectColor = glm::vec3(1.0f, 1.0f, 1.0f); // 흰색
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(0.20, 0.075, 0.27));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);

	objectColor = glm::vec3(0.0f, 0.0f, 0.0f); // 검정색
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(-0.20, 0.075, 0.2));
	model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);

	objectColor = glm::vec3(1.0f, 1.0f, 1.0f); // 흰색
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(-0.20, 0.075, 0.27));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);

	objectColor = glm::vec3(0.0f, 0.0f, 0.0f); // 검정색
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(0.20, 0.075, -0.2));
	model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);

	objectColor = glm::vec3(1.0f, 1.0f, 1.0f); // 흰색
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(0.20, 0.075, -0.27));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);

	objectColor = glm::vec3(0.0f, 0.0f, 0.0f); // 검정색
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(-0.20, 0.075, -0.2));
	model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);

	objectColor = glm::vec3(1.0f, 1.0f, 1.0f); // 흰색
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(-0.20, 0.075, -0.27));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);

	// 창문
	objectColor = glm::vec3(0.0f, 0.0f, 0.0f); // 검정색
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(0.1, 0.3, 0.18));
	model = glm::scale(model, glm::vec3(0.15f, 0.1f, 0.05f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);

	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(-0.1, 0.3, 0.18));
	model = glm::scale(model, glm::vec3(0.15f, 0.1f, 0.05f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);

	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(0.1, 0.3, -0.18));
	model = glm::scale(model, glm::vec3(0.15f, 0.1f, 0.05f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);

	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(0.26, 0.2, 0));
	model = glm::scale(model, glm::vec3(0.01f, 0.3f, 0.3f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);

	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(-0.26, 0.2, 0));
	model = glm::scale(model, glm::vec3(0.01f, 0.3f, 0.3f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);
};

void Car::updateCar() {
	switch (direction) {
	case 0: // 왼쪽으로 이동
		pos.x += speed;
		if (pos.x >= line[lineNum].floorPosition[8].x) {
			active = false;
		}
		break;
	case 1:
		pos.x -= speed;
		if (pos.x <= line[lineNum].floorPosition[0].x) {
			active = false;
		}
		break;
	}
};

void Car::createCar(float sp, glm::vec3 position, int n, int d) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> urdC(0, 4);
	int c = urdC(gen);

	active = true;
	lineNum = n;
	speed = sp;
	direction = d;
	// 위치 설정
	pos = position;
	if (direction == 1) {
		pos.x += 8;
	}
	switch (c) { // 색상 설정
	case 0:
		color = glm::vec3(1, 0, 0.5);
		break;
	case 1:
		color = glm::vec3(0.6, 0.0, 1.0);
		break;
	case 2:
		color = glm::vec3(0.0, 0.9, 1.0);
		break;
	case 3:
		color = glm::vec3(0.2, 1.0, 0.0);
		break;
	case 4:
		color = glm::vec3(1.0, 0.3, 0.0);
		break;
	}
};
