#include "map_object.h"

extern GLuint vertexCount[2];
extern GLuint VAO[2];
extern GLuint shaderProgramID;

void draw_grass(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos) {
	objectColor = glm::vec3(0.0, 1.0, 0.0); // 물체 색상
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(1.0f, 0.01f, 1.0f));
	model = glm::translate(model, pos);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);
}