#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <GL/glew.h>

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
	void parseFile(const char* filename);
	std::ifstream m_graphFile;
	std::vector<Vertex> m_vertices;
	std::vector<Edge> m_edges;
	
public:
	Graph(const char* filename);
	void draw();
	~Graph();
};