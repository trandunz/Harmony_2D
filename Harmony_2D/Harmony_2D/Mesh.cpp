#include "Mesh.h"

Mesh::Mesh(GLuint _textureID)
{
	Init(_textureID);
}

Mesh::Mesh(Camera& _camera)
{
	Init();
	m_Camera = &_camera;
}

Mesh::~Mesh()
{
	// Unbind
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}
	// Delete
	{
		glDeleteBuffers(1, &UniformBufferID);
		glDeleteVertexArrays(1, &VertexArrayID);
		glDeleteBuffers(1, &VertBufferID);
		glDeleteBuffers(1, &IndexBufferID);
		glDeleteProgram(ShaderID);
	}
	m_Camera = nullptr;
}

void Mesh::Init(GLuint _screenTextureID)
{
	// Indices
	m_Indices.push_back(0);// Top Left
	m_Indices.push_back(1);// Top Right
	m_Indices.push_back(2);// Bottom Right

	m_Indices.push_back(3);// Top Left
	m_Indices.push_back(4);// Bottom Left
	m_Indices.push_back(5);// Bottom Right

	// Vertices
	m_Vertices.push_back({ {1.0f,   -1.0f, 0.0f},{1.0f,0.0f} });
	m_Vertices.push_back({ {-1.0f,   -1.0f, 0.0f},{0.0f,0.0f} });
	m_Vertices.push_back({ {-1.0f,   1.0f, 0.0f},{0.0f,1.0f} });
	m_Vertices.push_back({ {1.0f,   1.0f, 0.0f},{1.0f,1.0f} });
	m_Vertices.push_back({ {1.0f,   -1.0f, 0.0f},{1.0f,0.0f} });
	m_Vertices.push_back({ {-1.0f,   1.0f, 0.0f},{0.0f,1.0f} });

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

	// Shader
	ShaderID = ShaderLoader::CreateShader("Resources/Shaders/frameBuffer.vert", "Resources/Shaders/frameBuffer.frag");
	glUseProgram(ShaderID);
	
	// Layouts
	glBindBuffer(GL_ARRAY_BUFFER, VertBufferID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords)));
	
	glUniform1i(glGetUniformLocation(ShaderID, "screenTexture"), 0);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Mesh::Init()
{
	// Indices
	m_Indices.push_back(0);// Top Left
	m_Indices.push_back(1);// Top Right
	m_Indices.push_back(3);// Bottom Right

	m_Indices.push_back(0);// Top Left
	m_Indices.push_back(2);// Bottom Left
	m_Indices.push_back(3);// Bottom Right

	// Vertices
	m_Vertices.push_back({ {-0.5f,   0.5f, 0.0f} }); // Top Left
	m_Vertices.push_back({ { 0.5f,   0.5f, 0.0f} }); // Top Right
	m_Vertices.push_back({ {-0.5f,  -0.5f, 0.0f} }); // Bottom Left
	m_Vertices.push_back({ { 0.5f,  -0.5f, 0.0f} }); // Bottom Right

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

	// Shader
	ShaderID = ShaderLoader::CreateShader("Resources/Shaders/basic.vert", "Resources/Shaders/basic.frag");
	glUseProgram(ShaderID);

	// Layouts
	glBindBuffer(GL_ARRAY_BUFFER, VertBufferID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords)));

	// Uniform Buffer
	unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(ShaderID, "Matrices");
	glUniformBlockBinding(ShaderID, uniformBlockIndexYellow, 0);
	glCreateBuffers(1, &UniformBufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, UniformBufferID);
	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, uniformBlockIndexYellow, UniformBufferID, 0, 3 * sizeof(glm::mat4));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Mesh::Draw()
{
	// Bind
	glUseProgram(ShaderID);
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, VertBufferID);

	if (m_Camera)
	{
		ProjectionMat = m_Camera->GetProjectionMatrix();
		ViewMat = m_Camera->GetViewMatrix();
		Model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -1));

		// Stream In Proj Mat
		glBindBuffer(GL_UNIFORM_BUFFER, UniformBufferID);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &ProjectionMat[0]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// Stream In View Mat
		glBindBuffer(GL_UNIFORM_BUFFER, UniformBufferID);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &ViewMat[0]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// Stream In View Mat
		glBindBuffer(GL_UNIFORM_BUFFER, UniformBufferID);
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), &Model[0]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	// Draw
	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr);

	// Unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
