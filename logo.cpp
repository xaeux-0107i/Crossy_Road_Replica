#include "duck.h"

extern GLuint logo_vertexCount[3];
extern GLuint logo_VAO[3];
extern GLuint shaderProgramID;

void logo(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos, float logoAngle) {
	glBindVertexArray(logo_VAO[0]);
	objectColor = glm::vec3(1.0f, 1.0f, 1.0f); // 물체 색상
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	//로고
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.5f, 2.5f, 0));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(logoAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(logo_VAO[0]);
	glDrawArrays(GL_TRIANGLES, 0, logo_vertexCount[0]);

	glBindVertexArray(logo_VAO[1]);
	objectColor = glm::vec3(0.6f, 0.4f, 0.3f); // 물체 색상
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	//판
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.5f, 2.5f, 0));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(logoAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(logo_VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, logo_vertexCount[1]);
}