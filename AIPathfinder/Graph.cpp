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
			// Create a vertex and push it into vector
			Vertex n;
			n.label = label;
			n.position = glm::vec2(x, y);
			n.vertexStatus = INACTIVE;
			m_vertices.push_back(n);

		}

		// Check if Edge
		int start = NULL, end = NULL, fs2 = NULL, cost = NULL;
		sscanf(str.c_str(), "%d--%d [fontsize=\"%d\",label=\"%d\"];", &start, &end, &fs2, &cost);
		if (end + fs2 + cost != 0) {
			// Create an edge and push it into vector
			Edge n;
			n.end = &m_vertices[end];
			n.cost = cost;
			n.edgeStatus = INACTIVE;
			// Connect edge to start vertex
			m_vertices[start].edges.push_back(n);
			m_childCount++;

			Edge m;
			m.end = &m_vertices[start];
			m.cost = cost;
			m.edgeStatus = INACTIVE;
			m_vertices[end].edges.push_back(m);

			m_childCount++;
		}
	}
	std::cout << m_vertices.size() << " vertices found!" << std::endl;
}

void Graph::createCircle(glm::vec2 pos, glm::vec4 color, int num)
{
	GLfloat twicePi = 2.0f * 3.14159265359;

	glm::vec2 circleCentre = pos;
	const int numOfVertices = 46;
	numOfCircleVertex = numOfVertices;
	const float radius = 10;
	GLfloat circleCoords[numOfVertices * 3];

	// Calculate the points of the circle
	for (int i = 0; i < numOfVertices * 3; i += 3) {
		circleCoords[i] = circleCentre.x + (radius * cos(i *  twicePi / numOfVertices));
		circleCoords[i + 1] = 720 - (circleCentre.y +(radius * sin(i * twicePi / numOfVertices)));
		circleCoords[i + 2] = 0;
	}

	// Generate buffers for rendering
	m_shader->use();
	GLuint VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(vertexVAO[num]);

	// Pass vertex (circle points) into the shader
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circleCoords), circleCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid*)0);

	glUniform4f(glGetUniformLocation(m_shader->getID(), "color"), color.x, color.y, color.z, color.w);

	// Unbind all
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Graph::createLine(glm::vec2 start, glm::vec2 end, glm::vec4 color, int num)
{
	const int coordNum = 3 * 2;
	GLfloat lineCoords[coordNum];
	lineCoords[0] = start.x;
	lineCoords[1] = 720 - start.y;
	lineCoords[2] = 0;
	lineCoords[3] = end.x;
	lineCoords[4] = 720 - end.y;
	lineCoords[5] = 0;

	m_shader->use();
	GLuint VBO;
	glGenBuffers(1, &VBO);

	glBindVertexArray(edgeVAO[num]);

	// Pass vertex (circle points) into the shader
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineCoords), lineCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid*)0);

	glUniform4f(glGetUniformLocation(m_shader->getID(), "color"), color.x, color.y, color.z, color.w);

	// Unbind all
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

