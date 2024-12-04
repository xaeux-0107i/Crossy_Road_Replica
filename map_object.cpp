#include "map_object.h"

extern GLuint vertexCount[2];
extern GLuint VAO[2];
extern GLuint shaderProgramID;

extern Line line[16];
extern int numOfLines;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> uid(0, 1);
std::uniform_int_distribution<> uidT(0, 4);

int treeType[5][9] = {
	{1, 1, 0, 0, 0, 0, 0, 1, 1},
	{1, 1, 0, 0, 0, 1, 0, 1, 1},
	{1, 1, 0, 1, 0, 0, 0, 1, 1},
	{1, 1, 1, 0, 0, 0, 0, 1, 1},
	{1, 1, 0, 0, 0, 0, 1, 1, 1}

};

// Ÿ�� �ϳ� �� �� ���� ���� = 1.0
void draw_grass(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos) {
	objectColor = glm::vec3(0.578, 0.882, 0.396); // ���λ�
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
void draw_road(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos) {
	objectColor = glm::vec3(0.3, 0.3, 0.3); // ȸ�� - ����
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(1.0f, 0.01f, 1.0f));
	model = glm::translate(model, pos);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);

	objectColor = glm::vec3(0.5, 0.5, 0.5); // ���� ȸ�� - �߾Ӽ�
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(0.0, 0.001, 0.0));
	model = glm::scale(model, glm::vec3(0.7f, 0.01f, 0.1f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);
}
void draw_tree(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos) {
	objectColor = glm::vec3(0.45, 0.15, 0.0); // ���� - ���� �ٱ�
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(0.0, 0.25, 0.0));
	model = glm::scale(model, glm::vec3(0.3f, 0.5f, 0.3f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);

	objectColor = glm::vec3(0.578, 0.882, 0.396); // ���λ� - ��
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(0.0, 0.25, 0.0));
	model = glm::translate(model, glm::vec3(0.0, 0.4, 0.0));
	model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);
};
void create_new_line(int i) {
	line[i].floorType = uid(gen); // ���� ����
	int tree = uidT(gen); // ���� ��ġ ����
	for (int k = 0; k < 9; k++) {
		line[i].isTree[k] = treeType[tree][k];
	}
	for (int j = 0; j < 9; j++) { // Ÿ�� ��ġ ����
		line[i].floorPosition[j] = glm::vec3(j - 4.0, 0.0, line[i].floorPosition[j].z - 15);
	}
};

void init_lines() {
	for (int i = 0; i < 16; i++) {

		if (i < 4) {
			line[i].floorType = 0;
			for (int k = 0; k < 9; k++) {
				line[i].isTree[k] = treeType[0][k];
			}
		}
		else {
			line[i].floorType = uid(gen);
			int tree = uidT(gen);
			for (int k = 0; k < 9; k++) {
				line[i].isTree[k] = treeType[tree][k];
			}
		}

		for (int j = 0; j < 9; j++) { // Ÿ�� ��ġ ����
			line[i].floorPosition[j] = glm::vec3(j - 4.0, 0.0, 2-i);
		}
	}
};