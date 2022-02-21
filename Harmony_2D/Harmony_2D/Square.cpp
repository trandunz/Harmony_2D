#include "Square.h"

Square::Square()
{
	Init();
}

Square::~Square()
{
	// Unbind
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}
	// Delete
	{
		glDeleteVertexArrays(1, &VertexArrayID);
		glDeleteBuffers(1, &VertBufferID);
		glDeleteBuffers(1, &IndexBufferID);
		glDeleteProgram(ShaderID);
	}
}

void Square::Init()
{
	m_Indices.push_back(0);
	m_Indices.push_back(1);
	m_Indices.push_back(3);

	m_Indices.push_back(0);
	m_Indices.push_back(2);
	m_Indices.push_back(3);

	m_Vertices.push_back({{-0.5f,   0.5f, 0.0f}}); // Top Left
	m_Vertices.push_back({{ 0.5f,   0.5f, 0.0f}}); // Top Right
	m_Vertices.push_back({{-0.5f,  -0.5f, 0.0f}}); // Bottom Left
	m_Vertices.push_back({{ 0.5f,  -0.5f, 0.0f}}); // Bottom Right

	ShaderID = ShaderLoader::CreateShader("Resources/Shaders/basic.vert", "", "Resources/Shaders/basic.frag");

	// Vertex Buffer
	glCreateBuffers(1, &VertBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, VertBufferID);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

	// Index Buffer
	glCreateBuffers(1, &IndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);

	// Vertex Array
	glCreateVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glUseProgram(ShaderID);
	{
		glBindBuffer(GL_ARRAY_BUFFER, VertBufferID);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	}
	glUseProgram(0);
}

void Square::Draw()
{
	glUseProgram(ShaderID);
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
	{
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