// Check colour of Edge/Vertex depending on it's current status
glm::vec4 Graph::checkColour(status sta)
{
	glm::vec4 color;
	switch (sta) {
	case INACTIVE:
		color = glm::vec4(1, 1, 1, 0.5); // White
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

void Graph::colorReset()
{
	for (int i = 0; i < m_vertices.size(); i++) {
		m_vertices[i].vertexStatus = INACTIVE;
		//glm::vec4 vColour = checkColour(m_vertices[i].vertexStatus);
		//glUniform4f(glGetUniformLocation(m_shader->getID(), "color"), vColour.x, vColour.y, vColour.z, vColour.w);
		for (int j = 0; j < m_vertices[i].edges.size(); j++) {
			m_vertices[i].edges[j].edgeStatus = INACTIVE;
		}
	}
}

int Graph::heuristic(glm::vec2 a, glm::vec2 b)
{
	return abs(a.x - b.x) + abs(a.y - b.y);
}

// Based on the A* algorithm by Red Blob Games (https://www.redblobgames.com/pathfinding/a-star/introduction.html)
void Graph::astarFind(int start, int end)
{
	PriorityQueue<int, int> frontier;
	frontier.put(start, 0);
	std::vector<int> came_from;
	came_from.resize(m_vertices.size());
	came_from[start] = 0;
	std::vector<int> cost_so_far;
	cost_so_far.resize(m_vertices.size());
	cost_so_far[start] = 0;

	int current;

	while (!frontier.empty()) {
		current = frontier.get();
		if (current == end) {
			std::cout << "Path found!" << std::endl;
			break;
		}
		else {
			std::cout << "Current Vertex: " << current << std::endl;
			for (int i = 0; i < m_vertices[current].edges.size(); i++) {
				int next = m_vertices[current].edges[i].end->label;
				int new_cost = cost_so_far[current] + m_vertices[current].edges[i].cost;
				if (cost_so_far[next] == NULL || new_cost < cost_so_far[next]) {
					cost_so_far[next] = new_cost;
					int priority = new_cost + heuristic(m_vertices[end].position, m_vertices[next].position);
					frontier.put(next, priority);
					came_from[next] = current;
				}
			}
		}
	}
	int r_current = end;
	std::cout << "End: " << end << std::endl;
	for (int i = 0; i < came_from.size(); i++) {
		if(r_current == start){
			std::cout << r_current << std::endl;
			std::cout << "Boop" << std::endl;
			break;
		}
		else {
			m_vertices[r_current].vertexStatus = ACTIVE;
			int next = came_from[r_current];
			for (int j = 0; j < m_vertices[r_current].edges.size(); j++) {
				if (m_vertices[r_current].edges[j].end->label == next) {
					m_vertices[r_current].edges[j].edgeStatus = ACTIVE;
				}
			}
			for (int j = 0; j < m_vertices[next].edges.size(); j++) {
				if (m_vertices[next].edges[j].end->label == r_current) {
					m_vertices[next].edges[j].edgeStatus = ACTIVE;
				}
			}
			r_current = next;
		}
	}

	m_vertices[start].vertexStatus = START;
	m_vertices[end].vertexStatus = TARGET;
}

Graph::Graph(const char * filename)
{
	m_shader = new Shader("Shaders/Image.vert", "Shaders/Image.frag");
	parseFile(filename);

	vertexVAO = new GLuint[m_vertices.size()];
	glGenVertexArrays(m_vertices.size(), vertexVAO);
	edgeVAO = new GLuint[m_childCount];
	glGenVertexArrays(m_childCount, edgeVAO);

	int vaochildCount= 0;
	// Draw Vertex
	for (int i = 0; i < m_vertices.size(); i++) {
		createCircle(m_vertices[i].position, checkColour(m_vertices[i].vertexStatus), i);
		for (int j = 0; j < m_vertices[i].edges.size(); j++) {
			vaochildCount++;
			m_vertices[i].edges[j].edgeNum = vaochildCount;
			createLine(m_vertices[i].position, m_vertices[i].edges[j].end->position, checkColour(m_vertices[i].edges[j].edgeStatus), vaochildCount);
		}
	}
}

Graph::~Graph()
{
}

int Graph::getVertexCount()
{
	return m_vertices.size();
}

void Graph::scale(float scaleAmount)
{
	m_modelMat = glm::scale(m_modelMat, glm::vec3(scaleAmount, scaleAmount, 0));
}

void Graph::draw(glm::mat4 view)
{
	m_shader->use();
	glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f);
	//glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(glGetUniformLocation(m_shader->getID(), "imgProj"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(m_shader->getID(), "imgView"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(m_shader->getID(), "imgModel"), 1, GL_FALSE, glm::value_ptr(m_modelMat));

	for (int i = 0; i < m_vertices.size(); i++) {
		glBindVertexArray(vertexVAO[i]);
		glm::vec4 vColour = checkColour(m_vertices[i].vertexStatus);
		glUniform4f(glGetUniformLocation(m_shader->getID(), "color"), vColour.x, vColour.y, vColour.z, vColour.w);
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(4);
		glDrawArrays(GL_LINE_LOOP, 0, numOfCircleVertex);
		glBindVertexArray(0);
	}

	for (int i = 0; i < m_childCount; i++) {
		glBindVertexArray(edgeVAO[i]);
		for (int i = 0; i < m_vertices.size(); i++){
			for (int j = 0; j < m_vertices[i].edges.size(); j++) {
				glm::vec4 eColour = checkColour(m_vertices[i].edges[j].edgeStatus);
					glUniform4f(glGetUniformLocation(m_shader->getID(), "color"), eColour.x, eColour.y, eColour.z, eColour.w);
			}
		}
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);
	}
}

void Graph::find(algorithm algo, int start, int end)
{
	colorReset();
	switch (algo) {
	case A_STAR:
		astarFind(start, end);
		break;
	}
}
