#include "map_object.h"
#include "car.h"

extern GLuint vertexCount[2];
extern GLuint VAO[2];
extern GLuint shaderProgramID;

extern Line line[16];
extern int numOfLines;
extern Car car[100];

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> uid(0, 1);
std::uniform_int_distribution<> uidT(0, 4);
std::uniform_int_distribution<> uidTime(2,4);
std::uniform_real_distribution<> urdS(0.03, 0.06f);
std::uniform_int_distribution<> urdD(0, 1);

int treeType[5][15] = {
	{1, 1, 1, 0, 0, 0, 0, 0, 0, 2, 0, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 2, 0, 0, 0, 1, 0, 0, 1, 1, 1},
	{1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 2, 0, 1, 1, 1},
	{1, 1, 1, 1, 0, 1, 0, 0, 2, 1, 0, 0, 1, 1, 1},
	{1, 1, 1, 0, 2, 1, 0, 0, 1, 0, 0, 2, 1, 1, 1}
};

// 타일 하나 당 한 변의 길이 = 1.0
void draw_grass(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos) {
	objectColor = glm::vec3(0.578, 0.882, 0.396); // 연두색
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
	objectColor = glm::vec3(0.3, 0.3, 0.3); // 회색 - 도로
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(1.0f, 0.01f, 1.0f));
	model = glm::translate(model, pos);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);

	objectColor = glm::vec3(0.5, 0.5, 0.5); // 밝은 회색 - 중앙선
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
	objectColor = glm::vec3(0.45, 0.15, 0.0); // 갈색 - 나무 줄기
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

	objectColor = glm::vec3(0.578, 0.882, 0.396); // 연두색 - 잎
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
void draw_stone(GLint modelLoc, glm::vec3 objectColor, glm::vec3 pos) {
	objectColor = glm::vec3(0.5, 0.5, 0.5); // 회색
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, &objectColor[0]);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	model = glm::translate(model, glm::vec3(0.0, 0.2, 0.0));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(VAO[1]);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	glBindVertexArray(0);
};
void create_new_line(int i) {
	line[i].floorType = uid(gen); // 종류 결정
	int tree = uidT(gen); // 나무 위치 결정
	line[i].speed = urdS(gen); // 자동차 속도 설정
	line[i].spawnTime = uidTime(gen); // 자동차 생성 시간 설정
	for (int k = 0; k < 15; k++) {
		line[i].isTree[k] = treeType[tree][k];
	}
	for (int j = 0; j < 15; j++) { // 타일 위치 지정
		line[i].floorPosition[j] = glm::vec3(j - 7.0, 0.0, line[i].floorPosition[j].z - 16);
	}
	line[i].direction = urdD(gen);
	line[i].start_time = std::time(nullptr);
};

void init_lines() {
	for (int i = 0; i < 16; i++) {

		if (i < 4) {
			line[i].floorType = 0;
			int tree = uidT(gen);
			for (int k = 0; k < 15; k++) {
				line[i].isTree[k] = treeType[tree][k];
			}
		}
		else {
			line[i].floorType = uid(gen);
			int tree = uidT(gen);
			for (int k = 0; k < 15; k++) {
				line[i].isTree[k] = treeType[tree][k];
			}
			line[i].speed = urdS(gen); // 자동차 속도 설정
			line[i].spawnTime = uidTime(gen); // 자동차 생성 시간 설정
		}

		for (int j = 0; j < 15; j++) { // 타일 위치 지정
			line[i].floorPosition[j] = glm::vec3(j - 7.0, 0.0, 2-i);
		}

		line[i].start_time = std::time(nullptr);
		line[i].lineNum = i;
		line[i].direction = urdD(gen);
	}
};

void Line::update_line() {
	std::time_t timeNow = std::time(nullptr);
	if (timeNow - start_time >= spawnTime) {
		for (int i = 0; i < 100; i++) {
			if (!car[i].active) {
				car[i].createCar(speed, floorPosition[0], lineNum, direction);
				start_time = std::time(nullptr);
				break;
			}
		}
	}
};