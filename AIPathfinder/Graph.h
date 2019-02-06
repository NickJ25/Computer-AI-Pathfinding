#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include "Shader.h"

struct Vertex {
	int label;
	glm::vec2 position;
	std::vector<Vertex> child;
};

struct Edge {
	Vertex* from;
	Vertex* to;
	int cost;
};

class Graph {
private:
	enum status {
		INACTIVE, ACTIVE, START, TARGET
	};
	void parseFile(const char* filename);
	std::ifstream m_graphFile;
	std::vector<Vertex> m_vertices;
	std::vector<Edge> m_edges;

	Shader* m_shader;
	
	void drawCircle(glm::vec2 pos, glm::vec4 color);
	void drawLine(glm::vec2 start, glm::vec2 end, glm::vec4 colour);
	glm::vec4 checkColour(status sta);
public:
	enum algorithm {
		A_STAR, IDA_STAR, DIJKSTRA
	};

	Graph(const char* filename);
	~Graph();
	void draw();
	void find(algorithm algo, int start, int end);
};