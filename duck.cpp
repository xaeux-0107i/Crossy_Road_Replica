#include "duck.h"

extern GLuint Duck_vertexCount[6];
extern GLuint Duck_VAO[6];
extern GLuint shaderProgramID;

void duck(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos, float sc, float degree, float height) {
	glBindVertexArray(Duck_VAO[0]);
	objectColor = glm::vec3(1.0f, 1.0f, 1.0f); // 물체 색상
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	//오리
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, height, 0));
	model = glm::rotate(model, glm::radians(degree), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(sc, sc, sc));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(Duck_VAO[0]);
	glDrawArrays(GL_TRIANGLES, 0, Duck_vertexCount[0]);
	//다리
	glBindVertexArray(Duck_VAO[1]);
	objectColor = glm::vec3(1.0f, 0.6f, 0.2f); // 물체 색상
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, height, 0));
	model = glm::rotate(model, glm::radians(degree), glm::vec3(0.0f, 1.0f, 0.0f)); 
	model = glm::scale(model, glm::vec3(sc, sc, sc));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(Duck_VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, Duck_vertexCount[0]);

	//입
	glBindVertexArray(Duck_VAO[2]);
	objectColor = glm::vec3(0.8f, 0.0f, 0.1f); // 물체 색상
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, height, 0));
	model = glm::rotate(model, glm::radians(degree), glm::vec3(0.0f, 1.0f, 0.0f)); 
	model = glm::scale(model, glm::vec3(sc, sc, sc));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(Duck_VAO[2]);
	glDrawArrays(GL_TRIANGLES, 0, Duck_vertexCount[0]);

	//머리
	glBindVertexArray(Duck_VAO[3]);
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, height, 0));
	model = glm::rotate(model, glm::radians(degree), glm::vec3(0.0f, 1.0f, 0.0f)); 
	model = glm::scale(model, glm::vec3(sc, sc, sc));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(Duck_VAO[3]);
	glDrawArrays(GL_TRIANGLES, 0, Duck_vertexCount[0]);
}

void wing(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos, float sc, float degree, float height, float wingAngle) {
	glBindVertexArray(Duck_VAO[4]);
	objectColor = glm::vec3(1.0f, 1.0f, 1.0f); // 물체 색상
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(degree), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0, height, 0));
	model = glm::translate(model, glm::vec3(0.2f, 0, 0));

	model = glm::rotate(model, glm::radians(wingAngle), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(sc, sc, sc));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(Duck_VAO[4]);
	glDrawArrays(GL_TRIANGLES, 0, Duck_vertexCount[4]);


	glBindVertexArray(Duck_VAO[5]);
	objectColor = glm::vec3(1.0f, 1.0f, 1.0f); // 물체 색상
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(degree), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0, height, 0));
	model = glm::translate(model, glm::vec3(-0.2f, 0, 0));

	model = glm::rotate(model, glm::radians(wingAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(sc, sc, sc));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(Duck_VAO[5]);
	glDrawArrays(GL_TRIANGLES, 0, Duck_vertexCount[5]);
}