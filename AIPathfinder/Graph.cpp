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

void Graph::drawCircle(glm::vec2 pos, glm::vec4 color)
{

}

void Graph::drawLine(glm::vec2 start, glm::vec2 end, glm::vec4 colour)
{

}

// Check colour of Edge/Vertex depending on it's current status
glm::vec4 Graph::checkColour(status sta)
{
	glm::vec4 color;
	switch (sta) {
	case INACTIVE:
		color = glm::vec4(1, 1, 1, 1); // White
		break;
	case ACTIVE:
		color = glm::vec4(0.043, 0.980, 0, 1); // Green
		break;
	case START: 
		color = glm::vec4(0.984, 0.980, 0.074, 1); // Red
		break;
	case TARGET:
		color = glm::vec4(0.894, 0.188, 0.066, 1); // Yellow
		break;
	default:
		color = glm::vec4(0, 0, 0, 1); // Black
		break;
	}
	return color;
}

Graph::Graph(const char * filename)
{
	//m_shader = new Shader("Image.vert", "Image.frag");
	parseFile(filename);
}

void Graph::draw()
{
	glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f);
	// Draw Vertex
	for (int i = 0; m_vertices.size(); i++) {
		//drawCircle(m_vertices[i].position, );
	}

	// Draw Edge
	for (int i = 0; m_edges.size(); i++) {
		//drawLine()
	}
}

void Graph::find(algorithm algo, int start, int end)
{
}
