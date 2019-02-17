#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include "Shader.h"

class Graph {
private:

	// Vertex Status
	enum status {
		INACTIVE, ACTIVE, START, TARGET
	};

	// Vertex and Edges
	struct Vertex {
		int label;
		glm::vec2 position;
		std::vector<Vertex> child;
		status vertexStatus;
	};

	std::vector<Vertex> m_vertices;
	unsigned int m_childCount = 0;

	void parseFile(const char* filename);
	std::ifstream m_graphFile;
	Shader* m_shader;

	GLuint *vertexVAO, *edgeVAO;
	int numOfCircleVertex;
	glm::mat4 m_modelMat = glm::mat4(1.0);

	void createCircle(glm::vec2 pos, glm::vec4 color, int num);
	void createLine(glm::vec2 start, glm::vec2 end, glm::vec4 colour, int num);
	glm::vec4 checkColour(status sta);
public:
	enum algorithm {
		A_STAR, IDA_STAR, DIJKSTRA
	};

	Graph(const char* filename);
	~Graph();
	void scale(float scaleAmount);
	void draw(glm::mat4 view);
	void find(algorithm algo, int start, int end);
};