#include "Graph.h"

void Graph::parseFile(const char * filename)
{
	m_graphFile.open(filename);
	
	if (m_graphFile.fail()) {
		std::cout << "Error opening file: " << filename << std::endl;
		exit(1);
	}
	
	// Go through each line and check if it's Edge or Vertex
	std::string str;
	while (std::getline(m_graphFile, str)) {
		
		// Check if Vertex
		int label = NULL, fs = NULL, x = NULL, y = NULL;
		sscanf(str.c_str(), "%d[fontsize=\"%d\",pos=\"%d,%d\"];", &label, &fs, &x, &y);
		if (fs + x + y != 0) {
			Vertex n;
			n.label = label;
			n.position = glm::vec2(x, y);
			m_vertices.push_back(n);
		}

		// Check if Edge
		int start = NULL, end = NULL, fs2 = NULL, cost = NULL;
		sscanf(str.c_str(), "%d--%d [fontsize=\"%d\",label=\"%d\"];", &start, &end, &fs2, &cost);
		if (end + fs2 + cost != 0) {
			Edge n;
			n.from = &m_vertices[start];
			m_vertices[start].child.push_back(m_vertices[end]);
			n.to = &m_vertices[end];
			n.cost = cost;
			m_edges.push_back(n);
		}
	}

	std::cout << m_vertices.size() << std::endl;
	for (int i = 0; i < m_vertices.size(); i++) {
		std::cout << "[" << m_vertices[i].label << "] \n\t POS: (" << m_vertices[i].position.x << "," << m_vertices[i].position.y << ")" << std::endl;
		for (int j = 0; j < m_vertices[i].child.size(); j++) {
			std::cout << "\t[" << j << "] " << m_vertices[i].child[j].label << std::endl;
		}
	}
}

Graph::Graph(const char * filename)
{
	parseFile(filename);
}

void Graph::draw()
{

}
