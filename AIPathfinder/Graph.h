#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <GL/glew.h>
#include "Shader.h"


class Graph {
private:
	// Priority Queue for pathfinding algorithms by Red Blob Games (https://www.redblobgames.com/pathfinding/a-star/implementation.html#cpp-dijkstra)
	template<typename T, typename priority_t>
	struct PriorityQueue {
		typedef std::pair<priority_t, T> PQElement;
		std::priority_queue<PQElement, std::vector<PQElement>,
			std::greater<PQElement>> elements;

		inline bool empty() const {
			return elements.empty();
		}

		inline void put(T item, priority_t priority) {
			elements.emplace(priority, item);
		}

		T get() {
			T best_item = elements.top().second;
			elements.pop();
			return best_item;
		}
	};

	// Vertex Status
	enum status {
		INACTIVE, ACTIVE, START, TARGET
	};

	// Vertex and Edges
	struct Edge;
	struct Vertex {
		int label;
		glm::vec2 position;
		//std::vector<Vertex> child;
		std::vector<Edge> edges;
		status vertexStatus;
	};

	struct Edge {
		Vertex* end;
		int cost;
		int edgeNum;
		status edgeStatus;
	};

	std::vector<Vertex> m_vertices;
	//std::vector<Edge> m_edges;
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

	void colorReset();
	void astarFind(int start, int end);
	int heuristic(glm::vec2 a, glm::vec2 b);
public:
	Graph(const char* filename);
	~Graph();

	enum algorithm {
		A_STAR, IDA_STAR, DIJKSTRA
	};

	int getVertexCount();
	void scale(float scaleAmount);
	void draw(glm::mat4 view);
	void find(algorithm algo, int start, int end);

};